/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "restapimethod.h"
#include "restapirequest.h"
#include "rocketchatqtrestapi_debug.h"

#include "serverinfojob.h"
#include "uploadfilejob.h"
#include "settings/privateinfojob.h"
#include "channellistjob.h"
#include "downloadfilejob.h"
#include "spotlightjob.h"

#include "users/getavatarjob.h"
#include "users/setavatarjob.h"
#include "users/forgotpasswordjob.h"
#include "users/usersinfojob.h"
#include "users/getpresencejob.h"
#include "users/getusernamesuggestionjob.h"

#include "misc/owninfojob.h"

#include "emoji/loademojicustomjob.h"

#include "authentication/logoutjob.h"
#include "authentication/loginjob.h"

#include "chat/starmessagejob.h"
#include "chat/postmessagejob.h"
#include "chat/deletemessagejob.h"
#include "chat/updatemessagejob.h"
#include "chat/reactonmessagejob.h"
#include "chat/searchmessagejob.h"
#include "chat/ignoreuserjob.h"
#include "chat/reportmessagejob.h"
#include "chat/pinmessagejob.h"
#include "chat/followmessagejob.h"
#include "chat/unfollowmessagejob.h"
#include "chat/getthreadsjob.h"
#include "chat/getthreadmessagesjob.h"
#include "chat/sendmessagejob.h"

#include "channels/changechanneltopicjob.h"
#include "channels/changechannelannouncementjob.h"
#include "channels/createchanneljob.h"
#include "channels/leavechanneljob.h"
#include "channels/channelclosejob.h"
#include "channels/channelhistoryjob.h"
#include "channels/changechanneldescriptionjob.h"
#include "channels/archivechanneljob.h"
#include "channels/channelfilesjob.h"
#include "channels/channelinvitejob.h"
#include "channels/setchanneltypejob.h"
#include "channels/getchannelrolesjob.h"
#include "channels/setjoincodechanneljob.h"
#include "channels/channeljoinjob.h"
#include "channels/channelinfojob.h"
#include "channels/changechannelnamejob.h"
#include "channels/channelgetallusermentionsjob.h"
#include "channels/channelkickjob.h"
#include "channels/channeladdownerjob.h"
#include "channels/channelremoveownerjob.h"
#include "channels/channeladdmoderatorjob.h"
#include "channels/channelremovemoderatorjob.h"
#include "channels/channeldeletejob.h"
#include "channels/channelmembersjob.h"
#include "channels/changechannelreadonlyjob.h"
#include "channels/changechannelencryptedjob.h"
#include "channels/channeladdleaderjob.h"
#include "channels/channelremoveleaderjob.h"

#include "groups/changegroupsannouncementjob.h"
#include "groups/changegroupstopicjob.h"
#include "groups/creategroupsjob.h"
#include "groups/leavegroupsjob.h"
#include "groups/changegroupsdescriptionjob.h"
#include "groups/archivegroupsjob.h"
#include "groups/groupsinvitejob.h"
#include "groups/setgrouptypejob.h"
#include "groups/getgrouprolesjob.h"
#include "groups/changegroupsnamejob.h"
#include "groups/groupsinfojob.h"
#include "groups/groupskickjob.h"
#include "groups/groupaddleaderjob.h"
#include "groups/groupremoveleaderjob.h"
#include "groups/groupaddownerjob.h"
#include "groups/groupremoveownerjob.h"
#include "groups/groupaddmoderatorjob.h"
#include "groups/groupremovemoderatorjob.h"
#include "groups/groupsdeletejob.h"
#include "groups/changegroupsreadonlyjob.h"
#include "groups/changegroupsencryptedjob.h"

#include "rooms/getroomsjob.h"
#include "rooms/roomfavoritejob.h"
#include "rooms/savenotificationjob.h"
#include "rooms/roomstartdiscussionjob.h"
#include "rooms/getdiscussionsjob.h"

#include "directmessage/createdmjob.h"
#include "directmessage/opendmjob.h"

#include "subscriptions/markroomasreadjob.h"
#include "subscriptions/markroomasunreadjob.h"

#include "permissions/listpermissionsjob.h"

#include "commands/listcommandsjob.h"

#include "e2e/fetchmykeysjob.h"

#include "video-conference/videoconfupdatejitsitimeoutjob.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
using namespace RocketChatRestApi;
RestApiRequest::RestApiRequest(QObject *parent)
    : QObject(parent)
{
    mRestApiMethod = new RestApiMethod;
    mCookieJar = new QNetworkCookieJar;
    mNetworkAccessManager = new QNetworkAccessManager(this);
    mNetworkAccessManager->setCookieJar(mCookieJar);
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &RestApiRequest::slotResult);
    connect(mNetworkAccessManager, &QNetworkAccessManager::sslErrors, this, &RestApiRequest::slotSslErrors);
}

