/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "pinnedmessagemodel.h"

#include "threadmessages.h"

PinnedMessageModel::PinnedMessageModel(const QString &roomID, RocketChatAccount *account, Room *room, QObject *parent)
    : MessageModel(roomID, account, room, parent)
{
}

PinnedMessageModel::~PinnedMessageModel()
{
}

QString PinnedMessageModel::roomId() const
{
    return mRoomId;
}

void PinnedMessageModel::setRoomId(const QString &threadMessageId)
{
    mRoomId = threadMessageId;
}

void PinnedMessageModel::parse(const QJsonObject &obj)
{
    ThreadMessages threadmessages;
    threadmessages.parseThreadMessages(obj);
    for (int i = 0; i < threadmessages.count(); ++i) {
        addMessage(threadmessages.at(i));
    }
}

void PinnedMessageModel::parsePinnedMessages(const QJsonObject &obj)
{
    clear();
    parse(obj);
}

void PinnedMessageModel::loadMorePinnedMessages(const QJsonObject &obj)
{
    parse(obj);
}

int PinnedMessageModel::total() const
{
    return mTotal;
}

void PinnedMessageModel::setTotal(int total)
{
    mTotal = total;
}
