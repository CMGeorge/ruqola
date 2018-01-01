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

#include "usersmodel.h"
#include "ruqola_debug.h"

#include <QJsonObject>
UsersModel::UsersModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

UsersModel::~UsersModel()
{
    qCDebug(RUQOLA_LOG) << "UsersModel::~UsersModel() "<< mUsers.count();
    qDeleteAll(mUsers);
}

int UsersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mUsers.size();
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
    User *user = mUsers.at(index.row());
    if (user) {
        switch (role) {
        case UserName:
            return user->name();
        case UserId:
            return user->userId();
        case UserStatus:
            return user->status();
        case UserListRooms:
            return user->listRooms();
        case UserIcon:
            return user->iconFromStatus();
        default:
            qCWarning(RUQOLA_LOG) << "Unknow usersmodel roles: " << role;
        }
    }
    return {};
}

User *UsersModel::user(const QString &userId)
{
    const int userCount{
        mUsers.count()
    };
    for (int i = 0; i < userCount; ++i) {
        if (mUsers.at(i)->userId() == userId) {
            return mUsers.at(i);
        }
    }
    return nullptr;
}

void UsersModel::removeUser(const QString &userId)
{
    qCDebug(RUQOLA_LOG) << " User removed " << userId;
    const int userCount{
        mUsers.count()
    };
    for (int i = 0; i < userCount; ++i) {
        if (mUsers.at(i)->userId() == userId) {
            qCDebug(RUQOLA_LOG) << " User removed " << mUsers.at(i)->name();
            beginRemoveRows(QModelIndex(), i, i);
            delete mUsers.takeAt(i);
            endRemoveRows();
            break;
        }
    }
}

void UsersModel::addUser(User *user)
{
    qCDebug(RUQOLA_LOG) << " User added " << *user;
    //TODO verify if duplicate ?
    const int pos = mUsers.size();
    beginInsertRows(QModelIndex(), pos, pos);
    mUsers.append(user);
    endInsertRows();
}

void UsersModel::updateUser(const QJsonObject &array)
{
    const QString id = array.value(QLatin1String("id")).toString();
    const int userCount{
        mUsers.count()
    };
    for (int i = 0; i < userCount; ++i) {
        if (mUsers.at(i)->userId() == id) {
            User *user = mUsers.at(i);
            const QJsonObject fields = array.value(QLatin1String("fields")).toObject();
            const QString newStatus = fields.value(QLatin1String("status")).toString();
            user->setStatus(newStatus);
            //TODO name ?
            Q_EMIT dataChanged(createIndex(i, 0), createIndex(i, 0));
            break;
        }
    }
}
