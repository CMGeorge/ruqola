/*

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
#include "ruqola_debug.h"
#include "rocketchataccount.h"
#include "usersforroommodel.h"
#include "roomwrapper.h"
#include <KLocalizedString>

#include <QIcon>
#include <QDir>
#include <QFile>
#include <QJsonArray>

RoomModel::RoomModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mRocketChatAccount(account)
{
}

RoomModel::~RoomModel()
{
    if (mRocketChatAccount) {
        const QString cachePath = mRocketChatAccount->settings()->cacheBasePath();
        if (cachePath.isEmpty()) {
            qCWarning(RUQOLA_LOG) << " Cache Path is not defined";
            return;
        }
        QDir cacheDir(cachePath);
        if (!cacheDir.exists(cacheDir.path())) {
            cacheDir.mkpath(cacheDir.path());
        }

        QFile f(cacheDir.absoluteFilePath(QStringLiteral("rooms")));

        if (f.open(QIODevice::WriteOnly)) {
            QDataStream out(&f);
            for (Room *m : qAsConst(mRoomsList)) {
                qCDebug(RUQOLA_LOG) << " save cache for room " << m->name();
                const QByteArray ms = Room::serialize(m);
                out.writeBytes(ms, ms.size());
            }
        }
    }
    qDeleteAll(mRoomsList);
}

void RoomModel::clear()
{
    if (!mRoomsList.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        qDeleteAll(mRoomsList);
        mRoomsList.clear();
        endRemoveRows();
    }
}

Room *RoomModel::findRoom(const QString &roomID) const
{
    foreach (Room *r, qAsConst(mRoomsList)) {
        if (r->roomId() == roomID) {
            return r;
        }
    }
    return nullptr;
}

RoomWrapper *RoomModel::findRoomWrapper(const QString &roomID) const
{
    foreach (Room *r, qAsConst(mRoomsList)) {
        if (r->roomId() == roomID) {
            RoomWrapper *wrapper = new RoomWrapper(r);
            return wrapper;
        }
    }
    return nullptr;
}

// Clear data and refill it with data in the cache, if there is
void RoomModel::reset()
{
    clear();

    if (!mRocketChatAccount) {
        return;
    }
    if (mRocketChatAccount->settings()->cacheBasePath().isEmpty()) {
        return;
    }

    //Laurent disable cache for the moment
/*
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
                Room m = Room::fromJSon(QJsonDocument::fromBinaryData(arr).object());
                qDebug() <<" Load from cache room name: " << m.name;
                addRoom(m.id, m.name, m.selected);
            }
        }
        qCDebug(RUQOLA_LOG) << "Cache Loaded";
    }
    */
}

QHash<int, QByteArray> RoomModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RoomName] = QByteArrayLiteral("name");
    roles[RoomID] = QByteArrayLiteral("room_id");
    roles[RoomSelected] = QByteArrayLiteral("selected");
    roles[RoomUnread] = QByteArrayLiteral("unread");
    roles[RoomType] = QByteArrayLiteral("type");
    roles[RoomUserName] = QByteArrayLiteral("username");
    roles[RoomUserID] = QByteArrayLiteral("userID");
    roles[RoomTopic] = QByteArrayLiteral("topic");
    roles[RoomMutedUsers] = QByteArrayLiteral("mutedUsers");
    roles[RoomJitsiTimeout] = QByteArrayLiteral("jitsiTimeout");
    roles[RoomRo] = QByteArrayLiteral("readOnly");
    roles[RoomAnnoucement] = QByteArrayLiteral("announcement");
    roles[RoomOpen] = QByteArrayLiteral("open");
    roles[RoomAlert] = QByteArrayLiteral("alert");
    roles[RoomOrder] = QByteArrayLiteral("roomorder");
    roles[RoomFavorite] = QByteArrayLiteral("favorite");
    roles[RoomSection] = QByteArrayLiteral("sectionname");
    roles[RoomIcon] = QByteArrayLiteral("channelicon");
    roles[RoomUserMentions] = QByteArrayLiteral("userMentions");
    return roles;
}

int RoomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mRoomsList.size();
}

