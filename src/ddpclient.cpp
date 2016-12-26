/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ddpclient.h"
#include <qt5/QtCore/qjsonobject.h>
#include <qt5/QtCore/qjsonobject.h>

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include <iostream>
#include "userdata.h"

void process_test(QJsonDocument doc)
{
    qDebug() << "Callback test:" << doc;
    qDebug() << "End callback";
}

void login_callback(QJsonDocument doc)
{
    qDebug() << "LOGIN:" << doc;
    UserData::instance()->setAuthToken(doc.object().value("token").toString());
    qDebug() << "End callback";
}

void DDPClient::resume_login_callback(QJsonDocument doc)
{
    qDebug() << "LOGIN:" << doc;
    UserData::instance()->setAuthToken(doc.object().value("token").toString());
    qDebug() << "End callback";
}


void empty_callback(QJsonDocument doc)
{
    Q_UNUSED(doc);
}

DDPClient::DDPClient(const QString& url2, QObject* parent)
 : QObject(parent),
  m_url(url2),
  m_uid(1),
  m_loginJob(0),
  m_loginStatus(NotConnected),
  m_connected(false),
  m_doingTokenLogin(false)
{
    m_webSocket.ignoreSslErrors();
    connect(&m_webSocket, &QWebSocket::connected, this, &DDPClient::onWSConnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &DDPClient::onTextMessageReceived);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &DDPClient::WSclosed);
    
//     QNetworkProxy proxy;
// proxy.setType(QNetworkProxy::Socks5Proxy);
// proxy.setHostName("localhost");
// proxy.setPort(9999);
// // proxy.setUser("username");
// // proxy.setPassword("password");
// QNetworkProxy::setApplicationProxy(proxy);

//     m_webSocket.setProxy(proxy);
    QString url = "chat.wikitolearn.org";
//     QString url = "demo.rocket.chat";
//     QString url = "chat.mozillaitalia.org";
    if (!url.isEmpty()) {
        m_webSocket.open(QUrl("wss://"+url+"/websocket"));
    }
    qDebug() << "Trying to connect to URL" << url;
    
}

DDPClient::~DDPClient()
{
    m_webSocket.close();
}

void DDPClient::onServerURLChange()
{
    if (UserData::instance()->serverURL() != m_url || !m_webSocket.isValid()) {
        if (m_webSocket.isValid()) {
            m_webSocket.flush();
            m_webSocket.close();
        }
        m_url = UserData::instance()->serverURL();
        m_webSocket.open(QUrl("wss://"+m_url+"/websocket"));
        connect(&m_webSocket, &QWebSocket::connected, this, &DDPClient::onWSConnected);
        qDebug() << "Reconnecting" << m_url; //<< m_webSocket.st;
    }
}

DDPClient::LoginStatus DDPClient::loginStatus() const
{
    return m_loginStatus;
}

bool DDPClient::isConnected() const
{
    return m_connected;
}

bool DDPClient::isLoggedIn() const
{
    return m_loginStatus == LoggedIn;
}

unsigned int DDPClient::method(const QString& m, const QJsonDocument& params)
{
    return method(m, params, empty_callback);
}

unsigned int DDPClient::method(const QString& method, const QJsonDocument& params, std::function<void (QJsonDocument)> callback)
{
    QString json;
    if (params.isArray()){
        json = "{\"msg\":\"method\", \"method\": \"%1\", \"id\": \"%2\", \"params\": %3}";
    } else {
        json = "{\"msg\":\"method\", \"method\": \"%1\", \"id\": \"%2\", \"params\": [%3]}";
    }
    
    json = json.arg(method).arg(m_uid).arg(QString(params.toJson(QJsonDocument::Compact)));
    
    qint64 bytes = m_webSocket.sendTextMessage(json.toUtf8()); // FIXME : text? maybe binary will be better - check if it keeps working
    if (bytes < json.length()) {
        qDebug() << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qDebug() << m_webSocket.isValid() << m_webSocket.error() << m_webSocket.requestUrl();
    } else {
        qDebug() << "Successfully sent " << json;
    }

    //callback(QJsonDocument::fromJson(json.toUtf8()));
    m_callbackHash[m_uid] = callback;

    m_uid++;
    return m_uid - 1 ;
}

