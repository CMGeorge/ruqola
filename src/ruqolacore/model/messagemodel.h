/*

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

#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include "libruqola_private_export.h"
#include "messages/message.h"
#include <QAbstractListModel>
#include <QObject>
#include <QByteArray>

class RocketChatAccount;
class TextConverter;
class LoadRecentHistoryManager;
class Room;
class LIBRUQOLACORE_EXPORT MessageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MessageRoles {
        Username = Qt::UserRole + 1,
        OriginalMessage,
        MessageConvertedText,
        Timestamp,
        UserId,
        SystemMessageType,
        MessageId,
        RoomId,
        UpdatedAt,
        EditedAt,
        EditedByUserName,
        EditedByUserId,
        Alias,
        Avatar,
        Groupable,
        ParseUrls,
        MessageType,
        Attachments,
        Urls,
        Date,
        CanEditMessage,
        Starred,
        UsernameUrl,
        Roles,
        Reactions,
        Ignored,
        Pinned,
        DiscussionCount,
        DiscussionRoomId,
        DiscussionLastMessage,
        ThreadCount,
        ThreadLastMessage,
        ThreadMessageId
    };
    Q_ENUM(MessageRoles)

    explicit MessageModel(const QString &roomID = QStringLiteral("no_room"), RocketChatAccount *account = nullptr, Room *room = nullptr, QObject *parent = nullptr);
    ~MessageModel() override;

    /**
    * @brief Adds a message to QVector m_allMessages
    *
    * @param message The message to be added
    */
    void addMessage(const Message &message);

    /**
    * @brief returns number of messages in QVector m_allMessages
    *
    * @param parent, it is void
    * @return int, The number of messages in QVector mAllMessages
    */
    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
    * @brief Returns last timestamp of last message in QVector mAllMessages
    *
    * @return qint64 The last timestamp
    */
    Q_REQUIRED_RESULT qint64 lastTimestamp() const;

    void deleteMessage(const QString &messageId);

    Q_REQUIRED_RESULT qint64 generateNewStartTimeStamp(qint64 lastTimeStamp);
    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

    void setRoomID(const QString &roomID);
    Q_REQUIRED_RESULT bool isEmpty() const;

private:
    Q_DISABLE_COPY(MessageModel)
    void refresh();
    QStringList roomRoles(const QString &userId) const;
    QString convertMessageText(const QString &str, const QString &userName, const QString &threadMessageId) const;
    QString mRoomID;
    QVector<Message> mAllMessages;
    RocketChatAccount *mRocketChatAccount = nullptr;
    TextConverter *mTextConverter = nullptr;
    Room *mRoom = nullptr;
    LoadRecentHistoryManager *mLoadRecentHistoryManager = nullptr;
};

#endif
