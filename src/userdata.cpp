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

#include "userdata.h"
#include "roommodel.h"
#include "ddpclient.h"

UserData *UserData::m_self = 0;

QString UserData::authToken() const
{
    return m_authToken;
}

QString UserData::userName() const
{
    return m_userName;
}

QString UserData::password() const
{
    return m_password;
}

void UserData::setAuthToken(const QString& token)
{
    qDebug() << "Setting token to" << token;
    QSettings s;
    m_authToken = token;
    s.setValue("authToken", token);
}

void UserData::setPassword(const QString& password)
{
    m_password = password;
}

void UserData::setUserName(const QString& username)
{
    m_userName = username;
    QSettings s;
    s.setValue("username", username);
    emit userNameChanged();
}

RoomModel * UserData::roomModel() const
{
    return m_roomModel;
}

DDPClient * UserData::ddp() const
{
    return m_ddp;
}

void UserData::sendMessage(const QString &roomID, const QString &message)
{
    QString json = "{\"rid\": \"%1\", \"msg\": \"%2\"}";
    json = json.arg(roomID, message);
    ddp()->method("sendMessage", QJsonDocument::fromJson(json.toUtf8()));
}

MessageModel * UserData::getModelForRoom(const QString& roomID)
{
    if (m_messageModels.contains(roomID)) {
        return m_messageModels.value(roomID);
    } else {
        qDebug() << "Creating a new model";
        m_messageModels[roomID] = new MessageModel(this);
        return m_messageModels[roomID];        
    }
}

QString UserData::serverURL() const
{
    return m_serverURL;
}

void UserData::setServerURL(const QString& serverURL)
{
    QSettings s;
    s.setValue("serverURL", serverURL);
    m_serverURL = serverURL;
    emit serverURLChanged();
}

bool UserData::connected() const
{
    return ddp()->isConnected();
}

bool UserData::loggedIn() const
{
    return ddp()->isLoggedIn();
}

void UserData::tryLogin()
{
    qDebug() << "Attempting login" << password() << userName();
    ddp()->login();
}

UserData::UserData(QObject* parent)
 : QObject(parent),
 m_roomModel(new RoomModel)
{
    QSettings s;
    m_serverURL = s.value("serverURL", "wss://demo.rocket.chat/websocket").toString();
    m_ddp = new DDPClient(QUrl(m_serverURL));
    m_userName = s.value("username").toString();
    m_authToken = s.value("authToken").toString();
    connect(m_ddp, &DDPClient::loggedInChanged, this, &UserData::loggedInChanged);
}

UserData::~UserData()
{
    delete m_roomModel;
    delete m_ddp;
}

UserData * UserData::instance() 
{
    if (!m_self) {
        m_self = new UserData;
    }
    return m_self;
}