void DDPClient::subscribe(const QString& collection, const QJsonDocument& params)
{
    QString json("{\"msg\":\"sub\",\"id\": \"%1\",\"name\":\"%2\", \"params\": %3}");
    json = json.arg(m_uid).arg(collection).arg(QString(params.toJson(QJsonDocument::Compact)));
    qint64 bytes = m_webSocket.sendBinaryMessage(json.toUtf8()); // FIXME : text? maybe binary will be better - check if it keeps working
    if (bytes < json.length()) {
        qDebug() << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
    } else {
        qDebug() << "Successfully sent " << json;
    }
    
    m_uid++;
    
}

void DDPClient::onTextMessageReceived(QString message)
{
    QJsonDocument response = QJsonDocument::fromJson(message.toUtf8());
    
    if (!response.isNull() && response.isObject()) {
        
        QJsonObject root = response.object();
        QString messageType = root.value("msg").toString();
        
        if (messageType == "updated") {
        
            
        } else if (messageType == "result") {
            
            qDebug() << "got a result" << root;
            unsigned id = root.value("id").toString().toInt();
            
            if (m_callbackHash.contains(id)) {
                std::function<void (QJsonDocument)> callback = m_callbackHash.take(id);
                callback( QJsonDocument(root.value("result").toObject()) );
            }
            emit result(id, QJsonDocument(root.value("result").toObject()));
            
            if (id == m_loginJob) {
                if (root.value("error").toObject().value("error").toInt() == 403) {
                    qDebug() << "Wrong password or token expired";
                    
                    // Kill wrong credentials, so we don't try to use them again
                    if (!UserData::instance()->authToken().isEmpty()) {
                        UserData::instance()->setAuthToken(QString());
                    } else if (!UserData::instance()->password().isEmpty()) {
                        UserData::instance()->setPassword(QString());
                    }
                    setLoginStatus(DDPClient::LoginFailed);
                    
                } else {
                    UserData::instance()->setAuthToken(root.value("result").toObject().value("token").toString());

                    setLoginStatus(DDPClient::LoggedIn);
                }
//                 emit loggedInChanged();
            }
            
        } else if (messageType == "connected") {
            qDebug() << "Connected";
            m_connected = true;
//             emit connected();
            emit connectedChanged();
            if (!UserData::instance()->authToken().isEmpty()) {
                setLoginStatus(DDPClient::LoggingIn);
                login();// Try to resume auth token login
            }
            
            
        } else if (messageType == "error") {
            qDebug() << "ERROR!!" << message;
        } else if (messageType == "ping") {
            qDebug() << "Ping - Pong";
            m_webSocket.sendBinaryMessage("{\"msg\":\"pong\"}");
        } else if (messageType == "added"){
            emit added(root);
        } else if (messageType == "changed") {
            emit changed(root);
        } else {
            qDebug() << "received something unhandled:" << message;
        }
    }
    
}

void DDPClient::setLoginStatus(DDPClient::LoginStatus l)
{
    m_loginStatus = l;
    emit loginStatusChanged();
}

void DDPClient::login()
{
    if (!UserData::instance()->authToken().isEmpty()) {
        m_doingTokenLogin = true;
        QString json = "{\"resume\":\"%1\"}";
        json = json.arg(UserData::instance()->authToken());
        m_loginJob = method("login", QJsonDocument::fromJson(json.toUtf8()));
    } else if (!UserData::instance()->password().isEmpty()) {
        QString json = "{\"password\":\"%1\", \"user\": {\"username\":\"%2\"}}";
        json = json.arg(UserData::instance()->password()).arg(UserData::instance()->userName());
        m_loginJob = method("login", QJsonDocument::fromJson(json.toUtf8()));
    } else {
        setLoginStatus(LoginFailed);
    }
}

void DDPClient::onWSConnected()
{
    qDebug() << "Websocket connected at URL" << m_url;
    
    QString json("{\"msg\":\"connect\", \"version\": \"1\", \"support\": [\"1\"]}");
    
    qint64 bytes = m_webSocket.sendTextMessage(json.toUtf8());
    if (bytes < json.length()) {
        qDebug() << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
    } else {
        qDebug() << "Successfully sent " << json;
    }
}

void DDPClient::WSclosed()
{
    qDebug() << "WebSocket CLOSED" << m_webSocket.closeReason() << m_webSocket.error() << m_webSocket.closeCode();
    setLoginStatus(NotConnected);
//     m_connected = false;
}


