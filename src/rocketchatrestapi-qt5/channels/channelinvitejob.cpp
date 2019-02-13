/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "channelinvitejob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelInviteJob::ChannelInviteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelInviteJob::~ChannelInviteJob()
{
}

bool ChannelInviteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ChannelInviteJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ChannelInviteJob::slotInvitationFinished);
    return true;
}

void ChannelInviteJob::slotInvitationFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChannelInviteJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT inviteDone();
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("ChannelInviteJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
    }
    deleteLater();
}

QString ChannelInviteJob::inviteUserName() const
{
    return mInviteUserName;
}

void ChannelInviteJob::setInviteUserName(const QString &userName)
{
    mInviteUserName = userName;
}

QString ChannelInviteJob::inviteUserId() const
{
    return mInviteUserId;
}

void ChannelInviteJob::setInviteUserId(const QString &userId)
{
    mInviteUserId = userId;
}

bool ChannelInviteJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelInviteJob::canStart() const
{
    if (mInviteUserId.isEmpty() && mInviteUserName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelInviteJob: inviteUserId is empty or inviteUserName is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelInviteJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelInviteJob job";
        return false;
    }
    return true;
}

QJsonDocument ChannelInviteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    if (!inviteUserId().isEmpty()) {
        jsonObj[QLatin1String("userId")] = inviteUserId();
    } else if (!inviteUserName().isEmpty()) {
        jsonObj[QLatin1String("userName")] = inviteUserName();
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChannelInviteJob::roomId() const
{
    return mRoomId;
}

void ChannelInviteJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChannelInviteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsInvite);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
