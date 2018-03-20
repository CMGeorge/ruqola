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

#include "changegroupstopicjob.h"
#include "ruqola_restapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

ChangeGroupsTopicJob::ChangeGroupsTopicJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeGroupsTopicJob::~ChangeGroupsTopicJob()
{
}

bool ChangeGroupsTopicJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ChangeGroupsTopicJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ChangeGroupsTopicJob::slotChangeTopicFinished);
    return true;
}

void ChangeGroupsTopicJob::slotChangeTopicFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RUQOLA_RESTAPI_LOG) << "Change Topic success";
            Q_EMIT changeTopicDone();
        } else {
            qCWarning(RUQOLA_RESTAPI_LOG) <<" Problem when we tried to change topic" << data;
        }
    }
    qDebug() << " DONE !";
    deleteLater();
}

bool ChangeGroupsTopicJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeGroupsTopicJob::canStart() const
{
    if (mTopic.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "ChangeGroupsTopicJob: Topic is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "ChangeGroupsTopicJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start ChangeGroupsTopicJob job";
        return false;
    }
    return true;
}

QJsonDocument ChangeGroupsTopicJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("topic")] = topic();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeGroupsTopicJob::roomId() const
{
    return mRoomId;
}

void ChangeGroupsTopicJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString ChangeGroupsTopicJob::topic() const
{
    return mTopic;
}

void ChangeGroupsTopicJob::setTopic(const QString &topic)
{
    mTopic = topic;
}

QNetworkRequest ChangeGroupsTopicJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsSetTopic);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
