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

#include "setavatarjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

SetAvatarJob::SetAvatarJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SetAvatarJob::~SetAvatarJob()
{
}

bool SetAvatarJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("SetAvatarJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &SetAvatarJob::slotSetAvatar);
    addLoggerInfo(QByteArrayLiteral("SetAvatarJob: start"));
    return true;
}

void SetAvatarJob::slotSetAvatar()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
    addLoggerInfo(QByteArrayLiteral("SetAvatarJob: finished: ") + data);
    Q_EMIT setAvatarDone();
    }
    deleteLater();
}

QString SetAvatarJob::avatarUrl() const
{
    return mAvatarUrl;
}

void SetAvatarJob::setAvatarUrl(const QString &avatarUrl)
{
    mAvatarUrl = avatarUrl;
}

QString SetAvatarJob::avatarUserId() const
{
    return mAvatarUserId;
}

void SetAvatarJob::setAvatarUserId(const QString &avatarUserId)
{
    mAvatarUserId = avatarUserId;
}

bool SetAvatarJob::requireHttpAuthentication() const
{
    return true;
}

bool SetAvatarJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
    qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start SetAvatarJob";
        return false;
    }
    if (mAvatarUrl.isEmpty()) {
    qCWarning(RUQOLA_RESTAPI_LOG) << "SetAvatarJob: mAvatarUrl is empty";
        return false;
    }
    return true;
}

QNetworkRequest SetAvatarJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersSetAvatar);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}

QJsonDocument SetAvatarJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("avatarUrl")] = mAvatarUrl;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
