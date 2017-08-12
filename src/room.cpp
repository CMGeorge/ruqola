/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright 2017  Laurent Montel <montel@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "room.h"
#include <QDebug>
Room::Room()
{
}

bool Room::isEqual(const Room &other) const
{
    return (id == other.id)
           && (type == other.type)
           && (name == other.name)
           && (mAnnouncement == other.mAnnouncement)
           && (userName == other.userName)
           && (userID == other.userID)
           && (topic == other.topic)
           && (mutedUsers == other.mutedUsers)
           && (jitsiTimeout == other.jitsiTimeout)
           && (ro == other.ro)
           && (unread == other.unread)
           && (selected == other.selected)
           && (favorite == other.favorite)
           && (open == other.open);
}

QDebug operator <<(QDebug d, const Room &t)
{
    d << "id :" << t.id;
    d << "type :" << t.type;
    d << "name :" << t.name;
    d << "mAnnouncement :" << t.mAnnouncement;
    d << "userName :" << t.userName;
    d << "userID :" << t.userID;
    d << "topic :" << t.topic;
    d << "mutedUsers :" << t.mutedUsers;
    d << "jitsiTimeout :" << t.jitsiTimeout;
    d << "ro :" << t.ro;
    d << "unread :" << t.unread;
    d << "selected :" << t.selected;
    d << "favorite :" << t.favorite;
    d << "open :" << t.open;
    return d;
}
