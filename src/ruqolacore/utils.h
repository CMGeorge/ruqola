/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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
#ifndef UTILS_H
#define UTILS_H

#include "libruqola_private_export.h"
#include "user.h"
#include <QUrl>

namespace Utils {
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QUrl generateServerUrl(const QString &url);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString markdownToRichText(const QString &markDown);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString presenceStatusToString(User::PresenceStatus status);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT User::PresenceStatus presenceStatusFromString(const QString &status);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString generateRichText(const QString &markDown, const QMap<QString, QString> &mentions, const QString &username);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString extractRoomUserFromUrl(QString url);
LIBRUQOLACORE_TESTS_EXPORT void parseNotification(const QJsonArray &contents, QString &message, QString &title, QString &sender);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT QString userIdFromDirectChannel(const QString &rid, const QString &userId);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT qint64 parseDate(const QString &key, const QJsonObject &o);
Q_REQUIRED_RESULT LIBRUQOLACORE_TESTS_EXPORT qint64 parseIsoDate(const QString &key, const QJsonObject &o);
}

#endif // UTILS_H