QVariant RoomModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mRoomsList.count()) {
        return QVariant();
    }

    Room *r = mRoomsList.at(index.row());

    switch (role) {
    case RoomModel::RoomName:
        return r->name();
    case RoomModel::RoomID:
        return r->roomId();
    case RoomModel::RoomSelected:
        return r->selected();
    case RoomModel::RoomType:
        return r->channelType();
    case RoomModel::RoomUserID:
        return r->roomCreatorUserId();
    case RoomModel::RoomUserName:
        return r->roomCreatorUserName();
    case RoomModel::RoomTopic:
        return r->topic();
    case RoomModel::RoomMutedUsers:
        return r->mutedUsers();
    case RoomModel::RoomJitsiTimeout:
        return r->jitsiTimeout();
    case RoomModel::RoomRo:
        return r->readOnly();
    case RoomModel::RoomAnnoucement:
        return r->announcement();
    case RoomModel::RoomUnread:
        return r->unread();
    case RoomModel::RoomOpen:
        return r->open();
    case RoomModel::RoomAlert:
        return r->alert();
    case RoomModel::RoomFavorite:
        return r->favorite();
    case RoomModel::RoomSection:
        return sectionName(r);
    case RoomModel::RoomOrder:
        return order(r);
    case RoomModel::RoomIcon:
        return icon(r);
    case RoomModel::RoomOtr:
        //TODO implement it.
        return {};
    case RoomModel::RoomUserMentions:
        return r->userMentions();
    }
    return {};
}

void RoomModel::addRoom(const QString &roomID, const QString &roomName, bool selected)
{
    if (roomID.isEmpty() || roomName.isEmpty()) {
        qCDebug(RUQOLA_LOG) << " Impossible to add a room";
        return;
    }
    qCDebug(RUQOLA_LOG) << "Adding room : roomId: " << roomID << " room Name " << roomName << " isSelected : " << selected;

    Room *r = createNewRoom();
    r->setRoomId(roomID);
    r->setName(roomName);
    r->setSelected(selected);
    addRoom(r);
}

Room *RoomModel::createNewRoom()
{
    Room *r = new Room(mRocketChatAccount);
    connect(r, &Room::alertChanged, this, &RoomModel::needToUpdateNotification);
    connect(r, &Room::unreadChanged, this, &RoomModel::needToUpdateNotification);
    return r;
}

void RoomModel::getUnreadAlertFromAccount(bool &hasAlert, int &nbUnread)
{
    for (int i = 0; i < mRoomsList.count(); ++i) {
        if (mRoomsList.at(i)->open()) {
            if (mRoomsList.at(i)->alert()) {
                hasAlert = true;
            }
            nbUnread += mRoomsList.at(i)->unread();
        }
    }
}

void RoomModel::updateSubscriptionRoom(const QJsonObject &roomData)
{
    //TODO fix me!
    //Use "_id"
    QString rId = roomData.value(QLatin1String("rid")).toString();
    if (rId.isEmpty()) {
        rId = roomData.value(QLatin1String("_id")).toString();
    }
    if (!rId.isEmpty()) {
        const int roomCount = mRoomsList.size();
        for (int i = 0; i < roomCount; ++i) {
            if (mRoomsList.at(i)->roomId() == rId) {
                qCDebug(RUQOLA_LOG) << " void RoomModel::updateSubscriptionRoom(const QJsonArray &array) room found";
                Room *room = mRoomsList.at(i);
                room->updateSubscriptionRoom(roomData);
                Q_EMIT dataChanged(createIndex(i, 0), createIndex(i, 0));

                break;
            }
        }
    } else {
        //qCWarning(RUQOLA_LOG) << "RoomModel::updateRoom incorrect jsonobject "<< roomData;
        qWarning() << "RoomModel::updateSubscriptionRoom incorrect jsonobject "<< roomData;
    }
}

QString RoomModel::insertRoom(const QJsonObject &room)
{
    Room *r = createNewRoom();
    r->parseInsertRoom(room);
    qCDebug(RUQOLA_LOG) << "Inserting room" << r->name() << r->roomId() << r->topic();
    addRoom(r);
    return r->roomId();
}

