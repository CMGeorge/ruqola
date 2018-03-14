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

#include "restapirequest.h"
#include "ruqola_restapi_debug.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QJsonArray>
#include <QHttpMultiPart>
#include <QFile>

RestApiRequest::RestApiRequest(QObject *parent)
    : QObject(parent)
{
    mCookieJar = new QNetworkCookieJar;
    mNetworkAccessManager = new QNetworkAccessManager(this);
    mNetworkAccessManager->setCookieJar(mCookieJar);
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &RestApiRequest::slotResult);
    connect(mNetworkAccessManager, &QNetworkAccessManager::sslErrors, this, &RestApiRequest::slotSslErrors);
}

RestApiRequest::~RestApiRequest()
{
}

void RestApiRequest::initializeCookies()
{
    if (!mServerUrl.isEmpty()) {
        QString host;
        QStringList lsthost = mServerUrl.split(QStringLiteral("//"));
        if (lsthost.count() < 2) {
            host = mServerUrl;
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

void RestApiRequest::parseLogin(const QByteArray &data)
{
    const QJsonDocument replyJson = QJsonDocument::fromJson(data);
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("status")].toString() == QStringLiteral("success") && replyObject.contains(QLatin1String("data"))) {
        QJsonObject data = replyObject[QStringLiteral("data")].toObject();

        if (data.contains(QLatin1String("authToken")) && data.contains(QLatin1String("userId"))) {
            mAuthToken = data[QStringLiteral("authToken")].toString();
            mUserId = data[QStringLiteral("userId")].toString();
        }
    } else {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Error during login" << data;
    }
}

void RestApiRequest::parseOwnInfo(const QByteArray &data)
{
    qCDebug(RUQOLA_RESTAPI_LOG) << "RestApiRequest::parseOwnInfo: " << data;
    Q_EMIT getOwnInfoDone(data);
}

void RestApiRequest::parseLogout(const QByteArray &data)
{
    const QJsonDocument replyJson = QJsonDocument::fromJson(data);
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("status")].toString() == QStringLiteral("success")) {
        qCDebug(RUQOLA_RESTAPI_LOG) << " Logout";
        mUserId.clear();
        mAuthToken.clear();
    } else {
        qCWarning(RUQOLA_RESTAPI_LOG) <<" Problem when we try to logout";
    }

    qCDebug(RUQOLA_RESTAPI_LOG) << " void RestApiRequest::parseLogout(const QByteArray &data)" << data;
}

