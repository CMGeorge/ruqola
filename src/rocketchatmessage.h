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

#ifndef ROCKETCHATMESSAGE_H
#define ROCKETCHATMESSAGE_H

#include "libruqola_private_export.h"
#include "user.h"
#include <QString>
#include <QJsonDocument>
#include <QObject>
class QDateTime;
class LIBRUQOLACORE_TESTS_EXPORT RocketChatMessage
{
    Q_GADGET
public:
    RocketChatMessage();
    ~RocketChatMessage() = default;

    struct RocketChatMessageResult
    {
        QString method;
        QString result;
        QJsonDocument jsonDocument;
    };

    void setJsonFormat(const QJsonDocument::JsonFormat &jsonFormat);

    RocketChatMessageResult hideRoom(const QString &roomID, quint64 id);
    RocketChatMessageResult leaveRoom(const QString &roomID, quint64 id);
    RocketChatMessageResult openRoom(const QString &roomID, quint64 id);
    RocketChatMessageResult unarchiveRoom(const QString &roomID, quint64 id);
    RocketChatMessageResult archiveRoom(const QString &roomID, quint64 id);
    RocketChatMessageResult eraseRoom(const QString &roomID, quint64 id);
    RocketChatMessageResult getRoomRoles(const QString &roomID, quint64 id);
    RocketChatMessageResult toggleFavorite(const QString &roomId, bool favorite, quint64 id);

    //save room settings
    RocketChatMessageResult setRoomName(const QString &roomId, const QString &name, quint64 id);

    RocketChatMessageResult generateMethod(const QString &method, const QJsonDocument &params, quint64 id);
    RocketChatMessage::RocketChatMessageResult setRoomIsReadOnly(const QString &roomId, bool readOnly, quint64 id);
    RocketChatMessage::RocketChatMessageResult setRoomHasSystemMessages(const QString &roomId, bool systemMessages, quint64 id);
    RocketChatMessage::RocketChatMessageResult setRoomIsDefault(const QString &roomId, bool isDefault, quint64 id);
    RocketChatMessage::RocketChatMessageResult setRoomJoinCode(const QString &roomId, const QString &joinCode, quint64 id);
    RocketChatMessage::RocketChatMessageResult setRoomTopic(const QString &roomId, const QString &topic, quint64 id);
    RocketChatMessage::RocketChatMessageResult setRoomDescription(const QString &roomId, const QString &description, quint64 id);
    RocketChatMessage::RocketChatMessageResult readMessages(const QString &roomID, quint64 id);
    RocketChatMessage::RocketChatMessageResult joinRoom(const QString &roomId, const QString &accessCode, quint64 id);
    RocketChatMessage::RocketChatMessageResult informTypingStatus(const QString &roomId, const QString &userId, bool typingStatus, quint64 id);
    RocketChatMessage::RocketChatMessageResult getSubscriptions(const QDateTime &lastUpdate, quint64 id);
    RocketChatMessage::RocketChatMessageResult setDefaultStatus(User::PresenceStatus status, quint64 id);
    RocketChatMessage::RocketChatMessageResult setTemporaryStatus(User::PresenceStatus status, quint64 id);
    RocketChatMessage::RocketChatMessageResult getUsersOfRoom(const QString &roomId, bool showAll, quint64 id);
    RocketChatMessage::RocketChatMessageResult logout(const QString &name, quint64 id);
    RocketChatMessage::RocketChatMessageResult createChannel(const QString &roomName, bool readOnly, quint64 id);
private:
    QJsonValue toJsonDateTime(const QDateTime &dateTime);
    RocketChatMessage::RocketChatMessageResult subscribe(const QString &name, const QJsonDocument &params, quint64 id);
    RocketChatMessage::RocketChatMessageResult saveRoomSettings(const QString &key, const QString &roomId, const QJsonValue &value, quint64 id);
    QJsonDocument::JsonFormat mJsonFormat;
};

#endif // ROCKETCHATMESSAGE_H
