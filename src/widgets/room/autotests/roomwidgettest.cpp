/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "roomwidgettest.h"
#include "room/roomwidget.h"
#include "room/messagelinewidget.h"
#include "room/roomheaderwidget.h"
#include "room/messagelistview.h"
#include "room/readonlylineeditwidget.h"
#include "room/usersinroomflowwidget.h"

#include <QStackedWidget>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(RoomWidgetTest)
RoomWidgetTest::RoomWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void RoomWidgetTest::shouldHaveDefaultValues()
{
    RoomWidget w;
    auto *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto *mRoomHeaderWidget = w.findChild<RoomHeaderWidget *>(QStringLiteral("mRoomHeaderWidget"));
    QVERIFY(mRoomHeaderWidget);

    UsersInRoomFlowWidget *mUsersInRoomFlowWidget = w.findChild<UsersInRoomFlowWidget *>(QStringLiteral("mUsersInRoomFlowWidget"));
    QVERIFY(mUsersInRoomFlowWidget);

    auto *mMessageListView = w.findChild<MessageListView *>(QStringLiteral("mMessageListView"));
    QVERIFY(mMessageListView);

    auto *mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);
    QCOMPARE(mStackedWidget->count(), 2);

    auto *mMessageLineWidget = w.findChild<MessageLineWidget *>(QStringLiteral("mMessageLineWidget"));
    QVERIFY(mMessageLineWidget);

    auto *mReadOnlyLineEditWidget = w.findChild<ReadOnlyLineEditWidget *>(QStringLiteral("mReadOnlyLineEditWidget"));
    QVERIFY(mReadOnlyLineEditWidget);
    QCOMPARE(mStackedWidget->currentWidget(), mMessageLineWidget);

    QVERIFY(w.roomId().isEmpty());
}
