/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "ruqola_restapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

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
    connect(reply, &QNetworkReply::finished, this, &ChannelInviteJob::slotChangeAnnouncementFinished);
    return true;
}

void ChannelInviteJob::slotChangeAnnouncementFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RUQOLA_RESTAPI_LOG) << "Change announcement success: " << data;
            Q_EMIT changeAnnouncementDone();
        } else {
            qCWarning(RUQOLA_RESTAPI_LOG) <<" Problem when we tried to change announcement: " << data;
        }
    }
    deleteLater();
}

QString ChannelInviteJob::userId() const
{
    return mUserId;
}

void ChannelInviteJob::setUserId(const QString &userId)
{
    mUserId = userId;
}

bool ChannelInviteJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelInviteJob::canStart() const
{
    if (mUserId.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "ChannelInviteJob: userId is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "ChannelInviteJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start ChannelInviteJob job";
        return false;
    }
    return true;
}

QJsonDocument ChannelInviteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("userId")] = userId();

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
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
