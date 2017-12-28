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

#include "restapiutil.h"

QString RestApiUtil::adaptUrl(const QString &url)
{
    if (url.isEmpty()) {
        return url;
    }
    //Avoid to add more https:// :)
    if (url.startsWith(QLatin1String("https://")) || url.startsWith(QLatin1String("http://"))) {
        return url;
    } else {
        //Default to https
        return QStringLiteral("https://") + url;
    }
}

QString RestApiUtil::apiUri()
{
    return QStringLiteral("/api/v1/");
}

QString RestApiUtil::restUrl(RestApiUtil::RestApiUrlType type)
{
    switch (type) {
    case RestApiUtil::RestApiUrlType::Login:
        return QStringLiteral("login");
    case RestApiUtil::RestApiUrlType::Logout:
        return QStringLiteral("logout");
    case RestApiUtil::RestApiUrlType::Me:
        return QStringLiteral("me");
    case RestApiUtil::RestApiUrlType::UsersGetAvatar:
        return QStringLiteral("users.getAvatar");
    case RestApiUtil::RestApiUrlType::UsersCreate:
        return QStringLiteral("users.create");
    case RestApiUtil::RestApiUrlType::UsersCreateToken:
        return QStringLiteral("users.createToken");
    case RestApiUtil::RestApiUrlType::UsersGetPresence:
        return QStringLiteral("users.getPresence");
    case RestApiUtil::RestApiUrlType::UsersInfo:
        return QStringLiteral("users.info");
    case RestApiUtil::RestApiUrlType::UsersList:
        return QStringLiteral("users.list");
    case RestApiUtil::RestApiUrlType::UsersRegister:
        return QStringLiteral("users.register");
    case RestApiUtil::RestApiUrlType::UsersResetAvatar:
        return QStringLiteral("users.resetAvatar");
    case RestApiUtil::RestApiUrlType::UsersSetAvatar:
        return QStringLiteral("users.setAvatar");
    case RestApiUtil::RestApiUrlType::UsersUpdate:
        return QStringLiteral("users.update");
    case RestApiUtil::RestApiUrlType::ChatDelete:
        return QStringLiteral("chat.delete");
    case RestApiUtil::RestApiUrlType::ChatGetMessage:
        return QStringLiteral("chat.getMessage");
    case RestApiUtil::RestApiUrlType::ChatPinMessage:
        return QStringLiteral("chat.pinMessage");
    case RestApiUtil::RestApiUrlType::ChatPostMessage:
        return QStringLiteral("chat.postMessage");
    case RestApiUtil::RestApiUrlType::ChatReact:
        return QStringLiteral("chat.react");
    case RestApiUtil::RestApiUrlType::ChatStarMessage:
        return QStringLiteral("chat.starMessage");
    case RestApiUtil::RestApiUrlType::ChatUnPinMessage:
        return QStringLiteral("chat.unPinMessage");
    case RestApiUtil::RestApiUrlType::ChatUnStarMessage:
        return QStringLiteral("chat.unStarMessage");
    case RestApiUtil::RestApiUrlType::ChatUpdate:
        return QStringLiteral("chat.update");
    case RestApiUtil::RestApiUrlType::ChannelsAddAll:
        return QStringLiteral("channels.addAll");
    case RestApiUtil::RestApiUrlType::ChannelsAddModerator:
        return QStringLiteral("channels.addModerator");
    case RestApiUtil::RestApiUrlType::ChannelsAddOwner:
        return QStringLiteral("channels.addOwner");
    case RestApiUtil::RestApiUrlType::ChannelsArchive:
        return QStringLiteral("channels.archive");
    case RestApiUtil::RestApiUrlType::ChannelsCleanHistory:
        return QStringLiteral("channels.cleanHistory");
    case RestApiUtil::RestApiUrlType::ChannelsClose:
        return QStringLiteral("channels.close");
    case RestApiUtil::RestApiUrlType::ChannelsCreate:
        return QStringLiteral("channels.create");
    case RestApiUtil::RestApiUrlType::ChannelsGetIntegrations:
        return QStringLiteral("channels.getIntegrations");
    case RestApiUtil::RestApiUrlType::ChannelsHistory:
        return QStringLiteral("channels.history");
    case RestApiUtil::RestApiUrlType::ChannelsInfo:
        return QStringLiteral("channels.info");
    case RestApiUtil::RestApiUrlType::ChannelsInvite:
        return QStringLiteral("channels.invite");
    case RestApiUtil::RestApiUrlType::ChannelsKick:
        return QStringLiteral("channels.kick");
    case RestApiUtil::RestApiUrlType::ChannelsLeave:
        return QStringLiteral("channels.leave");
    case RestApiUtil::RestApiUrlType::ChannelsList:
        return QStringLiteral("channels.list");
    case RestApiUtil::RestApiUrlType::ChannelsListJoined:
        return QStringLiteral("channels.list.joined");
    case RestApiUtil::RestApiUrlType::ChannelsOpen:
        return QStringLiteral("channels.open");
    case RestApiUtil::RestApiUrlType::ChannelsRemoveModerator:
        return QStringLiteral("channels.removeModerator");
    case RestApiUtil::RestApiUrlType::ChannelsRemoveOwner:
        return QStringLiteral("channels.removeOwner");
    case RestApiUtil::RestApiUrlType::ChannelsRename:
        return QStringLiteral("channels.rename");
    case RestApiUtil::RestApiUrlType::ChannelsSetDescription:
        return QStringLiteral("channels.setDescription");
    case RestApiUtil::RestApiUrlType::ChannelsSetJoinCode:
        return QStringLiteral("channels.setJoinCode");
    case RestApiUtil::RestApiUrlType::ChannelsSetPurpose:
        return QStringLiteral("channels.setPurpose");
    case RestApiUtil::RestApiUrlType::ChannelsSetReadOnly:
        return QStringLiteral("channels.setReadOnly");
    case RestApiUtil::RestApiUrlType::ChannelsSetTopic:
        return QStringLiteral("channels.setTopic");
    case RestApiUtil::RestApiUrlType::ChannelsSetType:
        return QStringLiteral("channels.setType");
    case RestApiUtil::RestApiUrlType::ChannelsUnarchive:
        return QStringLiteral("channels.unarchive");
    }
#if 0
    //Channel
    return QStringLiteral("channels.addAll");
    return QStringLiteral("channels.addModerator");
    return QStringLiteral("channels.addOwner");
    return QStringLiteral("channels.archive");
    return QStringLiteral("channels.cleanHistory");
    return QStringLiteral("channels.close");
    return QStringLiteral("channels.create");
    return QStringLiteral("channels.getIntegrations");
    return QStringLiteral("channels.history");
    return QStringLiteral("channels.info");
    return QStringLiteral("channels.invite");
    return QStringLiteral("channels.kick");
    return QStringLiteral("channels.leave");
    return QStringLiteral("channels.list");
    return QStringLiteral("channels.list.joined");
    return QStringLiteral("channels.open");
    return QStringLiteral("channels.removeModerator");
    return QStringLiteral("channels.removeOwner");
    return QStringLiteral("channels.rename");
    return QStringLiteral("channels.setDescription");
    return QStringLiteral("channels.setJoinCode");
    return QStringLiteral("channels.setPurpose");
    return QStringLiteral("channels.setReadOnly");
    return QStringLiteral("channels.setTopic");
    return QStringLiteral("channels.setType");
    return QStringLiteral("channels.unarchive");
    //Group
    return QStringLiteral("groups.addAll");
    return QStringLiteral("groups.addModerator");
    return QStringLiteral("groups.addOwner");
    return QStringLiteral("groups.archive");
    return QStringLiteral("groups.close");
    return QStringLiteral("groups.create");
    return QStringLiteral("groups.getIntegrations");
    return QStringLiteral("groups.history");
    return QStringLiteral("groups.info");
    return QStringLiteral("groups.invite");
    return QStringLiteral("groups.kick");
    return QStringLiteral("groups.leave");
    return QStringLiteral("groups.list");
    return QStringLiteral("groups.open");
    return QStringLiteral("groups.removeModerator");
    return QStringLiteral("groups.removeOwner");
    return QStringLiteral("groups.rename");
    return QStringLiteral("groups.setDescription");
    return QStringLiteral("groups.setPurpose");
    return QStringLiteral("groups.setReadOnly");
    return QStringLiteral("groups.setTopic");
    return QStringLiteral("groups.setType");
    return QStringLiteral("groups.unarchive");
#endif
    return {};
}
