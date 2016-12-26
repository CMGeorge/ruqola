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

#include <QtCore/QAbstractItemModel>
#include <QtCore>

#include "userdata.h"

Room RoomModel::fromJSon(const QJsonObject& o)
{
    Room r;
    
    r.name = o["name"].toString();
    r.id = o["id"].toString();
    return r;
}

QByteArray RoomModel::serialize(const Room& r)
{
    QJsonDocument d;
    QJsonObject o;
    o["name"] = r.name;
    o["id"] = r.id;
    d.setObject(o);
    return d.toBinaryData();
}

RoomModel::RoomModel(QObject* parent)
    : QAbstractListModel(parent)
{
//     connect(UserData::self(), &UserData::loginStatusChanged, this, &RoomModel::onLoginStatusChanged);
}

RoomModel::~RoomModel()
{
    QDir cacheDir(UserData::self()->cacheBasePath());
    if (!cacheDir.exists(cacheDir.path())) {
        cacheDir.mkpath(cacheDir.path());
    }
    
    QFile f(cacheDir.absoluteFilePath("rooms"));

    if (f.open(QIODevice::WriteOnly)) {
        QDataStream out(&f);
        foreach (const Room m, m_roomsList) {            
            QByteArray ms = RoomModel::serialize(m);
            out.writeBytes(ms, ms.size());
        }
    }
}

void RoomModel::clear()
{
    if (m_roomsList.size()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        m_roomsList.clear();
        QAbstractItemModel::endRemoveRows();
    }
}

// Clear data and refill it it with data in the cache, if there is
void RoomModel::reset()
{
    if (UserData::self()->cacheBasePath().isEmpty()) {
        return;
    }
    
    clear();
    
//     beginResetModel();
//     m_roomsList.clear();
//     endResetModel();
//     QList roomsList;
    QDir cacheDir(UserData::self()->cacheBasePath());
    // load cache
    if (cacheDir.exists(cacheDir.path())) {
        QFile f(cacheDir.absoluteFilePath("rooms"));
        if (f.open(QIODevice::ReadOnly)) {
            QDataStream in(&f);
            while (!f.atEnd()) {
                char * byteArray; quint32 length;
                in.readBytes(byteArray, length);
                QByteArray arr = QByteArray::fromRawData(byteArray, length);
                Room m = RoomModel::fromJSon(QJsonDocument::fromBinaryData(arr).object());
                // This cache creates some instabilities
//                                 m_roomsList[m.name] = m;
                addRoom(m.id, m.name, m.selected);
            }
        }
    }
    qDebug() << "Cache Loaded";
}

QHash<int, QByteArray> RoomModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RoomName] = "name";
    roles[RoomID] = "room_id";
    roles[RoomSelected] = "selected";
    return roles;
}

int RoomModel::rowCount(const QModelIndex & parent) const
{
    return m_roomsList.size();
}

QVariant RoomModel::data(const QModelIndex & index, int role) const
{
    Room r = m_roomsList.values().at(index.row());
     if (role == RoomModel::RoomName) {
        return  r.name;
    } else if (role == RoomModel::RoomID) {
        return r.id;
    } else if (role == RoomModel::RoomSelected) {
        return r.selected;
    } else {
        return QVariant("0");
    }
}

void RoomModel::addRoom(const QString& roomID, const QString& roomName, bool selected)
{
//     qDebug() << m_roomsList.size();
//     return;
//     qDebug() << "Adding room" << roomID << roomName << m_roomsList.keys();
    
    if (roomID.isEmpty()) {
        return;
    }
    
    bool updating = false;
    qDebug() << "A te ti vedo";
    
    if (m_roomsList.contains(roomName)) {
//             qDebug() << "ESISTO GIa";
        // we are doing an update
        updating = true;
    }
    
    int size = m_roomsList.size();
    if (!updating) {
        beginInsertRows(index(size),  size, (size+1));
    }
    Room r;
    r.id = roomID;
    r.name = roomName;
    r.selected = selected;
    m_roomsList[roomName] = r;

    if (updating) {
        //Figure out a better way to update just the really changed message, not EVERYTHING
        emit dataChanged(createIndex(1, 1), createIndex(rowCount(), 1));
    } else {
        endInsertRows();
    }
    
    UserData::self()->getModelForRoom(r.id);
}


// #include "roommodel.moc"