RestApiRequest::~RestApiRequest()
{
    delete mRestApiMethod;
}

void RestApiRequest::setRestApiLogger(RocketChatRestApi::AbstractLogger *logger)
{
    mRuqolaLogger = logger;
}

void RestApiRequest::initializeCookies()
{
    QString url = serverUrl();
    if (!url.isEmpty()) {
        QString host;
        QStringList lsthost = url.split(QStringLiteral("//"));
        if (lsthost.count() < 2) {
            host = url;
        } else {
            host = lsthost.at(1);
        }

        if (!mUserId.isEmpty()) {
            QNetworkCookie userIdCookie;
            userIdCookie.setDomain(host);
            userIdCookie.setName(QByteArrayLiteral("rc_uid"));
            userIdCookie.setValue(mUserId.toLocal8Bit());
            mCookieJar->insertCookie(userIdCookie);
        }

        if (!mAuthToken.isEmpty()) {
            QNetworkCookie tokenCookie;
            tokenCookie.setDomain(host);
            tokenCookie.setName(QByteArrayLiteral("rc_token"));
            tokenCookie.setValue(mAuthToken.toLocal8Bit());
            mCookieJar->insertCookie(tokenCookie);
        }
    } else {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "We can not initialize cookies as server url is empty.";
    }
}

void RestApiRequest::setAuthToken(const QString &authToken)
{
    const bool isChanged = (mAuthToken != authToken);
    mAuthToken = authToken;
    if (isChanged) {
        if (!mAuthToken.isEmpty()) {
            initializeCookies();
        }
    }
}

void RestApiRequest::setUserId(const QString &userId)
{
    const bool isChanged = (mUserId != userId);
    mUserId = userId;
    if (isChanged) {
        if (!mUserId.isEmpty()) {
            initializeCookies();
        }
    }
}

void RestApiRequest::slotResult(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << " Error reply - "<<reply->errorString();
    }
}

void RestApiRequest::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)
{
    qCDebug(ROCKETCHATQTRESTAPI_LOG) << " void RestApiRequest::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)" << error.count();
    reply->ignoreSslErrors(error);
}

void RestApiRequest::setPassword(const QString &password)
{
    mPassword = password;
}

void RestApiRequest::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString RestApiRequest::serverUrl() const
{
    return mRestApiMethod->serverUrl();
}

void RestApiRequest::setServerUrl(const QString &serverUrl)
{
    mRestApiMethod->setServerUrl(serverUrl);
}

QString RestApiRequest::authToken() const
{
    return mAuthToken;
}

QString RestApiRequest::userId() const
{
    return mUserId;
}