Room *RoomModel::addRoom(const QJsonObject &room)
{
    Room *r = createNewRoom();
    r->parseSubscriptionRoom(room);
    qCDebug(RUQOLA_LOG) << "Adding room subscription" << r->name() << r->roomId() << r->topic();
    addRoom(r);
    return r;
}

void RoomModel::addRoom(Room *room)
{
    qCDebug(RUQOLA_LOG) << " void RoomModel::addRoom(const Room &room)"<<room->name();
    int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->roomId() == room->roomId()) {
            delete mRoomsList.takeAt(i);
            break;
        }
    }
    roomCount = mRoomsList.count();

    beginInsertRows(QModelIndex(), roomCount, roomCount);
    qCDebug(RUQOLA_LOG) << "Inserting room at position" <<roomCount;
    mRoomsList.append(room);
    endInsertRows();
}

void RoomModel::removeRoom(const QString &roomId)
{
    //TODO
}

void RoomModel::updateSubscription(const QJsonArray &array)
{
    const QString actionName = array[0].toString();
    const QJsonObject roomData = array[1].toObject();
    if (actionName == QStringLiteral("removed")) {
        qCDebug(RUQOLA_LOG) << "REMOVE ROOM name " << roomData.value(QLatin1String("name")) << " rid " << roomData.value(QLatin1String("rid"));
        const QString id = roomData.value(QLatin1String("rid")).toString();
        const int roomCount = mRoomsList.count();
        for (int i = 0; i < roomCount; ++i) {
            if (mRoomsList.at(i)->roomId() == id) {
                beginRemoveRows(QModelIndex(), i, i);
                mRoomsList.remove(i);
                endRemoveRows();
                break;
            }
        }
    } else if (actionName == QStringLiteral("inserted")) {
        qCDebug(RUQOLA_LOG) << "INSERT ROOM  name " << roomData.value(QLatin1String("name")) << " rid " << roomData.value(QLatin1String("rid"));
        //TODO fix me!
        addRoom(roomData);
        //addRoom(roomData.value(QLatin1String("rid")).toString(), roomData.value(QLatin1String("name")).toString(), false);
    } else if (actionName == QStringLiteral("updated")) {
        //qCDebug(RUQOLA_LOG) << "UPDATE ROOM name " << roomData.value(QLatin1String("name")).toString() << " rid " << roomData.value(QLatin1String("rid")) << " roomData " << roomData;
        qDebug() << "UPDATE ROOM name " << roomData.value(QLatin1String("name")).toString() << " rid " << roomData.value(QLatin1String("rid")) << " roomData " << roomData;
        updateSubscriptionRoom(roomData);
    } else if (actionName == QStringLiteral("changed")) {
        //qDebug() << "CHANGED ROOM name " << roomData.value(QLatin1String("name")).toString() << " rid " << roomData.value(QLatin1String("rid")) << " roomData " << roomData;
        qCDebug(RUQOLA_LOG) << "CHANGED ROOM name " << roomData.value(QLatin1String("name")).toString() << " rid " << roomData.value(QLatin1String("rid")) << " roomData " << roomData;
        qCDebug(RUQOLA_LOG) << "RoomModel::updateSubscription Not implementer changed room yet" << array;
        updateRoom(roomData);
    } else {
        qCDebug(RUQOLA_LOG) << "RoomModel::updateSubscription Undefined type" << actionName;
    }
}

void RoomModel::updateRoom(const QJsonObject &roomData)
{
    qCDebug(RUQOLA_LOG) << " void RoomModel::updateRoom(const QJsonObject &roomData)";
    //TODO fix me!
    //Use "_id"
    QString rId = roomData.value(QLatin1String("rid")).toString();
    if (rId.isEmpty()) {
        rId = roomData.value(QLatin1String("_id")).toString();
    }
    if (!rId.isEmpty()) {
        const int roomCount = mRoomsList.size();
        for (int i = 0; i < roomCount; ++i) {
            if (mRoomsList.at(i)->roomId() == rId) {
                qCDebug(RUQOLA_LOG) << " void RoomModel::updateRoom(const QJsonArray &array) room found";
                Room *room = mRoomsList.at(i);
                room->parseUpdateRoom(roomData);
                Q_EMIT dataChanged(createIndex(i, 0), createIndex(i, 0));

                break;
            }
        }
    } else {
        //qCWarning(RUQOLA_LOG) << "RoomModel::updateRoom incorrect jsonobject "<< roomData;
        qWarning() << "RoomModel::updateRoom incorrect jsonobject "<< roomData;
    }
}

