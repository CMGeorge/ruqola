/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "ruqolaserverconfig.h"
#include <QStringList>
#include "ruqola_debug.h"

RuqolaServerConfig::RuqolaServerConfig()
{
}

QString RuqolaServerConfig::uniqueId() const
{
    return mUniqueId;
}

void RuqolaServerConfig::setUniqueId(const QString &uniqueId)
{
    mUniqueId = uniqueId;
}

QString RuqolaServerConfig::jitsiMeetUrl() const
{
    return mJitsiMeetUrl;
}

void RuqolaServerConfig::setJitsiMeetUrl(const QString &jitsiMeetUrl)
{
    mJitsiMeetUrl = jitsiMeetUrl;
}

QString RuqolaServerConfig::jitsiMeetPrefix() const
{
    return mJitsiMeetPrefix;
}

void RuqolaServerConfig::setJitsiMeetPrefix(const QString &jitsiMeetPrefix)
{
    mJitsiMeetPrefix = jitsiMeetPrefix;
}

QString RuqolaServerConfig::fileUploadStorageType() const
{
    return mFileUploadStorageType;
}

void RuqolaServerConfig::setFileUploadStorageType(const QString &type)
{
    mFileUploadStorageType = type;
}

void RuqolaServerConfig::setAllowMessageEditing(bool state)
{
    mAllowEditingMessage = state;
}

bool RuqolaServerConfig::allowMessageEditing() const
{
    return mAllowEditingMessage;
}

void RuqolaServerConfig::setBlockEditingMessageInMinutes(int minutes)
{
    mBlockEditingMessageInMinutes = minutes;
}

int RuqolaServerConfig::blockEditingMessageInMinutes() const
{
    return mBlockEditingMessageInMinutes;
}

bool RuqolaServerConfig::otrEnabled() const
{
    return mOtrEnabled;
}

void RuqolaServerConfig::setOtrEnabled(bool otrEnabled)
{
    mOtrEnabled = otrEnabled;
}

bool RuqolaServerConfig::needAdaptNewSubscriptionRC60() const
{
    return mNeedAdaptNewSubscriptionRC60;
}

void RuqolaServerConfig::setServerVersion(const QString &version)
{
    qCDebug(RUQOLA_LOG) << " void RocketChatAccount::setServerVersion(const QString &version)" << version;
    const QStringList lst = version.split(QLatin1Char('.'));
    if (lst.count() == 3) {
        bool ok;
        int value = lst.at(0).toInt(&ok);
        if (ok) {
            mServerVersionMajor = value;
        }
        value = lst.at(1).toInt(&ok);
        if (ok) {
            mServerVersionMinor = value;
        }
        value = lst.at(2).toInt(&ok);
        if (ok) {
            mServerVersionPatch = value;
        }
    }
    adaptToServerVersion();
}

bool RuqolaServerConfig::ruqolaHasSupportForOauthType(AuthenticationManager::OauthType type) const
{
    switch (type) {
    case AuthenticationManager::OauthType::Unknown:
        return false;
    case AuthenticationManager::OauthType::Twitter:
        return false;
    case AuthenticationManager::OauthType::FaceBook:
        return false;
    case AuthenticationManager::OauthType::GitHub:
        return false;
    case AuthenticationManager::OauthType::GitLab:
        return false;
    case AuthenticationManager::OauthType::Google:
        return false;
    case AuthenticationManager::OauthType::Linkedin:
        return false;
    case AuthenticationManager::OauthType::Wordpress:
        return false;
    }
    return false;
}

bool RuqolaServerConfig::canShowOauthService(AuthenticationManager::OauthType type) const
{
    return serverHasSupportForOauthType(type) && ruqolaHasSupportForOauthType(type);
}

bool RuqolaServerConfig::serverHasSupportForOauthType(AuthenticationManager::OauthType type) const
{
    return mOauthTypes & type;
}

void RuqolaServerConfig::addOauthService(const QString &service)
{
    const QString serviceLower = service.toLower();
    if (serviceLower.endsWith(QLatin1String("twitter"))) {
        mOauthTypes |= AuthenticationManager::OauthType::Twitter;
    } else if (serviceLower.endsWith(QLatin1String("facebook"))) {
        mOauthTypes |= AuthenticationManager::OauthType::FaceBook;
    } else if (serviceLower.endsWith(QLatin1String("github"))) {
        mOauthTypes |= AuthenticationManager::OauthType::GitHub;
    } else if (serviceLower.endsWith(QLatin1String("gitlab"))) {
        mOauthTypes |= AuthenticationManager::OauthType::GitLab;
    } else if (serviceLower.endsWith(QLatin1String("google"))) {
        mOauthTypes |= AuthenticationManager::OauthType::Google;
    } else if (serviceLower.endsWith(QLatin1String("linkedin"))) {
        mOauthTypes |= AuthenticationManager::OauthType::Linkedin;
    } else if (serviceLower.endsWith(QLatin1String("wordpress"))) {
        mOauthTypes |= AuthenticationManager::OauthType::Wordpress;
    } else {
        qCWarning(RUQOLA_LOG) << "Unknow service type: " << service;
    }
}

void RuqolaServerConfig::adaptToServerVersion()
{
    mNeedAdaptNewSubscriptionRC60 = (mServerVersionMajor == 0) && (mServerVersionMinor >= 60);
}

AuthenticationManager::OauthTypes RuqolaServerConfig::oauthTypes() const
{
    return mOauthTypes;
}
