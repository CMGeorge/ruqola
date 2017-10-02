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

#include "messagemodelgui.h"

#include <roomfilterproxymodel.h>
#include <message.h>
#include "messagemodel.h"

#include <KLocalizedContext>

#include <QApplication>
#include <QStandardPaths>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QHBoxLayout>
#include <ruqolautils.h>
#include <ruqola.h>
#include <rocketchataccount.h>

MessageModelGui::MessageModelGui(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout(this);



    fillModel();
    qmlRegisterSingletonType(QUrl(QStringLiteral("qrc:/ExtraColors.qml")), "KDE.Ruqola.ExtraColors", 1, 0, "RuqolaSingleton");
    qmlRegisterSingletonType<RuqolaUtils>("KDE.Ruqola.RuqolaUtils", 1, 0, "RuqolaUtils", ruqolautils_singletontype_provider);
    qmlRegisterSingletonType<Ruqola>("KDE.Ruqola.Ruqola", 1, 0, "Ruqola", ruqola_singletontype_provider);
    qmlRegisterType<RoomFilterProxyModel>("KDE.Ruqola.RoomFilterProxyModel", 1, 0, "RoomFilterProxyModel");
    qmlRegisterType<RocketChatAccount>("KDE.Ruqola.RocketChatAccount", 1, 0, "RocketChatAccount");
    qRegisterMetaType<Message::MessageType>();
    qmlRegisterUncreatableType<Message>("KDE.Ruqola.Message", 1, 0, "Message", QStringLiteral("MessageType is an enum container"));
    mEngine = new QQmlApplicationEngine;

    QQmlContext *ctxt = mEngine->rootContext();
    ctxt->setContextObject(new KLocalizedContext(mEngine));
    ctxt->setContextProperty(QStringLiteral("messageModel"), mModel);

    mEngine->load(QUrl(QStringLiteral("qrc:/messagemodelgui.qml")));

    if (!mEngine->rootObjects().isEmpty()) {
        //TODO create widget
    }
}

