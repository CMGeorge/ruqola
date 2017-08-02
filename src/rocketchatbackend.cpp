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
#include "ruqola_debug.h"
#include "ruqola.h"
#include "ddpclient.h"

void process_backlog(const QJsonDocument &messages)
{
    qCDebug(RUQOLA_LOG) << messages.object().value(QStringLiteral("messages")).toArray().size();
    RocketChatBackend::processIncomingMessages(messages.object().value(QStringLiteral("messages")).toArray());
}


void rooms_callback(const QJsonDocument &doc)
{
    RoomModel *model = Ruqola::self()->roomModel();
    
    //QJsonArray removed = doc.object().value(QStringLiteral("remove")).toArray();
    const QJsonArray updated = doc.object().value(QStringLiteral("update")).toArray();
    
    for (int i = 0; i < updated.size(); i++) {
        QJsonObject room = updated.at(i).toObject();
    
        if (room.value(QStringLiteral("t")).toString() != QLatin1String("d")) {

            QString roomID = room.value(QStringLiteral("_id")).toString();
            MessageModel *roomModel = Ruqola::self()->getModelForRoom(roomID);
            
            // let's be extra safe around crashes
            if (Ruqola::self()->loginStatus() == DDPClient::LoggedIn) {
                Room r;
                r.id = roomID;
                r.name = room[QStringLiteral("name")].toString();
                r.topic = room[QStringLiteral("topic")].toString();
                                
                qCDebug(RUQOLA_LOG) << "Adding room" << r.name << r.id << r.topic;

                model->addRoom(r);
            }
            
            QJsonArray params;
            params.append(QJsonValue(roomID));
            Ruqola::self()->ddp()->subscribe(QStringLiteral("stream-room-messages"), params);

            // Load history
            params.append(QJsonValue(QJsonValue::Null));
            params.append(QJsonValue(50)); // Max number of messages to load;
            QJsonObject dateObject;
            dateObject[QStringLiteral("$date")] = QJsonValue(roomModel->lastTimestamp());
            params.append(dateObject);
            Ruqola::self()->ddp()->method(QStringLiteral("loadHistory"), QJsonDocument(params), process_backlog);
        }
    } 
}

void subs_callback(const QJsonDocument &doc)
{
    RoomModel *model = Ruqola::self()->roomModel();

    //QJsonArray removed = doc.object().value(QStringLiteral("remove")).toArray();
    const QJsonArray updated = doc.object().value(QStringLiteral("update")).toArray();
    
    for (int i = 0; i < updated.size(); i++) {
        QJsonObject room = updated.at(i).toObject();
    
        if (room.value(QStringLiteral("t")).toString() != QLatin1String("d")) {

            QString roomID = room.value(QStringLiteral("rid")).toString();
            MessageModel *roomModel = Ruqola::self()->getModelForRoom(roomID);
            
            // let's be extra safe around crashes
            if (Ruqola::self()->loginStatus() == DDPClient::LoggedIn) {
                Room r;
                r.id = roomID;
                r.name = room[QStringLiteral("name")].toString();
                r.topic = room[QStringLiteral("topic")].toString();
                                
//                 qCDebug(RUQOLA_LOG) << "Adding room" << r.name << r.id << r.topic;
                
                model->addRoom(r);
            }
            
            
            QJsonArray params;
            params.append(QJsonValue(roomID));
            Ruqola::self()->ddp()->subscribe(QStringLiteral("stream-room-messages"), params);

            // Load history
            params.append(QJsonValue(QJsonValue::Null));
            params.append(QJsonValue(50)); // Max number of messages to load;
            QJsonObject dateObject;
            dateObject[QStringLiteral("$date")] = QJsonValue(roomModel->lastTimestamp());
            params.append(dateObject);
            Ruqola::self()->ddp()->method(QStringLiteral("loadHistory"), QJsonDocument(params), process_backlog);
        }
    } 
}

