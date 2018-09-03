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

#include "changegroupsdescriptionjob.h"

#include "restapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChangeGroupsDescriptionJob::ChangeGroupsDescriptionJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeGroupsDescriptionJob::~ChangeGroupsDescriptionJob()
{
}

bool ChangeGroupsDescriptionJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ChangeGroupsDescriptionJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ChangeGroupsDescriptionJob::slotChangeDescriptionFinished);
    return true;
}

void ChangeGroupsDescriptionJob::slotChangeDescriptionFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RESTAPI_LOG) << "Change description success: " << data;
            Q_EMIT changeDescriptionDone();
        } else {
            qCWarning(RESTAPI_LOG) <<" Problem when we tried to change description: " << data;
        }
    }
    deleteLater();
}

QString ChangeGroupsDescriptionJob::description() const
{
    return mDescription;
}

void ChangeGroupsDescriptionJob::setDescription(const QString &description)
{
    mDescription = description;
}

bool ChangeGroupsDescriptionJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeGroupsDescriptionJob::canStart() const
{
    if (mDescription.isEmpty()) {
        qCWarning(RESTAPI_LOG) << "ChangeGroupsDescriptionJob: description is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(RESTAPI_LOG) << "ChangeGroupsDescriptionJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RESTAPI_LOG) << "Impossible to start ChangeGroupsDescriptionJob job";
        return false;
    }
    return true;
}

QJsonDocument ChangeGroupsDescriptionJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("description")] = description();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeGroupsDescriptionJob::roomId() const
{
    return mRoomId;
}

void ChangeGroupsDescriptionJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChangeGroupsDescriptionJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsSetDescription);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
