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

#include "logoutjob.h"
#include "restapi_debug.h"
#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

LogoutJob::LogoutJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

LogoutJob::~LogoutJob()
{
}

bool LogoutJob::start()
{
    if (!canStart()) {
        qCWarning(RESTAPI_LOG) << "Impossible to start LogoutJob job";
        deleteLater();
        return false;
    }

    QNetworkReply *reply = mNetworkAccessManager->get(request());
    addLoggerInfo("LogoutJob::start");
    connect(reply, &QNetworkReply::finished, this, &LogoutJob::slotLogout);
    return false;
}

void LogoutJob::slotLogout()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        addLoggerInfo("LogoutJob finished: " + data);
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("status")].toString() == QStringLiteral("success")) {
            qCDebug(RESTAPI_LOG) << " Logout";
            Q_EMIT logoutDone();
        } else {
            qCWarning(RESTAPI_LOG) <<" Problem when we try to logout";
        }

        qCDebug(RESTAPI_LOG) << " void RestApiRequest::parseLogout(const QByteArray &data)" << data;
    }
    deleteLater();
}

QNetworkRequest LogoutJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Logout);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}

bool LogoutJob::requireHttpAuthentication() const
{
    return true;
}
