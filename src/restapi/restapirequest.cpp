/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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
#include "ruqola_restapi_debug.h"

#include "serverinfojob.h"
#include "uploadfilejob.h"
#include "settings/privateinfojob.h"
#include "channellistjob.h"
#include "downloadfilejob.h"

#include "users/getavatarjob.h"

#include "misc/owninfojob.h"

#include "authentication/logoutjob.h"
#include "authentication/loginjob.h"

#include "chat/starmessagejob.h"
#include "chat/postmessagejob.h"
#include "chat/deletemessagejob.h"
#include "chat/updatemessagejob.h"
#include "chat/reactonmessagejob.h"

#include "channels/changechanneltopicjob.h"
#include "channels/changechannelannouncementjob.h"
#include "channels/createchanneljob.h"
#include "channels/leavechanneljob.h"
#include "channels/channelclosejob.h"
#include "channels/channelhistoryjob.h"
#include "channels/changechanneldescriptionjob.h"
#include "channels/archivechanneljob.h"
#include "channels/channelfilesjob.h"

#include "groups/changegroupsannouncementjob.h"
#include "groups/changegroupstopicjob.h"
#include "groups/creategroupsjob.h"
#include "groups/leavegroupsjob.h"
#include "groups/changegroupsdescriptionjob.h"
#include "groups/archivegroupsjob.h"

#include "directmessage/createdmjob.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <ruqolalogger.h>

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

void RestApiRequest::setRuqolaLogger(RuqolaLogger *logger)
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
        qCWarning(RUQOLA_RESTAPI_LOG) << "We can not initialize cookies as server url is empty.";
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
        qCWarning(RUQOLA_RESTAPI_LOG) << " Error reply - "<<reply->errorString();
    }
}

