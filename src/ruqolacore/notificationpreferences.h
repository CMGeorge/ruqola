/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#ifndef NOTIFICATIONPREFERENCES_H
#define NOTIFICATIONPREFERENCES_H

#include "libruqola_private_export.h"
#include <QObject>
class QQmlEngine;
class QJSEngine;

class NotificationPreferenceModel;
class LIBRUQOLACORE_TESTS_EXPORT NotificationPreferences : public QObject
{
    Q_OBJECT
public:
    explicit NotificationPreferences(QObject *parent = nullptr);
    ~NotificationPreferences();

    Q_INVOKABLE Q_REQUIRED_RESULT NotificationPreferenceModel *emailNotificationModel() const;

    Q_INVOKABLE Q_REQUIRED_RESULT NotificationPreferenceModel *mobileNotificationModel() const;

    Q_INVOKABLE Q_REQUIRED_RESULT NotificationPreferenceModel *desktopNotificationModel() const;

    Q_INVOKABLE Q_REQUIRED_RESULT NotificationPreferenceModel *desktopAudioNotificationModel() const;

private:
    NotificationPreferenceModel *mEmailNotificationModel = nullptr;
    NotificationPreferenceModel *mMobileNotificationModel = nullptr;
    NotificationPreferenceModel *mDesktopNotificationModel = nullptr;
    NotificationPreferenceModel *mDesktopAudioNotificationModel = nullptr;
};

inline static QObject *notificationpreferences_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new NotificationPreferences();
}


#endif // NOTIFICATIONPREFERENCES_H
