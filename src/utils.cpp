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

#include "utils.h"
#include "ruqola_debug.h"
#include <KTextToHTML>

QUrl Utils::generateServerUrl(const QString &url)
{
    if (url.isEmpty()) {
        return {};
    }
    QString serverUrl = url;
    if (serverUrl.startsWith(QLatin1String("https://"))) {
        serverUrl.replace(QStringLiteral("https://"), QStringLiteral("wss://"));
    } else if (serverUrl.startsWith(QLatin1String("http://"))) {
        serverUrl.replace(QStringLiteral("http://"), QStringLiteral("ws://"));
    } else {
        serverUrl = QStringLiteral("wss://") + serverUrl;
    }
    return QUrl(serverUrl + QStringLiteral("/websocket"));
}

QString Utils::markdownToRichText(const QString &markDown)
{
    //qCDebug(RUQOLA_LOG) << "BEFORE markdownToRichText "<<markDown;
    //Problem with smileys as qml load image as qrc:/.... so perhaps we need to change contextProperty("baseUrl"...)
    const KTextToHTML::Options convertFlags = KTextToHTML::PreserveSpaces | KTextToHTML::HighlightText /* | KTextToHTML::ReplaceSmileys*/;
    const QString str = KTextToHTML::convertToHtml(markDown, convertFlags);
    //qCDebug(RUQOLA_LOG) << "markdownToRichText "<<str;
    return str;
}

QString Utils::presenceStatusToString(User::PresenceStatus status)
{
    switch (status) {
    case User::PresenceStatus::PresenceOnline:
        return QStringLiteral("online");
    case User::PresenceStatus::PresenceBusy:
        return QStringLiteral("busy");
    case User::PresenceStatus::PresenceAway:
        return QStringLiteral("away");
    case User::PresenceStatus::PresenceOffline:
        return QStringLiteral("offline");
    case User::PresenceStatus::Unknown:
        return {};
    }
    Q_UNREACHABLE();
    return {};
}

User::PresenceStatus Utils::presenceStatusFromString(const QString &status)
{
    if (status == QStringLiteral("online")) {
        return User::PresenceStatus::PresenceOnline;
    } else if (status == QStringLiteral("busy")) {
        return User::PresenceStatus::PresenceBusy;
    } else if (status == QStringLiteral("away")) {
        return User::PresenceStatus::PresenceAway;
    } else if (status == QStringLiteral("offline")) {
        return User::PresenceStatus::PresenceOffline;
    } else {
        qCDebug(RUQOLA_LOG) << "Problem with status " << status;
        return {};
    }
}
