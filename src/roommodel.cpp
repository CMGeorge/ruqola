/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
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

#include "roommodel.h"
#include "ruqola.h"
#include "ruqola_debug.h"

#include <QAbstractItemModel>
#include <QtCore>

RoomWrapper::RoomWrapper(QObject *parent)
    : QObject(parent)
{
}

RoomWrapper::RoomWrapper(const Room &r, QObject *parent)
    : QObject(parent)
{
    m_name = r.name;
    m_topic = r.topic;
    m_unread = r.unread;
    m_id = r.id;
    m_selected = r.selected;
}

Room RoomModel::fromJSon(const QJsonObject &o)
{
    Room r;

    r.id = o[QStringLiteral("id")].toString();
    r.type = o[QStringLiteral("t")].toString();
    r.name = o[QStringLiteral("name")].toString();
    r.userName = o[QStringLiteral("userName")].toString();
    r.userID = o[QStringLiteral("userID")].toString();
    r.topic = o[QStringLiteral("topic")].toString();
    r.mutedUsers = o[QStringLiteral("mutedUsers")].toString();
    r.jitsiTimeout = o[QStringLiteral("jitsiTimeout")].toDouble();
    r.ro = o[QStringLiteral("ro")].toBool();
    r.unread = o[QStringLiteral("unread")].toInt(0);

    return r;
}

QByteArray RoomModel::serialize(const Room &r)
{
    QJsonDocument d;
    QJsonObject o;

    o[QStringLiteral("id")] = r.id;
    o[QStringLiteral("t")] = r.type;
    o[QStringLiteral("name")] = r.name;
    o[QStringLiteral("userName")] = r.userName;
    o[QStringLiteral("userID")] = r.userID;
    o[QStringLiteral("topic")] = r.topic;
    o[QStringLiteral("mutedUsers")] = r.mutedUsers;
    o[QStringLiteral("jitsiTimeout")] = r.jitsiTimeout;
    o[QStringLiteral("ro")] = r.ro;
    o[QStringLiteral("unread")] = r.unread;

    d.setObject(o);
    return d.toBinaryData();
}

QString RoomWrapper::getName() const
{
    return m_name;
}

QString RoomWrapper::getTopic() const
{
    return m_topic;
}

RoomModel::RoomModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

RoomModel::~RoomModel()
{
    QDir cacheDir(Ruqola::self()->cacheBasePath());
    if (!cacheDir.exists(cacheDir.path())) {
        cacheDir.mkpath(cacheDir.path());
    }

    QFile f(cacheDir.absoluteFilePath(QStringLiteral("rooms")));

    if (f.open(QIODevice::WriteOnly)) {
        QDataStream out(&f);
        for (const Room &m : qAsConst(m_roomsList)) {
            const QByteArray ms = RoomModel::serialize(m);
            out.writeBytes(ms, ms.size());
        }
    }
}

void RoomModel::clear()
{
    if (!m_roomsList.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        m_roomsList.clear();
        QAbstractItemModel::endRemoveRows();
    }
}

RoomWrapper *RoomModel::findRoom(const QString &roomID) const
{
    foreach (const Room &r, m_roomsList) {
        if (r.id == roomID) {
            return new RoomWrapper(r);
        }
    }
    Room r;
    return new RoomWrapper(r);
}

// Clear data and refill it with data in the cache, if there is
void RoomModel::reset()
{
    if (Ruqola::self()->cacheBasePath().isEmpty()) {
        return;
    }

    clear();

    QDir cacheDir(Ruqola::self()->cacheBasePath());
    // load cache
    if (cacheDir.exists(cacheDir.path())) {
        QFile f(cacheDir.absoluteFilePath(QStringLiteral("rooms")));
        if (f.open(QIODevice::ReadOnly)) {
            QDataStream in(&f);
            while (!f.atEnd()) {
                char *byteArray;
                quint32 length;
                in.readBytes(byteArray, length);
                QByteArray arr = QByteArray::fromRawData(byteArray, length);
                Room m = RoomModel::fromJSon(QJsonDocument::fromBinaryData(arr).object());
                addRoom(m.id, m.name, m.selected);
            }
        }
        qCDebug(RUQOLA_LOG) << "Cache Loaded";
    }
}

QHash<int, QByteArray> RoomModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RoomName] = "name";
    roles[RoomID] = "room_id";
    roles[RoomSelected] = "selected";
    roles[RoomUnread] = "unread";
    roles[RoomType] = "type";
    roles[RoomUserName] = "username";
    roles[RoomUserID] = "userID";
    roles[RoomTopic] = "topic";
    roles[RoomMutedUsers] = "mutedUsers";
    roles[RoomJitsiTimeout] = "jitsiTimeout";
    roles[RoomRO] = "readOnly";
    return roles;
}

int RoomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_roomsList.size();
}

QVariant RoomModel::data(const QModelIndex &index, int role) const
{
    Room r = m_roomsList.at(index.row());

    if (role == RoomModel::RoomName) {
        return r.name;
    } else if (role == RoomModel::RoomID) {
        return r.id;
    } else if (role == RoomModel::RoomSelected) {
        return r.selected;
    } else if (role == RoomModel::RoomType) {
        return r.type;
    } else if (role == RoomModel::RoomUserID) {
        return r.userID;
    } else if (role == RoomModel::RoomUserName) {
        return r.userName;
    } else if (role == RoomModel::RoomTopic) {
        return r.topic;
    } else if (role == RoomModel::RoomMutedUsers) {
        return r.mutedUsers;
    } else if (role == RoomModel::RoomJitsiTimeout) {
        return r.jitsiTimeout;
    } else if (role == RoomModel::RoomRO) {
        return r.ro;
    } else {
        return QVariant(QStringLiteral("0"));
    }
}

void RoomModel::addRoom(const QString &roomID, const QString &roomName, bool selected)
{
    if (roomID.isEmpty() || roomName.isEmpty()) {
        qDebug() << " Impossible to add a room";
        return;
    }
    qCDebug(RUQOLA_LOG) << "Adding room : roomId: " << roomID << " room Name " << roomName << " isSelected : " << selected;

    Room r;
    r.id = roomID;
    r.name = roomName;
    r.selected = selected;
    addRoom(r);
}

void RoomModel::addRoom(const Room &room)
{
    auto existingRoom = std::find(m_roomsList.begin(), m_roomsList.end(), room);
    bool present = (existingRoom != m_roomsList.end());

    auto i = std::upper_bound(m_roomsList.begin(), m_roomsList.end(),
                              room);
    int pos = i-m_roomsList.begin();
    bool roomChanged = false;
    qCDebug(RUQOLA_LOG) << pos;

//     if (qFind(m_roomsList.begin(), m_roomsList.end(), room) != m_roomsList.end() && pos > 0) {
    if (present) {
//         qCDebug(RUQOLA_LOG) << (qFind(m_roomsList.begin(), m_roomsList.end(), room) - m_roomsList.begin());
//     if (pos != m_roomsList.size()) { // we're at the end
        qCDebug(RUQOLA_LOG) << "Room changed!";
        roomChanged = true;
        //Figure out a better way to update just the really changed message
    } else {
        beginInsertRows(QModelIndex(), pos, pos);
    }

    if (roomChanged) {
        m_roomsList.replace(pos-1, room);
    } else {
        qCDebug(RUQOLA_LOG) << "Inserting room at position" <<pos;
        m_roomsList.insert(i, room);
    }

    if (roomChanged) {
        emit dataChanged(createIndex(1, 1), createIndex(pos, 1));
    } else {
        endInsertRows();
    }

    Ruqola::self()->getModelForRoom(room.id);
}
