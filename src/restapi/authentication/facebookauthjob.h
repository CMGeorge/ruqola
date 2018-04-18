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

#ifndef FACEBOOKAUTHJOB_H
#define FACEBOOKAUTHJOB_H
#include "libruqola_private_export.h"
#include "restapiabstractjob.h"

#include <QNetworkRequest>

class LIBRUQOLACORE_TESTS_EXPORT FacebookAuthJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit FacebookAuthJob(QObject *parent = nullptr);
    ~FacebookAuthJob() override;

    bool start() override;
    bool requireHttpAuthentication() const override;
    QString userName() const;
    void setUserName(const QString &userName);

    QString password() const;
    void setPassword(const QString &password);

    bool canStart() const override;

    QNetworkRequest request() const override;
    QJsonDocument json() const;

Q_SIGNALS:
    void loginDone(const QString &authToken, const QString &userId);

private:
    Q_DISABLE_COPY(FacebookAuthJob)
    void slotLoginDone();
    QString mUserName;
    QString mPassword;
};

#endif // FACEBOOKAUTHJOB_H