void RestApiRequest::parseChannelList(const QByteArray &data)
{
    //qDebug() << " void RestApiRequest::parseChannelList(const QByteArray &data)" << data;
    const QJsonDocument replyJson = QJsonDocument::fromJson(data);
    const QJsonObject replyObject = replyJson.object();
    //qDebug() << " replyObject"<<replyObject;
    const QVariantList lst = replyObject.value(QLatin1String("channels")).toArray().toVariantList();
    for (const QVariant &item : lst) {
        //qDebug() << " item " << item;
        //qDebug() << " map  ? : "<<item.toMap();
    }
    //qDebug() << "*******************************" << ;
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

void RestApiRequest::parseGetAvatar(const QByteArray &data, const QString &userId)
{
    //qCDebug(RUQOLA_RESTAPI_LOG) << "RestApiRequest::parseGetAvatar: " << data << " userId "<<userId;
    QString str = QString::fromUtf8(data);
    str.remove(QLatin1Char('"'));
    Q_EMIT avatar(userId, str);
}

void RestApiRequest::parseGet(const QByteArray &data, const QUrl &url, bool storeInCache, const QUrl &localFileUrl)
{
    qCDebug(RUQOLA_RESTAPI_LOG) << "RestApiRequest::parseGet: url " << url;
    Q_EMIT getDataDone(data, url, storeInCache, localFileUrl);
}

void RestApiRequest::parsePost(const QByteArray &data)
{
    qCDebug(RUQOLA_RESTAPI_LOG) << "RestApiRequest::parsePost: " << data;
}

void RestApiRequest::parseUploadFile(const QByteArray &data)
{
    qDebug() << "RestApiRequest::parseUploadFile result " << data;
}

void RestApiRequest::parsePrivateInfo(const QByteArray &data)
{
    const QJsonDocument replyJson = QJsonDocument::fromJson(data);
    qDebug() << " replyJson" << replyJson;
}

void RestApiRequest::parseServerInfo(const QByteArray &data)
{
    //qCDebug(RUQOLA_RESTAPI_LOG) << "RestApiRequest::parseServerInfo: " << data;
    const QJsonDocument replyJson = QJsonDocument::fromJson(data);
    const QJsonObject replyObject = replyJson.object();
    const QJsonObject version = replyObject.value(QStringLiteral("info")).toObject();
    const QString versionStr = version.value(QStringLiteral("version")).toString();
    if (versionStr.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Problem during parsing server version";
    }

    Q_EMIT getServerInfoDone(versionStr);
}

void RestApiRequest::slotResult(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        const QByteArray data = reply->readAll();
        const RestMethod restMethod = reply->property("method").value<RestMethod>();
        switch (restMethod) {
        case Login:
            parseLogin(data);
            break;
        case Logout:
            parseLogout(data);
            break;
        case ChannelList:
            parseChannelList(data);
            break;
        case PrivateInfo:
            parsePrivateInfo(data);
            break;
        case GetAvatar:
            parseGetAvatar(data, reply->property("userId").toString());
            break;
        case ServerInfo:
            parseServerInfo(data);
            break;
        case Me:
            parseOwnInfo(data);
            break;
        case UploadFile:
            parseUploadFile(data);
            break;
        case Get:
        {
            const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (status == 200) {
                bool storeInCache = true;
                QUrl localFile;
                QVariant var = reply->property("storeInCache");
                if (var.isValid()) {
                    storeInCache = var.toBool();
                }
                var = reply->property("localFile");
                if (var.isValid()) {
                    localFile = var.toUrl();
                }
                parseGet(data, reply->url(), storeInCache, localFile);
            } else {
                qCWarning(RUQOLA_RESTAPI_LOG) << "Unable to download " << reply->url();
            }
            break;
        }
        case Post:
            parsePost(data);
            break;
        case Unknown:
            qCWarning(RUQOLA_RESTAPI_LOG) << " Unknown restapi method" << data;
            break;
        }
    } else {
        const RestMethod restMethod = reply->property("method").value<RestMethod>();
        qCWarning(RUQOLA_RESTAPI_LOG) << " Error reply - "<<reply->errorString() << " restMethod "<<restMethod;
    }
    reply->deleteLater();
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
    return mServerUrl;
}

void RestApiRequest::setServerUrl(const QString &serverUrl)
{
    mServerUrl = serverUrl;
}

QString RestApiRequest::authToken() const
{
    return mAuthToken;
}

QString RestApiRequest::userId() const
{
    return mUserId;
}

QUrl RestApiRequest::generateUrl(RestApiUtil::RestApiUrlType type, const QString &urlExtension)
{
    QString urlStr = RestApiUtil::adaptUrl(mServerUrl) + RestApiUtil::apiUri() + RestApiUtil::restUrl(type);
    if (!urlExtension.isEmpty()) {
        urlStr += QLatin1Char('/') + urlExtension;
    }
    return QUrl(urlStr);
}

void RestApiRequest::login()
{
    if (!mUserName.isEmpty() && !mPassword.isEmpty() && !mServerUrl.isEmpty()) {
        QUrl url = generateUrl(RestApiUtil::RestApiUrlType::Login);
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

        QVariantMap loginMap;
        loginMap.insert(QStringLiteral("user"), mUserName);
        loginMap.insert(QStringLiteral("password"), mPassword);
        const QJsonDocument postData = QJsonDocument::fromVariant(loginMap);
        const QByteArray baPostData = postData.toJson(QJsonDocument::Compact);
        QNetworkReply *reply = mNetworkAccessManager->post(request, baPostData);
        reply->setProperty("method", QVariant::fromValue(RestMethod::Login));
    } else {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Password or user or url is empty";
    }
}

void RestApiRequest::logout()
{
    const QUrl url = generateUrl(RestApiUtil::RestApiUrlType::Logout);
    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
    QNetworkReply *reply = mNetworkAccessManager->get(request);
    reply->setProperty("method", QVariant::fromValue(RestMethod::Logout));
}

