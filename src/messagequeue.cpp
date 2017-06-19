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

#include "ruqola.h"
#include "ddpclient.h"

QPair<QString,QJsonDocument> MessageQueue::fromJson(const QJsonObject &o)
{
    QPair<QString,QJsonDocument> pair;

    pair.first = o["method"].toString();
    QJsonArray arr = o["params"].toArray();
    pair.second = QJsonDocument(arr);
    return pair;
}


QByteArray MessageQueue::serialize(const QPair<QString,QJsonDocument> pair)
{
    QJsonDocument d;
    QJsonObject o;

    o["method"] = QJsonValue(pair.first);

    QJsonArray arr;
    if ( pair.second.isArray() ){
        arr.append(pair.second.array());
    } else if ( pair.second.isObject() ) {
        arr.append(pair.second.object());
    }

    o["params"] = QJsonValue(arr);

    d.setObject(o);
    return d.toBinaryData();
}


MessageQueue::MessageQueue()
{
    connect(Ruqola::self()->ddp(), &DDPClient::loginStatusChanged, this, &MessageQueue::onLoginStatusChanged);
    QDir cacheDir(Ruqola::self()->ddp()->cachePath());

    // load unsent messages cache
    if (QFile::exists(cacheDir.absoluteFilePath("QueueCache"))) {
        QFile f(cacheDir.absoluteFilePath("QueueCache"));
        if (f.open(QIODevice::ReadOnly)) {
            QDataStream in(&f);
            while (!f.atEnd()) {
                char * byteArray;
                quint32 length;
                in.readBytes(byteArray, length);
                QByteArray ba = QByteArray::fromRawData(byteArray, length);
                QPair<QString,QJsonDocument> pair = MessageQueue::fromJson(QJsonDocument::fromBinaryData(ba).object());

                QString method = pair.first;
                QJsonDocument params = pair.second;
                Ruqola::self()->ddp()->messageQueue().enqueue(qMakePair(method,params));
            }
        }
    }
}

MessageQueue::~MessageQueue()
{
    QDir cacheDir(Ruqola::self()->ddp()->cachePath());
    qDebug() << "Caching Unsent messages to... " << cacheDir.path();
    if (!cacheDir.exists(cacheDir.path())) {
        cacheDir.mkpath(cacheDir.path());
    }
    QFile f(cacheDir.absoluteFilePath("QueueCache"));
    if (f.open(QIODevice::WriteOnly)) {
        QDataStream out(&f);

        QQueue<QPair<QString,QJsonDocument>>::iterator it;
        QQueue<QPair<QString,QJsonDocument>> queue = Ruqola::self()->ddp()->messageQueue();

        for ( it = queue.begin(); it != queue.end(); it++ ) {
            QPair<QString,QJsonDocument> pair = *it;
            QByteArray ba = serialize(pair);
            out.writeBytes(ba, ba.size());
        }
    }
}


void MessageQueue::onLoginStatusChanged()
{
    if (Ruqola::self()->loginStatus() == DDPClient::LoggedIn && !Ruqola::self()->ddp()->messageQueue().empty()){
        //retry sending messages
        processQueue();
    }
}


void MessageQueue::processQueue()
{
    while ( Ruqola::self()->loginStatus() == DDPClient::LoggedIn && !Ruqola::self()->ddp()->messageQueue().empty() ){
            QPair<QString,QJsonDocument> pair = Ruqola::self()->ddp()->messageQueue().head();
            QString method = pair.first;
            QJsonDocument params = pair.second;
            Ruqola::self()->ddp()->method(method, params); //can be optimized using single shot timer
    }
}
