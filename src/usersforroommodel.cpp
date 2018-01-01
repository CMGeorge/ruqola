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

#include "usersforroommodel.h"
#include "usersmodel.h"
#include "ruqola_debug.h"

UsersForRoomModel::UsersForRoomModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

UsersForRoomModel::~UsersForRoomModel()
{
}

bool UsersForRoomModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mCurrentRoomId.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "current room id is not defined. It's a bug";
        return false;
    }
    const QModelIndex idx = sourceModel()->index(source_row, 0, source_parent);
    if (!idx.isValid()) {
        return false;
    }
    const QStringList lst = idx.data(UsersModel::UserListRooms).toStringList();
    if (!lst.isEmpty()) {
        return false;
    }

    return lst.contains(mCurrentRoomId);
}

QString UsersForRoomModel::currentRoomId() const
{
    return mCurrentRoomId;
}

void UsersForRoomModel::setCurrentRoomId(const QString &currentRoomId)
{
    mCurrentRoomId = currentRoomId;
}
