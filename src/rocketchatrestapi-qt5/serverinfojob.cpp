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

#include "serverinfojob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ServerInfoJob::ServerInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ServerInfoJob::~ServerInfoJob()
{
}

bool ServerInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start server info job";
        deleteLater();
        return false;
    }

    QNetworkReply *reply = mNetworkAccessManager->get(request());
    addLoggerInfo("ServerInfoJob::start");
    connect(reply, &QNetworkReply::finished, this, &ServerInfoJob::slotServerInfoFinished);

    return true;
}

QNetworkRequest ServerInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ServerInfo);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    return request;
}

bool ServerInfoJob::requireHttpAuthentication() const
{
    return false;
}

void ServerInfoJob::slotServerInfoFinished()
{
    QString versionStr;
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        addLoggerInfo(QByteArrayLiteral("ServerInfoJob: finished: ") + data);
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "ServerInfoJob::parseServerInfo: " << data;
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        const QJsonObject version = replyObject.value(QStringLiteral("info")).toObject();
        versionStr = version.value(QStringLiteral("version")).toString();
        if (versionStr.isEmpty()) {
            addLoggerWarning(QByteArrayLiteral("ServerInfoJob::slotServerInfoFinished: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
    }
    Q_EMIT serverInfoDone(versionStr);
    deleteLater();
}