void RestApiRequest::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)
{
    qCDebug(RUQOLA_RESTAPI_LOG) << " void RestApiRequest::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)" << error.count();
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
    initializeRestApiJob(job, false);
    job->setPassword(mPassword);
    job->setUserName(mUserName);
    job->start();
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

void RestApiRequest::initializeRestApiJob(RestApiAbstractJob *job, bool needAuthentication)
{
    job->setNetworkAccessManager(mNetworkAccessManager);
    job->setRuqolaLogger(mRuqolaLogger);
    job->setRestApiMethod(mRestApiMethod);
    if (needAuthentication) {
        job->setAuthToken(mAuthToken);
        job->setUserId(mUserId);
    }
}

void RestApiRequest::logout()
{
    LogoutJob *job = new LogoutJob(this);
    connect(job, &LogoutJob::logoutDone, this, &RestApiRequest::slotLogout);
    initializeRestApiJob(job, true);
    job->start();
}

void RestApiRequest::channelList()
{
    ChannelListJob *job = new ChannelListJob(this);
    //TODO connect(job, &ChannelListJob::channelListDone, this, &RestApiRequest::slotLogout);
    initializeRestApiJob(job, true);
    job->start();
}

void RestApiRequest::getAvatar(const QString &userId)
{
    GetAvatarJob *job = new GetAvatarJob(this);
    connect(job, &GetAvatarJob::avatar, this, &RestApiRequest::avatar);
    initializeRestApiJob(job, false);
    job->setAvatarUserId(userId);
    job->start();
}

void RestApiRequest::getPrivateSettings()
{
    PrivateInfoJob *job = new PrivateInfoJob(this);
    connect(job, &PrivateInfoJob::privateInfoDone, this, &RestApiRequest::privateInfoDone);
    initializeRestApiJob(job, true);
    job->start();
}

void RestApiRequest::getOwnInfo()
{
    OwnInfoJob *job = new OwnInfoJob(this);
    connect(job, &OwnInfoJob::ownInfoDone, this, &RestApiRequest::getOwnInfoDone);
    initializeRestApiJob(job, true);
    job->start();
}

void RestApiRequest::starMessage(const QString &messageId, bool starred)
{
    StarMessageJob *job = new StarMessageJob(this);
    initializeRestApiJob(job, true);
    job->setMessageId(messageId);
    job->setStarMessage(starred);
    job->start();
}

void RestApiRequest::downloadFile(const QUrl &url, const QString &mimeType, bool storeInCache, const QUrl &localFileUrl)
{
    DownloadFileJob *job = new DownloadFileJob(this);
    //Rename signal
    connect(job, &DownloadFileJob::downloadFileDone, this, &RestApiRequest::getDataDone);
    job->setUrl(url);
    job->setMimeType(mimeType);
    job->setLocalFileUrl(localFileUrl);
    job->setStoreInCache(storeInCache);
    initializeRestApiJob(job, true);
    job->start();
}

void RestApiRequest::serverInfo()
{
    ServerInfoJob *job = new ServerInfoJob(this);
    initializeRestApiJob(job, false);
    connect(job, &ServerInfoJob::serverInfoDone, this, &RestApiRequest::getServerInfoDone);
    job->start();
}

void RestApiRequest::uploadFile(const QString &roomId, const QString &description, const QString &text, const QUrl &filename)
{
    UploadFileJob *job = new UploadFileJob(this);
    initializeRestApiJob(job, true);
    job->setDescription(description);
    job->setMessageText(text);
    job->setFilenameUrl(filename);
    job->setRoomId(roomId);
    job->start();
}

void RestApiRequest::changeChannelTopic(const QString &roomId, const QString &topic)
{
    ChangeChannelTopicJob *job = new ChangeChannelTopicJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    job->setTopic(topic);
    job->start();
}

void RestApiRequest::changeGroupsTopic(const QString &roomId, const QString &topic)
{
    ChangeGroupsTopicJob *job = new ChangeGroupsTopicJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    job->setTopic(topic);
    job->start();
}

void RestApiRequest::changeChannelAnnouncement(const QString &roomId, const QString &announcement)
{
    ChangeGroupsAnnouncementJob *job = new ChangeGroupsAnnouncementJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    job->setAnnouncement(announcement);
    job->start();
}

void RestApiRequest::changeGroupsAnnouncement(const QString &roomId, const QString &announcement)
{
    ChangeGroupsAnnouncementJob *job = new ChangeGroupsAnnouncementJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    job->setAnnouncement(announcement);
    job->start();
}

void RestApiRequest::changeChannelDescription(const QString &roomId, const QString &description)
{
    ChangeChannelDescriptionJob *job = new ChangeChannelDescriptionJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    job->setDescription(description);
    job->start();
}

void RestApiRequest::changeGroupsDescription(const QString &roomId, const QString &description)
{
    ChangeGroupsDescriptionJob *job = new ChangeGroupsDescriptionJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    job->setDescription(description);
    job->start();
}

void RestApiRequest::postMessage(const QString &roomId, const QString &text)
{
    PostMessageJob *job = new PostMessageJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    job->setText(text);
    job->start();
}

void RestApiRequest::deleteMessage(const QString &roomId, const QString &messageId)
{
    DeleteMessageJob *job = new DeleteMessageJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    job->setMessageId(messageId);
    job->start();
}

void RestApiRequest::createChannels(const QString &channelName, bool readOnly, const QStringList &members)
{
    CreateChannelJob *job = new CreateChannelJob(this);
    initializeRestApiJob(job, true);
    job->setChannelName(channelName);
    job->setReadOnly(readOnly);
    job->setMembers(members);
    job->start();
}

void RestApiRequest::createGroups(const QString &channelName, bool readOnly, const QStringList &members)
{
    CreateGroupsJob *job = new CreateGroupsJob(this);
    initializeRestApiJob(job, true);
    job->setChannelName(channelName);
    job->setReadOnly(readOnly);
    job->setMembers(members);
    job->start();
}

void RestApiRequest::leaveChannel(const QString &roomId)
{
    LeaveChannelJob *job = new LeaveChannelJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    job->start();
}

void RestApiRequest::leaveGroups(const QString &roomId)
{
    LeaveGroupsJob *job = new LeaveGroupsJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    job->start();
}

void RestApiRequest::archiveChannel(const QString &roomId)
{
    ArchiveChannelJob *job = new ArchiveChannelJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    job->start();
}

void RestApiRequest::archiveGroups(const QString &roomId)
{
    ArchiveGroupsJob *job = new ArchiveGroupsJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    job->start();
}

void RestApiRequest::updateMessage(const QString &roomId, const QString &messageId, const QString &text)
{
    UpdateMessageJob *job = new UpdateMessageJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    job->setMessageId(messageId);
    job->setUpdatedText(text);
    job->start();
}

void RestApiRequest::reactOnMessage(const QString &messageId, const QString &emoji)
{
    ReactOnMessageJob *job = new ReactOnMessageJob(this);
    initializeRestApiJob(job, true);
    job->setMessageId(messageId);
    job->setEmoji(emoji);
    job->start();
}

void RestApiRequest::closeChannel(const QString &roomId, const QString &type)
{
    ChannelCloseJob *job = new ChannelCloseJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    if (type == QLatin1String("d")) {
        job->setChannelType(ChannelCloseJob::Direct);
    } else if (type == QLatin1String("p")) {
        job->setChannelType(ChannelCloseJob::Groups);
    } else if (type == QLatin1String("c")) {
        job->setChannelType(ChannelCloseJob::Channel);
    }
    job->start();
}

void RestApiRequest::historyChannel(const QString &roomId, const QString &type)
{
    ChannelHistoryJob *job = new ChannelHistoryJob(this);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    if (type == QLatin1String("d")) {
        job->setChannelType(ChannelHistoryJob::Direct);
    } else if (type == QLatin1String("p")) {
        job->setChannelType(ChannelHistoryJob::Groups);
    } else if (type == QLatin1String("c")) {
        job->setChannelType(ChannelHistoryJob::Channel);
    }
    job->start();
}

void RestApiRequest::createDirectMessage(const QString &userName)
{
    CreateDmJob *job = new CreateDmJob(this);
    initializeRestApiJob(job, true);
    job->setUserName(userName);
    job->start();
}

void RestApiRequest::filesInRoom(const QString &roomId, const QString &type)
{
    ChannelFilesJob *job = new ChannelFilesJob(this);
    connect(job, &ChannelFilesJob::channelFilesDone, this, &RestApiRequest::channelFilesDone);
    initializeRestApiJob(job, true);
    job->setRoomId(roomId);
    if (type == QLatin1String("d")) {
        job->setChannelType(ChannelFilesJob::Direct);
    } else if (type == QLatin1String("p")) {
        job->setChannelType(ChannelFilesJob::Groups);
    } else if (type == QLatin1String("c")) {
        job->setChannelType(ChannelFilesJob::Channel);
    }
    job->start();
}