void RestApiRequest::channelList()
{
    if (mUserId.isEmpty() || mAuthToken.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "RestApiRequest::channelList problem with mUserId or mAuthToken";
    } else {
        const QUrl url = generateUrl(RestApiUtil::RestApiUrlType::ChannelsList);
        QNetworkRequest request(url);
        request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
        request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
        QNetworkReply *reply = mNetworkAccessManager->get(request);
        reply->setProperty("method", QVariant::fromValue(RestMethod::ChannelList));
    }
}

void RestApiRequest::getAvatar(const QString &userId)
{
    if (mUserId.isEmpty() || mAuthToken.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "RestApiRequest::getAvatar problem with mUserId or mAuthToken";
    } else {
        QUrl url = generateUrl(RestApiUtil::RestApiUrlType::UsersGetAvatar);
        QUrlQuery queryUrl;
        queryUrl.addQueryItem(QStringLiteral("userId"), userId);
        url.setQuery(queryUrl);
        QNetworkRequest request(url);
        QNetworkReply *reply = mNetworkAccessManager->get(request);
        reply->setProperty("method", QVariant::fromValue(RestMethod::GetAvatar));
        reply->setProperty("userId", userId);
    }
}

void RestApiRequest::getPrivateSettings()
{
    if (mUserId.isEmpty() || mAuthToken.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "RestApiRequest::getPrivateSettings problem with mUserId or mAuthToken";
    } else {
        QUrl url = generateUrl(RestApiUtil::RestApiUrlType::Settings);
        QNetworkRequest request(url);
        request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
        request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());

        QNetworkReply *reply = mNetworkAccessManager->get(request);
        reply->setProperty("method", QVariant::fromValue(RestMethod::PrivateInfo));
    }
}

void RestApiRequest::getOwnInfo()
{
    if (mUserId.isEmpty() || mAuthToken.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "RestApiRequest::getOwnInfo problem with mUserId or mAuthToken";
    } else {
        QUrl url = generateUrl(RestApiUtil::RestApiUrlType::Me);
        QNetworkRequest request(url);
        request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
        request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());

        QNetworkReply *reply = mNetworkAccessManager->get(request);
        reply->setProperty("method", QVariant::fromValue(RestMethod::Me));
    }
}

void RestApiRequest::post(const QUrl &url, const QByteArray &data, const QString &mimeType)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, mimeType);
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    QNetworkReply *reply = mNetworkAccessManager->post(request, data);
    reply->setProperty("method", QVariant::fromValue(RestMethod::Post));
}

QNetworkReply *RestApiRequest::get(const QUrl &url, const QString &mimeType)
{
    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
    request.setHeader(QNetworkRequest::ContentTypeHeader, mimeType);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    QNetworkReply *reply = mNetworkAccessManager->get(request);
    reply->setProperty("method", QVariant::fromValue(RestMethod::Get));
    return reply;
}

void RestApiRequest::serverInfo()
{
    QUrl url = generateUrl(RestApiUtil::RestApiUrlType::ServerInfo);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    QNetworkReply *reply = mNetworkAccessManager->get(request);
    reply->setProperty("method", QVariant::fromValue(RestMethod::ServerInfo));
}

void RestApiRequest::uploadFile(const QString &roomId, const QString &description, const QString &text, const QString &filename)
{
    qDebug() << " void RestApiRequest::uploadFile(const QString &roomId, const QString &description, const QString &text, const QString &filename)"<< filename << description;
    QFile *file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly)) {
        qCWarning(RUQOLA_RESTAPI_LOG) << " Impossible to open filename " << filename;
        delete file;
        return;
    }

    QUrl url = generateUrl(RestApiUtil::RestApiUrlType::RoomsUpload, roomId);
    QNetworkRequest request(url);
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QStringLiteral("text/plain")));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QStringLiteral("form-data; name=\"file\"; filename=\"example.txt\"")));

    filePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart


    QHttpPart msgPart;
    msgPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"msg\"")));
    msgPart.setBody(text.toUtf8());
    multiPart->append(msgPart);

    QHttpPart descriptionPart;
    descriptionPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"description\"")));
    descriptionPart.setBody(text.toUtf8());
    multiPart->append(descriptionPart);
    QNetworkReply *reply = mNetworkAccessManager->post(request, multiPart);
    reply->setProperty("method", QVariant::fromValue(RestMethod::UploadFile));
    multiPart->setParent(reply); // delete the multiPart with the reply
}
