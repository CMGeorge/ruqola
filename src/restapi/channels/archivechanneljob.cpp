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

#include "archivechanneljob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ArchiveChannelJob::ArchiveChannelJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ArchiveChannelJob::~ArchiveChannelJob()
{
}

bool ArchiveChannelJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ArchiveChannelJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ArchiveChannelJob::slotArchiveChannelFinished);
    return true;
}

void ArchiveChannelJob::slotArchiveChannelFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(ROCKETCHATQTRESTAPI_LOG) << "archive or unarchive channel success: " << data;
            Q_EMIT archiveChannelDone();
        } else {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) <<" Problem when we tried to archive or unarchive a channel" << data;
        }
    }
    deleteLater();
}

bool ArchiveChannelJob::archive() const
{
    return mArchive;
}

void ArchiveChannelJob::setArchive(bool archive)
{
    mArchive = archive;
}

bool ArchiveChannelJob::requireHttpAuthentication() const
{
    return true;
}

bool ArchiveChannelJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ArchiveChannelJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ArchiveChannelJob job";
        return false;
    }
    return true;
}

QJsonDocument ArchiveChannelJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ArchiveChannelJob::roomId() const
{
    return mRoomId;
}

void ArchiveChannelJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ArchiveChannelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(mArchive ? RestApiUtil::RestApiUrlType::ChannelsArchive : RestApiUtil::RestApiUrlType::ChannelsUnarchive);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
