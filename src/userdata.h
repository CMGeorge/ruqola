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

#ifndef USERDATA_H
#define USERDATA_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include "ddpclient.h"
#include "roommodel.h"
#include "messagemodel.h"

class QString;

class UserData: public QObject
{
    Q_OBJECT
    
    Q_PROPERTY (QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY (QString serverURL READ serverURL WRITE setServerURL NOTIFY serverURLChanged)
    Q_PROPERTY (QString password WRITE setPassword)
    Q_PROPERTY (bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY (bool loggedIn READ loggedIn NOTIFY loggedInChanged)
    
public:
    static UserData* instance();

    void setUserName(const QString &username);
    QString userName() const;
    
    void setPassword(const QString &password);
    QString password() const;
    
    void setAuthToken(const QString &token);
    QString authToken() const;
    
    bool connected() const;
    bool loggedIn() const;
    
    QString serverURL() const;
    void setServerURL(const QString &serverURL);
    
    DDPClient *ddp() const;
    Q_INVOKABLE RoomModel *roomModel() const;
    
    Q_INVOKABLE void sendMessage(const QString &roomID, const QString &message);
    Q_INVOKABLE MessageModel* getModelForRoom(const QString &roomID);
    
    Q_INVOKABLE void tryLogin();
    //     void setRoomModel();
    
    
signals:
    void userNameChanged();
    void connectedChanged();
    void serverURLChanged();
    void loggedInChanged();
    
private:
    UserData(QObject *parent = 0);
    ~UserData();
    static UserData *m_self;
    QString m_password;
    QString m_userName;
    QString m_authToken;
    QString m_serverURL;
    
    RoomModel *m_roomModel;
    DDPClient *m_ddp;
    QHash< QString, MessageModel * > m_messageModels;

    
};

inline static QObject *userdata_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    UserData *userData = UserData::instance();
    return userData;
}

#endif // USERDATA_H
