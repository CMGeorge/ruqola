/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#ifndef ROCKETCHATACCOUNT_H
#define ROCKETCHATACCOUNT_H

#include <QObject>
#include <ddpapi/ddpclient.h>
#include "rocketchataccountsettings.h"
#include "libruqola_private_export.h"
class TypingNotification;
class UsersModel;
class RoomModel;
class RoomWrapper;
class MessageModel;
class DDPClient;
class RestApiRequest;
class MessageQueue;
class RocketChatBackend;
class UsersForRoomModel;
class RoomFilterProxyModel;
class LIBRUQOLACORE_TESTS_EXPORT RocketChatAccount : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    //Q_PROPERTY(QString userID READ userID WRITE setUserID NOTIFY userIDChanged)
    //Q_PROPERTY(QString serverURL READ serverURL WRITE setServerURL NOTIFY serverURLChanged)
    //Q_PROPERTY(QString password WRITE setPassword)
    Q_PROPERTY(DDPClient::LoginStatus loginStatus READ loginStatus NOTIFY loginStatusChanged)
public:
    explicit RocketChatAccount(QObject *parent = nullptr);
    ~RocketChatAccount();

    RocketChatAccountSettings *settings() const;

    Q_INVOKABLE UsersModel *usersModel() const;

    Q_INVOKABLE RoomModel *roomModel() const;

    Q_INVOKABLE RoomFilterProxyModel *roomFilterProxyModel() const;
    Q_INVOKABLE RoomWrapper *getRoom(const QString &roomId);
    Q_INVOKABLE MessageModel *getMessageModelForRoom(const QString &roomID);
    Q_INVOKABLE UsersForRoomModel *getUsersForRoomModel(const QString &roomId);
    Q_INVOKABLE QString getUserCurrentMessage(const QString &roomId);
    Q_INVOKABLE void setUserCurrentMessage(const QString &message, const QString &roomId);

    Q_INVOKABLE void textEditing(const QString &roomId, const QString &str);

    Q_INVOKABLE void attachmentButtonClicked(const QString &roomId);
    Q_INVOKABLE void leaveRoom(const QString &roomId);
    Q_INVOKABLE void hideRoom(const QString &roomId);
    Q_INVOKABLE void tryLogin();
    Q_INVOKABLE void logOut();
    Q_INVOKABLE void clearUnreadMessages(const QString &roomId);

    DDPClient *ddp();

    DDPClient::LoginStatus loginStatus();
    RestApiRequest *restApi() const;
    RestApiRequest *restapi();

    //Make it private in future
    void slotInformTypingStatus(const QString &room, bool typing);
    void sendMessage(const QString &roomID, const QString &message, const QString &type);

    MessageQueue *messageQueue() const;

    RocketChatBackend *rocketChatBackend() const;


Q_SIGNALS:
    void userNameChanged();
    void userIDChanged();
    void serverURLChanged();
    void loginStatusChanged();
    void added(const QJsonObject &item);
    void changed(const QJsonObject &item);
    void notification(const QString &title, const QString &message);

private:
    void loadSettings();
    void clearModels();
    RocketChatAccountSettings *mSettings = nullptr;
    //room, messagemodel
    QHash<QString, MessageModel *> mMessageModels;
    QHash<QString, UsersForRoomModel *> mUsersForRoomModels;
    QHash<QString, QString> mUserCurrentMessage;

    TypingNotification *mTypingNotification = nullptr;
    UsersModel *mUserModel = nullptr;
    RoomModel *mRoomModel = nullptr;
    RoomFilterProxyModel *mRoomFilterProxyModel = nullptr;
    DDPClient *mDdp = nullptr;
    RestApiRequest *mRestApi = nullptr;
    MessageQueue *mMessageQueue = nullptr;
    RocketChatBackend *mRocketChatBackend = nullptr;
};

#endif // ROCKETCHATACCOUNT_H