void RoomModel::userStatusChanged(const User &user)
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        Room *room = mRoomsList.at(i);
        if (room->name() == user.userName()) {
            const QModelIndex idx = createIndex(i, 0);
            Q_EMIT dataChanged(idx, idx);
        }
        room->usersModelForRoom()->userStatusChanged(user);
    }
}

UsersForRoomModel *RoomModel::usersModelForRoom(const QString &roomId) const
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->roomId() == roomId) {
            return mRoomsList.at(i)->usersModelForRoom();
        }
    }
    qCWarning(RUQOLA_LOG) << " Users model for room undefined !";
    return nullptr;
}

UsersForRoomFilterProxyModel *RoomModel::usersForRoomFilterProxyModel(const QString &roomId) const
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->roomId() == roomId) {
            return mRoomsList.at(i)->usersModelForRoomProxyModel();
        }
    }
    return {};
}

FilesForRoomFilterProxyModel *RoomModel::filesForRoomFilterProxyModel(const QString &roomId) const
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->roomId() == roomId) {
            return mRoomsList.at(i)->filesForRoomFilterProxyModel();
        }
    }
    return {};
}

MessageModel *RoomModel::messageModel(const QString &roomId) const
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->roomId() == roomId) {
            return mRoomsList.at(i)->messageModel();
        }
    }
    return {};
}

FilesForRoomModel *RoomModel::filesModelForRoom(const QString &roomId) const
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->roomId() == roomId) {
            return mRoomsList.at(i)->filesModelForRoom();
        }
    }
    return {};
}

QString RoomModel::inputMessage(const QString &roomId) const
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->roomId() == roomId) {
            return mRoomsList.at(i)->inputMessage();
        }
    }
    return {};
}

void RoomModel::setInputMessage(const QString &roomId, const QString &inputMessage)
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->roomId() == roomId) {
            return mRoomsList.at(i)->setInputMessage(inputMessage);
        }
    }
}

QString RoomModel::sectionName(Room *r) const
{
    QString str;
    if (r->favorite()) {
        str = i18n("Favorites");
    } else {
        const QString channelTypeStr = r->channelType();
        if (channelTypeStr == QLatin1String("c") || channelTypeStr == QLatin1String("p")) {
            str = i18n("Rooms");
        } else if (channelTypeStr == QLatin1String("d")) {
            str = i18n("Private Message");
        }
    }
    return str;
}

int RoomModel::order(Room *r) const
{
    int order = 0;
    //First item are favorites channels
    if (!r->favorite()) {
        order += 3;
    }
    const QString channelTypeStr = r->channelType();
    if (channelTypeStr == QLatin1String("c") || channelTypeStr == QLatin1String("p")) {
        order += 1;
    } else if (channelTypeStr == QLatin1String("d")) {
        order += 2;
    } else {
        qCDebug(RUQOLA_LOG) << r->name() << "has unhandled channel type" << channelTypeStr;
        order += 3;
    }
    return order;
}

QIcon RoomModel::icon(Room *r) const
{
    if (r->channelType() == QLatin1String("c")) {
        if (r->unread() > 0 || r->alert()) {
            return QIcon::fromTheme(QStringLiteral("irc-channel-active"));
        } else {
            return QIcon::fromTheme(QStringLiteral("irc-channel-inactive"));
        }
    } else if (r->channelType() == QLatin1String("d")) {
        const QString userStatusIconFileName = mRocketChatAccount ? mRocketChatAccount->userStatusIconFileName(r->name()) : QString();
        if (userStatusIconFileName.isEmpty()) {
            return QIcon::fromTheme(QStringLiteral("user-avaliable"));
        } else {
            return QIcon::fromTheme(userStatusIconFileName);
        }
    } else if (r->channelType() == QLatin1String("p")) {
        return QIcon::fromTheme(QStringLiteral("lock"));
    }
    return {};
}
