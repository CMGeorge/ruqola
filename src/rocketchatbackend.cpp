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

#include "rocketchatbackend.h"
#include <QtCore>
#include <QJsonObject>
#include <QDebug>
#include "userdata.h"
#include "ddpclient.h"

void debug_callback(QJsonDocument doc)
{
    qDebug() << "DEBUG:" << doc;
}

void process_backlog(QJsonDocument messages)
{
    qDebug() << messages.object().value("messages").toArray().size();
    RocketChatBackend::processIncomingMessages(messages.object().value("messages").toArray());
}

void rooms_callback(QJsonDocument doc)
{
    RoomModel *model = UserData::instance()->roomModel();
    QJsonArray removed = doc.object().value("remove").toArray();
    QJsonArray updated = doc.object().value("update").toArray();
    
    for (int i = 0; i < updated.size(); i++) {
        QJsonObject room = updated.at(i).toObject();
    
//         qDebug() << "Room " << room.toObject().value("_id").toString();
        if (room.value("t").toString() == "c") {
            qDebug() << "Adding" << room.value("_id").toString()<< room.value("name").toString();
            model->addRoom(room.value("_id").toString(), room.value("name").toString());
            
            QString params = QString("[\"%1\"]").arg(room.value("_id").toString());
            UserData::instance()->ddp()->subscribe("stream-room-messages", QJsonDocument::fromJson(params.toLatin1()));
        
            // Load history
            QByteArray json = "[\""+room.value("_id").toString().toLatin1() +
                            "\", null, 50,{\"$date\": "+
                            QString::number(QDateTime::currentMSecsSinceEpoch()).toLatin1()+
                            "}]";
            qDebug() << json;
            UserData::instance()->ddp()->method("loadHistory", QJsonDocument::fromJson(json), process_backlog);
            
        }
    } 
//     qDebug() << "DEBUG:" << doc;
}

void RocketChatBackend::processIncomingMessages(QJsonArray messages)
{
    foreach (const QJsonValue v, messages) {
        QJsonObject o = v.toObject();
        
        Message m;
        QString roomId = o.value("rid").toString();
        QString type = o.value("t").toString();
        m.username = o.value("u").toObject().value("username").toString();
        m.userID = o.value("u").toObject().value("_id").toString();
        m.message = o.value("msg").toString();
        m.roomID = roomId;    
        m.timestamp = (qint64)o.value("ts").toObject().value("$date").toDouble();
        if (!type.isEmpty()) {
            m.systemMessage = true;
            m.systemMessageType = type;
        } else {
            m.systemMessage = false;
        }
        
        UserData::instance()->getModelForRoom(roomId)->addMessage(m);
        
    }
}

RocketChatBackend::RocketChatBackend(QObject* parent)
 : QObject(parent)
{
//     UserData::instance()->ddp() = new DDPClient(, this);
    connect(UserData::instance()->ddp(), &DDPClient::changed, this, &RocketChatBackend::onChanged);
    connect(UserData::instance()->ddp(), &DDPClient::added, this, &RocketChatBackend::onAdded);
    connect(UserData::instance()->ddp(), &DDPClient::loggedInChanged, this, &RocketChatBackend::onLoggedIn);
}

RocketChatBackend::~RocketChatBackend()
{
//     delete m_rooms;
//     delete UserData::instance()->ddp();
}

void RocketChatBackend::onLoggedIn()
{
    if (!UserData::instance()->ddp()->isLoggedIn()) {
        return;
    }
    qDebug() << "GETTING LIST OF ROOMS";
    // get list of rooms
    UserData::instance()->ddp()->method("rooms/get", QJsonDocument::fromJson("{\"$date\": 0}"), rooms_callback);
}

void RocketChatBackend::onAdded(QJsonObject object)
{
    QString collection = object.value("collection").toString();
    
    qDebug() << "ROCKET BACK" << object << collection;
    if (collection == "stream-room-messages") {
    
        
    } else if (collection == "users") {
        qDebug() << "NEW USER";
        
    } else if (collection == "rooms") {
    }
}

void RocketChatBackend::onChanged(QJsonObject object)
{
    QString collection = object.value("collection").toString();
    
    qDebug() << "ROCKET BACK" << object << collection;
    if (collection == "stream-room-messages") {
        QJsonObject fields = object.value("fields").toObject();
        QString roomId = fields.value("eventName").toString();
        QJsonArray contents = fields.value("args").toArray();
        RocketChatBackend::processIncomingMessages(contents);
        
    } else if (collection == "users") {
        qDebug() << "NEW USER";
        
    } else if (collection == "rooms") {
    }
}

