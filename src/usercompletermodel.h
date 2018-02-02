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

#ifndef USERCOMPLETERMODEL_H
#define USERCOMPLETERMODEL_H

#include "user.h"

#include <QStringListModel>

class UserCompleterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UserRoles {
        UserName = Qt::UserRole + 1,
        UserId,
        UserIconStatus
    };
    Q_ENUM(UserRoles)

    explicit UserCompleterModel(QObject *parent = nullptr);
    ~UserCompleterModel();

    void insertUsers(const User &users);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void parseUser(const QJsonObject &root);
    void clear();
protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<User> mUsers;

};

#endif // USERCOMPLETERMODEL_H