void RestApiRequest::login()
{
    LoginJob *job = new LoginJob(this);
    connect(job, &LoginJob::loginDone, this, &RestApiRequest::slotLogin);
    initializeRestApiJob(job);
    job->setPassword(mPassword);
    job->setUserName(mUserName);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::slotLogin(const QString &authToken, const QString &userId)
{
    mAuthToken = authToken;
    mUserId = userId;
}

void RestApiRequest::slotLogout()
{
    mUserId.clear();
    mAuthToken.clear();
    Q_EMIT logoutDone();
}

void RestApiRequest::initializeRestApiJob(RocketChatRestApi::RestApiAbstractJob *job)
{
    connect(job, &RocketChatRestApi::RestApiAbstractJob::failed, this, &RestApiRequest::failed);
    job->setNetworkAccessManager(mNetworkAccessManager);
    job->setRestApiLogger(mRuqolaLogger);
    job->setRestApiMethod(mRestApiMethod);
    if (job->requireHttpAuthentication()) {
        job->setAuthToken(mAuthToken);
        job->setUserId(mUserId);
    }
}

void RestApiRequest::logout()
{
    LogoutJob *job = new LogoutJob(this);
    connect(job, &LogoutJob::logoutDone, this, &RestApiRequest::slotLogout);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::channelList()
{
    ChannelListJob *job = new ChannelListJob(this);
    connect(job, &ChannelListJob::channelListDone, this, &RestApiRequest::channelListDone);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::getAvatar(const QString &userId)
{
    GetAvatarJob *job = new GetAvatarJob(this);
    connect(job, &GetAvatarJob::avatar, this, &RestApiRequest::avatar);
    connect(job, &GetAvatarJob::redownloadAvatar, this, &RestApiRequest::redownloadAvatar);
    initializeRestApiJob(job);
    job->setAvatarUserId(userId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::getPrivateSettings()
{
    PrivateInfoJob *job = new PrivateInfoJob(this);
    connect(job, &PrivateInfoJob::privateInfoDone, this, &RestApiRequest::privateInfoDone);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::getOwnInfo()
{
    OwnInfoJob *job = new OwnInfoJob(this);
    connect(job, &OwnInfoJob::ownInfoDone, this, &RestApiRequest::getOwnInfoDone);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::starMessage(const QString &messageId, bool starred)
{
    StarMessageJob *job = new StarMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    job->setStarMessage(starred);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::downloadFile(const QUrl &url, const QString &mimeType, bool storeInCache, const QUrl &localFileUrl)
{
    DownloadFileJob *job = new DownloadFileJob(this);
    connect(job, &DownloadFileJob::downloadFileDone, this, &RestApiRequest::downloadFileDone);
    job->setUrl(url);
    job->setMimeType(mimeType);
    job->setLocalFileUrl(localFileUrl);
    job->setStoreInCache(storeInCache);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::serverInfo()
{
    ServerInfoJob *job = new ServerInfoJob(this);
    initializeRestApiJob(job);
    connect(job, &ServerInfoJob::serverInfoDone, this, &RestApiRequest::getServerInfoDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::uploadFile(const QString &roomId, const QString &description, const QString &text, const QUrl &filename)
{
    UploadFileJob *job = new UploadFileJob(this);
    initializeRestApiJob(job);
    job->setDescription(description);
    job->setMessageText(text);
    job->setFilenameUrl(filename);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeChannelTopic(const QString &roomId, const QString &topic)
{
    ChangeChannelTopicJob *job = new ChangeChannelTopicJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setTopic(topic);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeGroupsTopic(const QString &roomId, const QString &topic)
{
    ChangeGroupsTopicJob *job = new ChangeGroupsTopicJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setTopic(topic);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeChannelReadOnly(const QString &roomId, bool b)
{
    ChangeChannelReadonlyJob *job = new ChangeChannelReadonlyJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setReadOnly(b);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeGroupsReadOnly(const QString &roomId, bool b)
{
    ChangeGroupsReadonlyJob *job = new ChangeGroupsReadonlyJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setReadOnly(b);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeChannelEncrypted(const QString &roomId, bool b)
{
    ChangeChannelEncryptedJob *job = new ChangeChannelEncryptedJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setEncrypted(b);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeGroupsEncrypted(const QString &roomId, bool b)
{
    ChangeGroupsEncryptedJob *job = new ChangeGroupsEncryptedJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setEncrypted(b);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeChannelAnnouncement(const QString &roomId, const QString &announcement)
{
    ChangeChannelAnnouncementJob *job = new ChangeChannelAnnouncementJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setAnnouncement(announcement);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeGroupsAnnouncement(const QString &roomId, const QString &announcement)
{
    ChangeGroupsAnnouncementJob *job = new ChangeGroupsAnnouncementJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setAnnouncement(announcement);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeChannelDescription(const QString &roomId, const QString &description)
{
    ChangeChannelDescriptionJob *job = new ChangeChannelDescriptionJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setDescription(description);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeGroupsDescription(const QString &roomId, const QString &description)
{
    ChangeGroupsDescriptionJob *job = new ChangeGroupsDescriptionJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setDescription(description);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::postMessage(const QString &roomId, const QString &text)
{
    PostMessageJob *job = new PostMessageJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setText(text);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::deleteMessage(const QString &roomId, const QString &messageId)
{
    DeleteMessageJob *job = new DeleteMessageJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setMessageId(messageId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::slotAddJoinCodeToChannel(const QString &channelId, const QString &password)
{
    setJoinCodeChannel(channelId, password);
}

void RestApiRequest::createChannels(const QString &channelName, bool readOnly, const QStringList &members, const QString &password)
{
    CreateChannelJob *job = new CreateChannelJob(this);
    connect(job, &CreateChannelJob::addJoinCodeToChannel, this, &RestApiRequest::slotAddJoinCodeToChannel);
    initializeRestApiJob(job);
    job->setChannelName(channelName);
    job->setReadOnly(readOnly);
    job->setMembers(members);
    job->setPassword(password);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::createGroups(const QString &channelName, bool readOnly, const QStringList &members)
{
    CreateGroupsJob *job = new CreateGroupsJob(this);
    initializeRestApiJob(job);
    job->setChannelName(channelName);
    job->setReadOnly(readOnly);
    job->setMembers(members);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::leaveChannel(const QString &roomId)
{
    LeaveChannelJob *job = new LeaveChannelJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::leaveGroups(const QString &roomId)
{
    LeaveGroupsJob *job = new LeaveGroupsJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::archiveChannel(const QString &roomId, bool archive)
{
    ArchiveChannelJob *job = new ArchiveChannelJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setArchive(archive);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::archiveGroups(const QString &roomId, bool archive)
{
    ArchiveGroupsJob *job = new ArchiveGroupsJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setArchive(archive);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::updateMessage(const QString &roomId, const QString &messageId, const QString &text)
{
    UpdateMessageJob *job = new UpdateMessageJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setMessageId(messageId);
    job->setUpdatedText(text);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::reactOnMessage(const QString &messageId, const QString &emoji, bool shouldReact)
{
    ReactOnMessageJob *job = new ReactOnMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    job->setEmoji(emoji);
    job->setShouldReact(shouldReact);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::closeChannel(const QString &roomId, const QString &type)
{
    ChannelCloseJob *job = new ChannelCloseJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (type == QLatin1String("d")) {
        job->setChannelType(ChannelCloseJob::Direct);
    } else if (type == QLatin1String("p")) {
        job->setChannelType(ChannelCloseJob::Groups);
    } else if (type == QLatin1String("c")) {
        job->setChannelType(ChannelCloseJob::Channel);
    }
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::historyChannel(const QString &roomId, const QString &type)
{
    ChannelHistoryJob *job = new ChannelHistoryJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (type == QLatin1String("d")) {
        job->setChannelType(ChannelHistoryJob::Direct);
    } else if (type == QLatin1String("p")) {
        job->setChannelType(ChannelHistoryJob::Groups);
    } else if (type == QLatin1String("c")) {
        job->setChannelType(ChannelHistoryJob::Channel);
    }
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::createDirectMessage(const QString &userName)
{
    CreateDmJob *job = new CreateDmJob(this);
    initializeRestApiJob(job);
    job->setUserName(userName);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::openDirectMessage(const QString &userId)
{
    OpenDmJob *job = new OpenDmJob(this);
    initializeRestApiJob(job);
    job->setDirectUserId(userId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::filesInRoom(const QString &roomId, const QString &type, int offset, int count)
{
    ChannelFilesJob *job = new ChannelFilesJob(this);
    connect(job, &ChannelFilesJob::channelFilesDone, this, &RestApiRequest::channelFilesDone);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    QueryParameters parameters;
    parameters.setCount(count);
    parameters.setOffset(offset);
    job->setQueryParameters(parameters);


    if (type == QLatin1String("d")) {
        job->setChannelType(ChannelFilesJob::Direct);
    } else if (type == QLatin1String("p")) {
        job->setChannelType(ChannelFilesJob::Groups);
    } else if (type == QLatin1String("c")) {
        job->setChannelType(ChannelFilesJob::Channel);
    }
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::membersInRoom(const QString &roomId, const QString &type)
{
    ChannelMembersJob *job = new ChannelMembersJob(this);
    connect(job, &ChannelMembersJob::channelMembersDone, this, &RestApiRequest::channelMembersDone);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (type == QLatin1String("d")) {
        job->setChannelType(ChannelMembersJob::Direct);
    } else if (type == QLatin1String("p")) {
        job->setChannelType(ChannelMembersJob::Groups);
    } else if (type == QLatin1String("c")) {
        job->setChannelType(ChannelMembersJob::Channel);
    }
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::addUserInChannel(const QString &roomId, const QString &userId)
{
    ChannelInviteJob *job = new ChannelInviteJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setInviteUserId(userId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::addUserInGroup(const QString &roomId, const QString &userId)
{
    GroupsInviteJob *job = new GroupsInviteJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setUserId(userId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::listEmojiCustom()
{
    LoadEmojiCustomJob *job = new LoadEmojiCustomJob(this);
    initializeRestApiJob(job);
    connect(job, &LoadEmojiCustomJob::loadEmojiCustomDone, this, &RestApiRequest::loadEmojiCustomDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::searchRoomUser(const QString &pattern)
{
    SpotlightJob *job = new SpotlightJob(this);
    job->setSearchPattern(pattern);
    initializeRestApiJob(job);
    connect(job, &SpotlightJob::spotlightDone, this, &RestApiRequest::spotlightDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::searchMessages(const QString &roomId, const QString &pattern)
{
    SearchMessageJob *job = new SearchMessageJob(this);
    job->setRoomId(roomId);
    job->setSearchText(pattern);
    initializeRestApiJob(job);
    connect(job, &SearchMessageJob::searchMessageDone, this, &RestApiRequest::searchMessageDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::markAsRead(const QString &roomId)
{
    MarkRoomAsReadJob *job = new MarkRoomAsReadJob(this);
    job->setRoomId(roomId);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::markRoomAsUnRead(const QString &roomId)
{
    MarkRoomAsUnReadJob *job = new MarkRoomAsUnReadJob(this);
    job->setObjectId(roomId);
    job->setUnReadObject(MarkRoomAsUnReadJob::Room);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::markMessageAsUnReadFrom(const QString &messageId)
{
    MarkRoomAsUnReadJob *job = new MarkRoomAsUnReadJob(this);
    job->setObjectId(messageId);
    job->setUnReadObject(MarkRoomAsUnReadJob::FromMessage);
    initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::getRooms()
{
    GetRoomsJob *job = new GetRoomsJob(this);
    initializeRestApiJob(job);
    connect(job, &GetRoomsJob::getRoomsDone, this, &RestApiRequest::getRoomsDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::markAsFavorite(const QString &roomId, bool favorite)
{
    RoomFavoriteJob *job = new RoomFavoriteJob(this);
    initializeRestApiJob(job);
    job->setFavorite(favorite);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::disableNotifications(const QString &roomId, bool value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setDisableNotifications(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::muteGroupMentions(const QString &roomId, bool value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setMuteGroupMentions(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::desktopDurationNotifications(const QString &roomId, int value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setDesktopNotificationDuration(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::desktopSoundNotifications(const QString &roomId, const QString &value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setAudioNotificationValue(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeGroupName(const QString &roomId, const QString &newName)
{
    ChangeGroupsNameJob *job = new ChangeGroupsNameJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setName(newName);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::changeChannelName(const QString &roomId, const QString &newName)
{
    ChangeChannelNameJob *job = new ChangeChannelNameJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setName(newName);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::channelInfo(const QString &roomId)
{
    ChannelInfoJob *job = new ChannelInfoJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::groupInfo(const QString &roomId)
{
    GroupsInfoJob *job = new GroupsInfoJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::hideUnreadStatus(const QString &roomId, bool value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setHideUnreadStatus(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::audioNotifications(const QString &roomId, const QString &value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setAudioNotifications(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::desktopNotifications(const QString &roomId, const QString &value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setDesktopNotifications(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::emailNotifications(const QString &roomId, const QString &value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setEmailNotifications(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::mobilePushNotifications(const QString &roomId, const QString &value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setMobilePushNotifications(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::unreadAlert(const QString &roomId, const QString &value)
{
    SaveNotificationJob *job = new SaveNotificationJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setUnreadAlert(value);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::setAvatar(const QString &avatarUrl)
{
    SetAvatarJob *job = new SetAvatarJob(this);
    initializeRestApiJob(job);
    job->setAvatarUrl(avatarUrl);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::forgotPassword(const QString &email)
{
    ForgotPasswordJob *job = new ForgotPasswordJob(this);
    initializeRestApiJob(job);
    job->setEmail(email);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::userInfo(const QString &identifier, bool userName)
{
    UsersInfoJob *job = new UsersInfoJob(this);
    initializeRestApiJob(job);
    job->setIdentifier(identifier);
    job->setUseUserName(userName);
    connect(job, &UsersInfoJob::usersInfoDone, this, &RestApiRequest::usersInfoDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::ignoreUser(const QString &roomId, const QString &userId, bool ignore)
{
    IgnoreUserJob *job = new IgnoreUserJob(this);
    initializeRestApiJob(job);
    job->setIgnoreUserId(userId);
    job->setRoomId(roomId);
    job->setIgnore(ignore);
    connect(job, &IgnoreUserJob::ignoreUserDone, this, &RestApiRequest::ignoreUserDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::userPresence(const QString &userId)
{
    GetPresenceJob *job = new GetPresenceJob(this);
    initializeRestApiJob(job);
    job->setPresenceUserId(userId);
    connect(job, &GetPresenceJob::getPresenceDone, this, &RestApiRequest::getPresenceDone);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::reportMessage(const QString &messageId, const QString &message)
{
    ReportMessageJob *job = new ReportMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    job->setReportMessage(message);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::setGroupType(const QString &roomId, bool isPrivate)
{
    SetGroupTypeJob *job = new SetGroupTypeJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setType(isPrivate ? SetGroupTypeJob::Private : SetGroupTypeJob::Public);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::setChannelType(const QString &roomId, bool isPrivate)
{
    SetChannelTypeJob *job = new SetChannelTypeJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    job->setType(isPrivate ? SetChannelTypeJob::Private : SetChannelTypeJob::Public);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}

void RestApiRequest::getGroupRoles(const QString &roomId)
{
    GetGroupRolesJob *job = new GetGroupRolesJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    connect(job, &GetGroupRolesJob::groupRolesDone, this, &RestApiRequest::channelRolesDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getGroupRoles job";
    }
}

void RestApiRequest::getChannelRoles(const QString &roomId)
{
    GetChannelRolesJob *job = new GetChannelRolesJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    connect(job, &GetChannelRolesJob::channelRolesDone, this, &RestApiRequest::channelRolesDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetChannelRolesJob job";
    }
}

void RestApiRequest::getUsernameSuggestion()
{
    GetUsernameSuggestionJob *job = new GetUsernameSuggestionJob(this);
    initializeRestApiJob(job);
    connect(job, &GetUsernameSuggestionJob::getUsernameSuggestionDone, this, &RestApiRequest::getUsernameSuggestionDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getUsernameSuggestion job";
    }
}

void RestApiRequest::listPermissions()
{
    ListPermissionsJob *job = new ListPermissionsJob(this);
    initializeRestApiJob(job);
    connect(job, &ListPermissionsJob::listPermissionDone, this, &RestApiRequest::listPermissionDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ListPermissionsJob job";
    }
}

void RestApiRequest::listCommands()
{
    ListCommandsJob *job = new ListCommandsJob(this);
    initializeRestApiJob(job);
    connect(job, &ListCommandsJob::listCommandsDone, this, &RestApiRequest::listCommandsDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ListPermissionsJob job";
    }
}

void RestApiRequest::fetchMyKeys()
{
    FetchMyKeysJob *job = new FetchMyKeysJob(this);
    initializeRestApiJob(job);
    connect(job, &FetchMyKeysJob::fetchMyKeysDone, this, &RestApiRequest::fetchMyKeysDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start fetchmykeys job";
    }
}

void RestApiRequest::setJoinCodeChannel(const QString &roomId, const QString &joinCode)
{
    SetJoinCodeChannelJob *job = new SetJoinCodeChannelJob(this);
    initializeRestApiJob(job);
    job->setJoinCode(joinCode);
    job->setRoomId(roomId);
    connect(job, &SetJoinCodeChannelJob::setJoinCodeDone, this, &RestApiRequest::setJoinCodeDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start setjoincode";
    }
}

void RestApiRequest::updatejitsiTimeOut(const QString &roomId)
{
    VideoConfUpdateJitsiTimeOutJob *job = new VideoConfUpdateJitsiTimeOutJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    connect(job, &VideoConfUpdateJitsiTimeOutJob::updateJitsiTimeOutDone, this, &RestApiRequest::updateJitsiTimeOutDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start VideoConfUpdateJitsiTimeOutJob";
    }
}

void RestApiRequest::channelJoin(const QString &roomId, const QString &joinCode)
{
    ChannelJoinJob *job = new ChannelJoinJob(this);
    initializeRestApiJob(job);
    job->setJoinCode(joinCode);
    job->setRoomId(roomId);
    connect(job, &ChannelJoinJob::setChannelJoinDone, this, &RestApiRequest::setChannelJoinDone);
    connect(job, &ChannelJoinJob::missingChannelPassword, this, &RestApiRequest::missingChannelPassword);
    connect(job, &ChannelJoinJob::openArchivedRoom, this, &RestApiRequest::openArchivedRoom);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start setChannelJoin";
    }
}

void RestApiRequest::channelGetAllUserMentions(const QString &roomId, int offset, int count)
{
    ChannelGetAllUserMentionsJob *job = new ChannelGetAllUserMentionsJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    QueryParameters parameters;
    QMap<QString, QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("_updatedAt"), QueryParameters::SortOrder::Descendant);
    parameters.setSorting(map);
    parameters.setCount(count);
    parameters.setOffset(offset);
    job->setQueryParameters(parameters);

    connect(job, &ChannelGetAllUserMentionsJob::channelGetAllUserMentionsDone, this, &RestApiRequest::channelGetAllUserMentionsDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start setChannelJoin";
    }
}

void RestApiRequest::channelKick(const QString &roomId, const QString &userId)
{
    ChannelKickJob *job = new ChannelKickJob(this);
    initializeRestApiJob(job);
    job->setKickUserId(userId);
    job->setRoomId(roomId);
    connect(job, &ChannelKickJob::kickUserDone, this, &RestApiRequest::channelKickUserDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channelKick";
    }
}

void RestApiRequest::groupKick(const QString &roomId, const QString &userId)
{
    GroupsKickJob *job = new GroupsKickJob(this);
    initializeRestApiJob(job);
    job->setKickUserId(userId);
    job->setRoomId(roomId);
    connect(job, &GroupsKickJob::kickUserDone, this, &RestApiRequest::groupKickUserDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channelKick";
    }
}

void RestApiRequest::groupAddModerator(const QString &roomId, const QString &userId)
{
    GroupAddModeratorJob *job = new GroupAddModeratorJob(this);
    initializeRestApiJob(job);
    job->setAddModeratorUserId(userId);
    job->setRoomId(roomId);
    connect(job, &GroupAddModeratorJob::addModeratorDone, this, &RestApiRequest::addModeratorDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start groupAddModerator";
    }
}

void RestApiRequest::groupRemoveModerator(const QString &roomId, const QString &userId)
{
    GroupRemoveModeratorJob *job = new GroupRemoveModeratorJob(this);
    initializeRestApiJob(job);
    job->setRemoveUserId(userId);
    job->setRoomId(roomId);
    connect(job, &GroupRemoveModeratorJob::removeModeratorDone, this, &RestApiRequest::removeModeratorDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start groupRemoveModerator";
    }
}

void RestApiRequest::groupAddLeader(const QString &roomId, const QString &userId)
{
    GroupAddLeaderJob *job = new GroupAddLeaderJob(this);
    initializeRestApiJob(job);
    job->setAddLeaderUserId(userId);
    job->setRoomId(roomId);
    connect(job, &GroupAddLeaderJob::addLeaderDone, this, &RestApiRequest::addLeaderDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start groupAddLeader";
    }
}

void RestApiRequest::groupRemoveLeader(const QString &roomId, const QString &userId)
{
    GroupRemoveLeaderJob *job = new GroupRemoveLeaderJob(this);
    initializeRestApiJob(job);
    job->setRemoveUserId(userId);
    job->setRoomId(roomId);
    connect(job, &GroupRemoveLeaderJob::removeLeaderDone, this, &RestApiRequest::removeLeaderDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start groupRemoveLeader";
    }
}

void RestApiRequest::groupAddOwner(const QString &roomId, const QString &userId)
{
    GroupAddOwnerJob *job = new GroupAddOwnerJob(this);
    initializeRestApiJob(job);
    job->setAddownerUserId(userId);
    job->setRoomId(roomId);
    connect(job, &GroupAddOwnerJob::addOwnerDone, this, &RestApiRequest::addOwnerDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start groupAddOwner";
    }
}

void RestApiRequest::groupRemoveOwner(const QString &roomId, const QString &userId)
{
    GroupRemoveOwnerJob *job = new GroupRemoveOwnerJob(this);
    initializeRestApiJob(job);
    job->setRemoveUserId(userId);
    job->setRoomId(roomId);
    connect(job, &GroupRemoveOwnerJob::removeOwnerDone, this, &RestApiRequest::removeOwnerDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start groupRemoveOwner";
    }
}

void RestApiRequest::channelAddModerator(const QString &roomId, const QString &userId)
{
    ChannelAddModeratorJob *job = new ChannelAddModeratorJob(this);
    initializeRestApiJob(job);
    job->setAddModeratorUserId(userId);
    job->setRoomId(roomId);
    connect(job, &ChannelAddModeratorJob::addModeratorDone, this, &RestApiRequest::addModeratorDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channelAddModerator";
    }
}

void RestApiRequest::channelRemoveModerator(const QString &roomId, const QString &userId)
{
    ChannelRemoveModeratorJob *job = new ChannelRemoveModeratorJob(this);
    initializeRestApiJob(job);
    job->setRemoveUserId(userId);
    job->setRoomId(roomId);
    connect(job, &ChannelRemoveModeratorJob::removeModeratorDone, this, &RestApiRequest::removeModeratorDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelRemoveModeratorJob";
    }
}

void RestApiRequest::channelAddLeader(const QString &roomId, const QString &userId)
{
    ChannelAddLeaderJob *job = new ChannelAddLeaderJob(this);
    initializeRestApiJob(job);
    job->setAddLeaderUserId(userId);
    job->setRoomId(roomId);
    connect(job, &ChannelAddLeaderJob::addLeaderDone, this, &RestApiRequest::addLeaderDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channelAddLeader";
    }
}

void RestApiRequest::channelRemoveLeader(const QString &roomId, const QString &userId)
{
    ChannelRemoveLeaderJob *job = new ChannelRemoveLeaderJob(this);
    initializeRestApiJob(job);
    job->setRemoveUserId(userId);
    job->setRoomId(roomId);
    connect(job, &ChannelRemoveLeaderJob::removeLeaderDone, this, &RestApiRequest::removeLeaderDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channelRemoveLeader";
    }
}

void RestApiRequest::channelAddOwner(const QString &roomId, const QString &userId)
{
    ChannelAddOwnerJob *job = new ChannelAddOwnerJob(this);
    initializeRestApiJob(job);
    job->setAddownerUserId(userId);
    job->setRoomId(roomId);
    connect(job, &ChannelAddOwnerJob::addOwnerDone, this, &RestApiRequest::addOwnerDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelAddOwnerJob";
    }
}

void RestApiRequest::channelRemoveOwner(const QString &roomId, const QString &userId)
{
    ChannelRemoveOwnerJob *job = new ChannelRemoveOwnerJob(this);
    initializeRestApiJob(job);
    job->setRemoveUserId(userId);
    job->setRoomId(roomId);
    connect(job, &ChannelRemoveOwnerJob::removeOwnerDone, this, &RestApiRequest::removeOwnerDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelRemoveOwnerJob";
    }
}

void RestApiRequest::channelDelete(const QString &roomId)
{
    ChannelDeleteJob *job = new ChannelDeleteJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    connect(job, &ChannelDeleteJob::deletechannelDone, this, &RestApiRequest::deletechannelDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelDeleteJob";
    }
}

void RestApiRequest::groupDelete(const QString &roomId)
{
    GroupsDeleteJob *job = new GroupsDeleteJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    connect(job, &GroupsDeleteJob::deleteGroupsDone, this, &RestApiRequest::deleteGroupsDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GroupsDeleteJob";
    }
}

void RestApiRequest::pinMessage(const QString &messageId, bool pinned)
{
    PinMessageJob *job = new PinMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    job->setPinMessage(pinned);
    connect(job, &PinMessageJob::pinMessageDone, this, &RestApiRequest::pinMessageDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start PinMessageJob";
    }
}

void RestApiRequest::followMessage(const QString &messageId)
{
    FollowMessageJob *job = new FollowMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    connect(job, &FollowMessageJob::followMessageDone, this, &RestApiRequest::followMessageDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start FollowMessageJob";
    }
}

void RestApiRequest::unFollowMessage(const QString &messageId)
{
    UnFollowMessageJob *job = new UnFollowMessageJob(this);
    initializeRestApiJob(job);
    job->setMessageId(messageId);
    connect(job, &UnFollowMessageJob::unFollowMessageDone, this, &RestApiRequest::unFollowMessageDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start unFollowMessageDone";
    }
}

//TODO add users arguments!
void RestApiRequest::createDiscussion(const QString &parentRoomId, const QString &discussionName, const QString &replyMessage, const QString &parentMessageId)
{
    RoomStartDiscussionJob *job = new RoomStartDiscussionJob(this);
    initializeRestApiJob(job);
    job->setParentRoomId(parentRoomId);

    job->setDiscussionName(discussionName);
    job->setParentMessageId(parentMessageId);
    job->setReplyMessage(replyMessage);
    connect(job, &RoomStartDiscussionJob::startDiscussionDone, this, &RestApiRequest::startDiscussionDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start roomStartDiscussion";
    }
}

void RestApiRequest::getDiscussions(const QString &roomId, int offset, int count)
{
    GetDiscussionsJob *job = new GetDiscussionsJob(this);
    initializeRestApiJob(job);
    QueryParameters parameters;
    parameters.setCount(count);
    parameters.setOffset(offset);
    job->setQueryParameters(parameters);
    job->setRoomId(roomId);
    connect(job, &GetDiscussionsJob::getDiscussionsDone, this, &RestApiRequest::getDiscussionsDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getDiscussions";
    }
}

void RestApiRequest::getThreadsList(const QString &roomId, int offset, int count)
{
    GetThreadsJob *job = new GetThreadsJob(this);
    initializeRestApiJob(job);
    job->setRoomId(roomId);
    QueryParameters parameters;
    parameters.setCount(count);
    parameters.setOffset(offset);
    job->setQueryParameters(parameters);
    connect(job, &GetThreadsJob::getThreadsDone, this, &RestApiRequest::getThreadsDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getThreadsList";
    }
}

void RestApiRequest::getThreadMessages(const QString &threadMessageId)
{
    GetThreadMessagesJob *job = new GetThreadMessagesJob(this);
    initializeRestApiJob(job);
    job->setThreadMessageId(threadMessageId);
    connect(job, &GetThreadMessagesJob::getThreadMessagesDone, this, &RestApiRequest::getThreadMessagesDone);
    if (!job->start()) {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getThreadMessages";
    }
}

void RestApiRequest::sendMessage(const QString &roomId, const QString &text, const QString &messageId, const QString &threadMessageId)
{
    SendMessageJob *job = new SendMessageJob(this);
    initializeRestApiJob(job);
    SendMessageJob::SendMessageArguments args;
    args.roomId = roomId;
    args.message = text;
    args.messageId = messageId;
    args.threadMessageId = threadMessageId;
    job->setSendMessageArguments(args);
    if (!job->start()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start job";
    }
}
