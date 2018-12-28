/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "usersinfojobtest.h"
#include "users/usersinfojob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(UsersInfoJobTest)
using namespace RocketChatRestApi;
UsersInfoJobTest::UsersInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersInfoJobTest::shouldHaveDefaultValue()
{
    UsersInfoJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.identifier().isEmpty());
    QVERIFY(!job.useUserName());
}

void UsersInfoJobTest::shouldGenerateRequest()
{
    UsersInfoJob job;
    const QString roomId = QStringLiteral("foo1");
    job.setIdentifier(roomId);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    const QString identifier = QStringLiteral("avat");
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.info?userId=foo1")));
}

void UsersInfoJobTest::shouldGenerateRequestUsername()
{
    UsersInfoJob job;
    const QString roomId = QStringLiteral("foo1");
    job.setIdentifier(roomId);
    job.setUseUserName(true);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    const QString identifier = QStringLiteral("avat");
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.info?username=foo1")));
}
