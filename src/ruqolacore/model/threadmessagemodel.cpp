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

#include "threadmessagemodel.h"

#include "threadmessages.h"

ThreadMessageModel::ThreadMessageModel(const QString &roomID, RocketChatAccount *account, Room *room, QObject *parent)
    : MessageModel(roomID, account, room, parent)
{
}

ThreadMessageModel::~ThreadMessageModel()
{
}

QString ThreadMessageModel::threadMessageId() const
{
    return mThreadMessageId;
}

void ThreadMessageModel::setThreadMessageId(const QString &threadMessageId)
{
    mThreadMessageId = threadMessageId;
}


void ThreadMessageModel::parseThreadMessages(const QJsonObject &obj)
{
    ThreadMessages threadmessages;
    threadmessages.parseThreadMessages(obj);
    clear();
    for (int i = 0; i < threadmessages.count(); ++i) {
        addMessage(threadmessages.at(i));
    }
}

void ThreadMessageModel::loadMoreThreadMessages(const QJsonObject &obj)
{
    //TODO
}

int ThreadMessageModel::total() const
{
    return mTotal;
}

void ThreadMessageModel::setTotal(int total)
{
    mTotal = total;
}