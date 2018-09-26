/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "addkeytochainjobtest.h"
#include "e2e/addkeytochainjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(AddKeyToChainJobTest)
using namespace RocketChatRestApi;
AddKeyToChainJobTest::AddKeyToChainJobTest(QObject *parent)
    : QObject(parent)
{
}

void AddKeyToChainJobTest::shouldHaveDefaultValue()
{
    AddKeyToChainJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.rsaPrivateKey().isEmpty());
    QVERIFY(job.rsaPublicKey().isEmpty());
}

void AddKeyToChainJobTest::shouldGenerateRequest()
{
    AddKeyToChainJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.favorite")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void AddKeyToChainJobTest::shouldGenerateJson()
{
    AddKeyToChainJob job;
    const QString rsapublic = QStringLiteral("foo1");
    job.setRsaPublicKey(rsapublic);
    const QString rsaprivate = QStringLiteral("private");
    job.setRsaPrivateKey(rsaprivate);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"favorite\":true,\"roomId\":\"%1\" %2}").arg(rsapublic).arg(rsaprivate).toLatin1());
}

void AddKeyToChainJobTest::shouldNotStarting()
{
    AddKeyToChainJob job;

    RestApiMethod *method = new RestApiMethod;
    method->setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(method);

    QNetworkAccessManager *mNetworkAccessManager = new QNetworkAccessManager;
    job.setNetworkAccessManager(mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString rsaprivate = QStringLiteral("foo1");
    job.setRsaPrivateKey(rsaprivate);
    QVERIFY(!job.canStart());

    const QString rsapublic = QStringLiteral("bla");
    job.setRsaPublicKey(rsapublic);
    QVERIFY(job.canStart());

    delete method;
    delete mNetworkAccessManager;
}
