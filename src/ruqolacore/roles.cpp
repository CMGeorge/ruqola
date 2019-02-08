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

#include "roles.h"
#include "ruqola_debug.h"

#include <QJsonArray>
#include <QJsonObject>

Roles::Roles()
{

}

QVector<Role> Roles::roles() const
{
    return mRoles;
}

bool Roles::isEmpty() const
{
    return mRoles.isEmpty();
}


void Roles::setRoles(const QVector<Role> &roles)
{
    mRoles = roles;
}

void Roles::parseRole(const QJsonObject &obj)
{
    mRoles.clear();

    const QJsonArray roleArray = obj[QStringLiteral("roles")].toArray();
    const int roleArrayCount = roleArray.count();
    mRoles.reserve(roleArrayCount);
    for (int i = 0; i < roleArrayCount; ++i) {
        Role r;
        r.parseRole(roleArray.at(i).toObject());
        if (r.isValid()) {
            mRoles.append(r);
        } else {
            qCWarning(RUQOLA_LOG) << "Invalid role" << roleArray.at(i).toObject();
        }
    }
}

Role Roles::findRoleByUserId(const QString &userId)
{
    for (const Role &r : qAsConst(mRoles)) {
        if (r.userId() == userId) {
            return r;
        }
    }
    return {};
}