void RocketChatBackend::processIncomingMessages(const QJsonArray &messages)
{
    for (const QJsonValue &v : messages) {
        QJsonObject o = v.toObject();
        
        Message m;
        QString roomId = o.value(QStringLiteral("rid")).toString();
        QString type = o.value(QStringLiteral("t")).toString();

        m.messageID = o.value(QStringLiteral("_id")).toString();
        m.roomID = roomId;
        m.message = o.value(QStringLiteral("msg")).toString();
        m.timestamp = (qint64)o.value(QStringLiteral("ts")).toObject().value(QStringLiteral("$date")).toDouble();
        m.username = o.value(QStringLiteral("u")).toObject().value(QStringLiteral("username")).toString();
        m.userID = o.value(QStringLiteral("u")).toObject().value(QStringLiteral("_id")).toString();
        m.updatedAt = o.value(QStringLiteral("_updatedAt")).toObject().value(QStringLiteral("$date")).toDouble();
        m.editedAt = o.value(QStringLiteral("editedAt")).toObject().value(QStringLiteral("$date")).toDouble();
        m.editedByUsername = o.value(QStringLiteral("editedBy")).toObject().value(QStringLiteral("username")).toString();
        m.editedByUserID = o.value(QStringLiteral("editedBy")).toObject().value(QStringLiteral("userID")).toString();
        m.url = o.value(QStringLiteral("urls")).toObject().value(QStringLiteral("url")).toString();
        m.meta = o.value(QStringLiteral("urls")).toObject().value(QStringLiteral("meta")).toString();
        m.headers = o.value(QStringLiteral("urls")).toObject().value(QStringLiteral("headers")).toString();
        m.parsedUrl = o.value(QStringLiteral("urls")).toObject().value(QStringLiteral("parsedUrl")).toString();
        m.imageUrl = o.value(QStringLiteral("attachments")).toObject().value(QStringLiteral("image_url")).toString();
        m.color = o.value(QStringLiteral("attachments")).toObject().value(QStringLiteral("color")).toString();
        m.alias = o.value(QStringLiteral("alias")).toString();
        m.avatar = o.value(QStringLiteral("avatar")).toString();
        m.groupable = o.value(QStringLiteral("groupable")).toBool();
        m.parseUrls = o.value(QStringLiteral("parseUrls")).toBool();

        if (!type.isEmpty()) {
            m.systemMessage = true;
            m.systemMessageType = type;
        } else {
            m.systemMessage = false;
        }
        
        Ruqola::self()->getModelForRoom(roomId)->addMessage(m);
    }
}

RocketChatBackend::RocketChatBackend(QObject* parent)
 : QObject(parent)
{
        connect(Ruqola::self(), &Ruqola::loginStatusChanged, this, &RocketChatBackend::onLoginStatusChanged);
        connect(Ruqola::self(), &Ruqola::userIDChanged, this, &RocketChatBackend::onUserIDChanged);
        connect(Ruqola::self()->ddp(), &DDPClient::changed, this, &RocketChatBackend::onChanged);
        connect(Ruqola::self()->ddp(), &DDPClient::added, this, &RocketChatBackend::onAdded);
}

RocketChatBackend::~RocketChatBackend()
{
}

void RocketChatBackend::onLoginStatusChanged()
{
    if (Ruqola::self()->loginStatus() == DDPClient::LoggedIn) {
//         qCDebug(RUQOLA_LOG) << "GETTING LIST OF ROOMS";
        QJsonObject params;
        params[QStringLiteral("$date")] = QJsonValue(0); // get ALL rooms we've ever seen
        Ruqola::self()->ddp()->method(QStringLiteral("rooms/get"), QJsonDocument(params), rooms_callback);
    }
}

void RocketChatBackend::onLoggedIn()
{

}

void RocketChatBackend::onAdded(const QJsonObject &object)
{
    QString collection = object.value(QStringLiteral("collection")).toString();
        
    if (collection == QLatin1String("stream-room-messages")) {
        
    } else if (collection == QLatin1String("users")) {
        
        QJsonObject fields =  object.value(QStringLiteral("fields")).toObject();
        QString username = fields.value(QStringLiteral("username")).toString();
        if(username == Ruqola::self()->userName()){
            Ruqola::self()->setUserID(object[QStringLiteral("id")].toString());
            qCDebug(RUQOLA_LOG) << "User id set to " << Ruqola::self()->userID();
        }

         qCDebug(RUQOLA_LOG) << "NEW USER ADDED: " << username;
        
    } else if (collection == QLatin1String("rooms")) {

    } else if (collection == QLatin1String("stream-notify-user")){

    }
}


void RocketChatBackend::onChanged(const QJsonObject &object)
{
    const QString collection = object[QStringLiteral("collection")].toString();

    if (collection == QLatin1String("stream-room-messages")) {
        QJsonObject fields = object.value(QStringLiteral("fields")).toObject();
        //QString roomId = fields.value(QStringLiteral("eventName")).toString();
        QJsonArray contents = fields.value(QStringLiteral("args")).toArray();
        processIncomingMessages(contents);        

    } else if (collection == QLatin1String("users")) {
        qCDebug(RUQOLA_LOG) << "USER CHANGED";
        
    } else if (collection == QLatin1String("rooms")) {

    } else if (collection == QLatin1String("stream-notify-user")) {
        QJsonObject fields = object.value(QStringLiteral("fields")).toObject();
        QJsonArray contents = fields.value(QStringLiteral("args")).toArray();
        QString message = contents.at(0).toObject()[QStringLiteral("text")].toString();
        Ruqola::self()->notification()->showMessage(tr("New message"), message, QSystemTrayIcon::Information, 5000 );
        qCDebug(RUQOLA_LOG) << "New notification" << object.value(QStringLiteral("fields")).toObject();
    }
}

void RocketChatBackend::onUserIDChanged()
{
    qCDebug(RUQOLA_LOG) << "subscribing to notification feed";
    QJsonArray params;
    params.append(QJsonValue(QStringLiteral("%1/%2").arg(Ruqola::self()->userID()).arg(QStringLiteral("notification"))));
    Ruqola::self()->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
}