void MessageModelGui::fillModel()
{
    mIndexMessage = 1;
    mModel = new MessageModel();
    Message m1;
    m1.setMessageType(Message::MessageType::NormalText);
    m1.setMessageId(QString::number(mIndexMessage++));
    m1.setText(QStringLiteral("foo"));
    m1.setUsername(QStringLiteral("blabla"));
    m1.setTimeStamp(QDateTime(QDate(2017, 03, 03), QTime(11, 30, 00)).toMSecsSinceEpoch());
    m1.setAlias(QStringLiteral("bla"));

    mModel->addMessage(m1);
    Message m2;
    m2.setMessageType(Message::MessageType::NormalText);
    m2.setMessageId(QString::number(mIndexMessage++));
    m2.setText(QStringLiteral("bla bla bla bla bla bla"));
    m2.setTimeStamp(QDateTime(QDate(2017, 03, 03), QTime(11, 31, 00)).toMSecsSinceEpoch());
    m2.setUsername(QStringLiteral("blo"));
    m2.setAlias(QStringLiteral("bla"));
    mModel->addMessage(m2);

    Message m3;
    m3.setMessageType(Message::MessageType::NormalText);
    m3.setMessageId(QString::number(mIndexMessage++));
    m3.setText(QStringLiteral("next day"));
    m3.setTimeStamp(QDateTime(QDate(2017, 03, 04), QTime(11, 31, 00)).toMSecsSinceEpoch());
    m3.setUsername(QStringLiteral("blo"));
    m3.setAlias(QStringLiteral("bla"));
    mModel->addMessage(m3);

    //System message
    {
        Message m4;
        m4.setMessageType(Message::MessageType::System);
        m4.setMessageId(QString::number(mIndexMessage++));
        m4.setText(QStringLiteral("uj"));
        m4.setTimeStamp(QDateTime(QDate(2017, 03, 05), QTime(11, 31, 00)).toMSecsSinceEpoch());
        m4.setUsername(QStringLiteral("blo"));
        m4.setAlias(QStringLiteral("bla"));
        m4.setSystemMessageType(QStringLiteral("uj"));
        mModel->addMessage(m4);
    }
    {
        Message m4;
        m4.setMessageType(Message::MessageType::System);
        m4.setMessageId(QString::number(mIndexMessage++));
        m4.setText(QStringLiteral("ul"));
        m4.setTimeStamp(QDateTime(QDate(2017, 03, 05), QTime(11, 31, 00)).toMSecsSinceEpoch());
        m4.setUsername(QStringLiteral("blo"));
        m4.setAlias(QStringLiteral("bla"));
        m4.setSystemMessageType(QStringLiteral("ul"));
        mModel->addMessage(m4);
    }
    {
        Message m4;
        m4.setMessageType(Message::MessageType::System);
        m4.setMessageId(QString::number(mIndexMessage++));
        m4.setText(QStringLiteral("room_changed_topic"));
        m4.setTimeStamp(QDateTime(QDate(2017, 03, 05), QTime(11, 31, 00)).toMSecsSinceEpoch());
        m4.setUsername(QStringLiteral("blo"));
        m4.setAlias(QStringLiteral("bla"));
        m4.setSystemMessageType(QStringLiteral("room_changed_topic"));
        mModel->addMessage(m4);
    }
    {
        Message m4;
        m4.setMessageType(Message::MessageType::System);
        m4.setMessageId(QString::number(mIndexMessage++));
        m4.setText(QStringLiteral("room_changed_topic"));
        m4.setTimeStamp(QDateTime(QDate(2017, 03, 05), QTime(11, 31, 00)).toMSecsSinceEpoch());
        m4.setUsername(QStringLiteral("blo"));
        m4.setAlias(QStringLiteral("bla"));
        m4.setSystemMessageType(QStringLiteral("au"));
        mModel->addMessage(m4);
    }
    {
        Message m4;
        m4.setMessageType(Message::MessageType::System);
        m4.setMessageId(QString::number(mIndexMessage++));
        m4.setText(QStringLiteral("room_changed_topic"));
        m4.setTimeStamp(QDateTime(QDate(2017, 03, 05), QTime(11, 31, 00)).toMSecsSinceEpoch());
        m4.setUsername(QStringLiteral("blo"));
        m4.setAlias(QStringLiteral("bla"));
        m4.setSystemMessageType(QStringLiteral("r"));
        mModel->addMessage(m4);
    }
    {
        Message m4;
        m4.setMessageType(Message::MessageType::System);
        m4.setMessageId(QString::number(mIndexMessage++));
        m4.setText(QStringLiteral("room_changed_topic"));
        m4.setTimeStamp(QDateTime(QDate(2017, 03, 05), QTime(11, 31, 00)).toMSecsSinceEpoch());
        m4.setUsername(QStringLiteral("blo"));
        m4.setAlias(QStringLiteral("bla"));
        m4.setSystemMessageType(QStringLiteral("room_changed_description"));
        mModel->addMessage(m4);
    }
    {
        Message m4;
        m4.setMessageType(Message::MessageType::System);
        m4.setMessageId(QString::number(mIndexMessage++));
        m4.setText(QStringLiteral("room_changed_topic"));
        m4.setTimeStamp(QDateTime(QDate(2017, 03, 05), QTime(11, 31, 00)).toMSecsSinceEpoch());
        m4.setUsername(QStringLiteral("blo"));
        m4.setAlias(QStringLiteral("bla"));
        m4.setSystemMessageType(QStringLiteral("room_changed_privacy"));
        mModel->addMessage(m4);
    }
    {
        Message m4;
        m4.setMessageType(Message::MessageType::System);
        m4.setMessageId(QString::number(mIndexMessage++));
        m4.setText(QStringLiteral("room_changed_topic"));
        m4.setTimeStamp(QDateTime(QDate(2017, 03, 05), QTime(11, 31, 00)).toMSecsSinceEpoch());
        m4.setUsername(QStringLiteral("blo"));
        m4.setAlias(QStringLiteral("bla"));
        m4.setSystemMessageType(QStringLiteral("sss"));
        mModel->addMessage(m4);
    }
    {
        Message m4;
        m4.setMessageType(Message::MessageType::System);
        m4.setMessageId(QString::number(mIndexMessage++));
        m4.setText(QStringLiteral("room_changed_topic"));
        m4.setTimeStamp(QDateTime(QDate(2017, 03, 05), QTime(11, 31, 00)).toMSecsSinceEpoch());
        m4.setUsername(QStringLiteral("blo"));
        m4.setAlias(QStringLiteral("bla"));
        m4.setSystemMessageType(QStringLiteral("ru"));
        mModel->addMessage(m4);
    }

    //Video
    {
        Message m4;
        m4.setMessageType(Message::MessageType::Video);
        m4.setMessageId(QString::number(mIndexMessage++));
        m4.setText(QStringLiteral("room_changed_topic"));
        m4.setTimeStamp(QDateTime(QDate(2017, 03, 05), QTime(11, 31, 00)).toMSecsSinceEpoch());
        m4.setUsername(QStringLiteral("blo"));
        m4.setAlias(QStringLiteral("bla"));
        mModel->addMessage(m4);
    }

    //Audio
    {
        Message m4;
        m4.setMessageType(Message::MessageType::Audio);
        m4.setMessageId(QString::number(mIndexMessage++));
        m4.setText(QStringLiteral("room_changed_topic"));
        m4.setTimeStamp(QDateTime(QDate(2017, 03, 05), QTime(11, 31, 00)).toMSecsSinceEpoch());
        m4.setUsername(QStringLiteral("blo"));
        m4.setAlias(QStringLiteral("bla"));
        mModel->addMessage(m4);
    }

    //ADD more
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);

    MessageModelGui w;
    w.show();
    return app.exec();
}
