/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef SEARCHMESSAGEMODEL_H
#define SEARCHMESSAGEMODEL_H

#include <QAbstractListModel>
#include <QJsonObject>
#include "libruqola_private_export.h"
#include "searchmessage.h"
class TextConverter;
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT SearchMessageModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool stringNotFound READ stringNotFound WRITE setStringNotFound NOTIFY stringNotFoundChanged)
public:
    enum SearchMessageRoles {
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
        ThreadMessageId,
        ThreadMessagePreview,
        SortByTimeStamp,
        ShowTranslatedMessage,
    };
    Q_ENUM(SearchMessageRoles)

    explicit SearchMessageModel(RocketChatAccount *account, QObject *parent = nullptr);
    ~SearchMessageModel() override;

    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = {}) const override;
    void parseResult(const QJsonObject &obj, bool restApi = false);
    void clear();

    Q_REQUIRED_RESULT bool stringNotFound() const;
    void setStringNotFound(bool stringNotFound);

Q_SIGNALS:
    void stringNotFoundChanged();

private:
    Q_DISABLE_COPY(SearchMessageModel)
    QString convertMessageText(const QString &str, const QString &userName) const;
    void setMessages(const QVector<SearchMessage> &channels);
    QVector<SearchMessage> mSearchMessages;
    bool mStringNotFound = false;
    TextConverter *mTextConverter = nullptr;
    RocketChatAccount *mRocketChatAccount = nullptr;
};

#endif // SEARCHMESSAGEMODEL_H
