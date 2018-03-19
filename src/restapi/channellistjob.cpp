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

#include "channellistjob.h"
#include "ruqola_restapi_debug.h"
#include "restapimethod.h"
#include "restapirequest.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

ChannelListJob::ChannelListJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelListJob::~ChannelListJob()
{
}

bool ChannelListJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start channel list job";
        deleteLater();
        return false;
    }

    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &ChannelListJob::slotListInfo);
    reply->setProperty("method", QVariant::fromValue(RestApiRequest::RestMethod::ChannelList));
    return false;
}

void ChannelListJob::slotListInfo()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        //qCDebug(RUQOLA_RESTAPI_LOG) << "RestApiRequest::parseGetAvatar: " << data << " userId "<<userId;
        Q_EMIT channelListDone(data);
    }
    deleteLater();
}

QNetworkRequest ChannelListJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsList);
    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
    return request;
}


bool ChannelListJob::requireHttpAuthentication() const
{
    return true;
}
