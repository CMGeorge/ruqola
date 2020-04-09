/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "restapiabstractjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "abstractlogger.h"
//#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
using namespace RocketChatRestApi;
RestApiAbstractJob::RestApiAbstractJob(QObject *parent)
    : QObject(parent)
{
}

QNetworkAccessManager *RestApiAbstractJob::networkAccessManager() const
{
    return mNetworkAccessManager;
}

void RestApiAbstractJob::setNetworkAccessManager(QNetworkAccessManager *networkAccessManager)
{
    mNetworkAccessManager = networkAccessManager;
}

RocketChatRestApi::RestApiMethod *RestApiAbstractJob::restApiMethod() const
{
    return mRestApiMethod;
}

void RestApiAbstractJob::setRestApiMethod(RocketChatRestApi::RestApiMethod *restApiMethod)
{
    mRestApiMethod = restApiMethod;
}

QString RestApiAbstractJob::authToken() const
{
    return mAuthToken;
}

void RestApiAbstractJob::setAuthToken(const QString &authToken)
{
    mAuthToken = authToken;
}

QString RestApiAbstractJob::userId() const
{
    return mUserId;
}

void RestApiAbstractJob::setUserId(const QString &userId)
{
    mUserId = userId;
}

bool RestApiAbstractJob::hasAuthenticationValue() const
{
    return !mAuthToken.isEmpty() && !mUserId.isEmpty();
}

bool RestApiAbstractJob::hasQueryParameterSupport() const
{
    return false;
}

bool RestApiAbstractJob::canStart() const
{
    if (!mNetworkAccessManager) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Network manager not defined";
        return false;
    }
    if (!mRestApiMethod) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RestApiMethod not defined";
        return false;
    }
    if (requireHttpAuthentication() && !hasAuthenticationValue()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Auth settings is empty. It's a bug";
        return false;
    }
    return true;
}

void RestApiAbstractJob::addRequestAttribute(QNetworkRequest &request) const
{
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
#else
    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, true);
#endif
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
}

void RestApiAbstractJob::addAuthRawHeader(QNetworkRequest &request) const
{
    request.setRawHeader(QByteArrayLiteral("X-Auth-Token"), mAuthToken.toLocal8Bit());
    request.setRawHeader(QByteArrayLiteral("X-User-Id"), mUserId.toLocal8Bit());
}

QueryParameters RestApiAbstractJob::queryParameters() const
{
    return mQueryParameters;
}

void RestApiAbstractJob::setQueryParameters(const QueryParameters &queryParameters)
{
    mQueryParameters = queryParameters;
}

void RestApiAbstractJob::addQueryParameter(QUrlQuery &urlQuery) const
{
    if (hasQueryParameterSupport() && mQueryParameters.isValid()) {
        if (mQueryParameters.count() >= 0) {
            urlQuery.addQueryItem(QStringLiteral("count"), QString::number(mQueryParameters.count()));
        }
        if (mQueryParameters.offset() >= 0) {
            urlQuery.addQueryItem(QStringLiteral("offset"), QString::number(mQueryParameters.offset()));
        }
        if (!mQueryParameters.sorting().isEmpty()) {
            //example    sort={"name" : -1,"status" : 1}
            QMapIterator<QString, QueryParameters::SortOrder> i(mQueryParameters.sorting());
            QString str;
            while (i.hasNext()) {
                i.next();
                if (!str.isEmpty()) {
                    str += QLatin1Char(',');
                }
                str += QLatin1Char('"') + i.key() + QLatin1Char('"') + QLatin1Char(':');
                switch (i.value()) {
                case QueryParameters::SortOrder::Ascendant:
                    str += QString::number(1);
                    break;
                case QueryParameters::SortOrder::Descendant:
                    str += QString::number(-1);
                    break;
                case QueryParameters::SortOrder::NoSorting:
                    qCWarning(ROCKETCHATQTRESTAPI_LOG) << "It's not a sorting attribute";
                    break;
                }
            }
            str = QStringLiteral("{%1}").arg(str);

            //It's ok for getAllMentions....
            urlQuery.addQueryItem(QStringLiteral("sort"), str);
        }
    }
    //qDebug() << " urlQuery " << urlQuery.toString();
}

RocketChatRestApi::AbstractLogger *RestApiAbstractJob::restApiLogger() const
{
    return mRestApiLogger;
}

void RestApiAbstractJob::setRestApiLogger(RocketChatRestApi::AbstractLogger *ruqolaLogger)
{
    mRestApiLogger = ruqolaLogger;
}

void RestApiAbstractJob::addLoggerInfo(const QByteArray &str)
{
    if (mRestApiLogger) { // when $RUQOLA_LOGFILE is set
        mRestApiLogger->dataSent("RESTAPI: " + str);
    } else {
        qCDebug(ROCKETCHATQTRESTAPI_LOG) << "RESTAPI: " << str;
    }
}

void RestApiAbstractJob::addLoggerWarning(const QByteArray &str)
{
    if (mRestApiLogger) {
        mRestApiLogger->dataSent("WARNING RESTAPI: " + str);
    } else {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RESTAPI: " << str;
    }
}

void RestApiAbstractJob::emitFailedMessage(const QJsonObject &replyObject, QNetworkReply *reply)
{
    const auto error = reply->error();
    // HTTP-level error (e.g. host not found)
    if (error != QNetworkReply::NoError) {
        Q_EMIT failed(reply->errorString() + QLatin1Char('\n') + errorStr(replyObject));
        return;
    }
    Q_EMIT failed(errorStr(replyObject));
}

QString RestApiAbstractJob::errorStr(const QJsonObject &replyObject)
{
    // JSon-level error
    const QString errorType = replyObject[QStringLiteral("errorType")].toString();
    if (!errorType.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "errorType" << errorType;
        const QString trStr = errorMessage(errorType);
        if (!trStr.isEmpty()) {
            return trStr;
        } else {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) << " errorType not defined as translated message: " << errorType;
            return tr("Unauthorized");
        }
    } else {
        const QString error = replyObject[QStringLiteral("error")].toString();
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "error " << error;
        return generateErrorMessage(error);
    }
}

QString RestApiAbstractJob::generateErrorMessage(const QString &errorStr) const
{
    if (jobName().isEmpty()) {
        return errorStr;
    }
    return tr("%1:%2").arg(jobName()).arg( errorStr);
}

QString RestApiAbstractJob::errorMessage(const QString &str)
{
    if (str == QLatin1String("error-action-not-allowed")) {
        return tr("__action__ is not allowed");
    } else if (str == QLatin1String("error-application-not-found")) {
        return tr("Application not found");
    } else if (str == QLatin1String("error-archived-duplicate-name")) {
        return tr("There's an archived channel with name '__room_name__'");
    } else if (str == QLatin1String("error-avatar-invalid-url")) {
        return tr("Invalid avatar URL: __url__");
    } else if (str == QLatin1String("error-avatar-url-handling")) {
        return tr("Error while handling avatar setting from a URL (__url__) for __username__");
    } else if (str == QLatin1String("error-cant-invite-for-direct-room")) {
        return tr("Can't invite user to direct rooms");
    } else if (str == QLatin1String("error-channels-setdefault-is-same")) {
        return tr("The channel default setting is the same as what it would be changed to.");
    } else if (str == QLatin1String("error-channels-setdefault-missing-default-param")) {
        return tr("The bodyParam 'default' is required");
    } else if (str == QLatin1String("error-could-not-change-email")) {
        return tr("Could not change email");
    } else if (str == QLatin1String("error-could-not-change-name")) {
        return tr("Could not change name");
    } else if (str == QLatin1String("error-could-not-change-username")) {
        return tr("Could not change username");
    } else if (str == QLatin1String("error-delete-protected-role")) {
        return tr("Cannot delete a protected role");
    } else if (str == QLatin1String("error-department-not-found")) {
        return tr("Department not found");
    } else if (str == QLatin1String("error-direct-message-file-upload-not-allowed")) {
        return tr("File sharing not allowed in direct messages");
    } else if (str == QLatin1String("error-duplicate-channel-name")) {
        return tr("A channel with name '__channel_name__' exists");
    } else if (str == QLatin1String("error-edit-permissions-not-allowed")) {
        return tr("Editing permissions is not allowed");
    } else if (str == QLatin1String("error-email-domain-blacklisted")) {
        return tr("The email domain is blacklisted");
    } else if (str == QLatin1String("error-email-send-failed")) {
        return tr("Error trying to send email: __message__");
    } else if (str == QLatin1String("error-field-unavailable")) {
        return tr("<strong>__field__</strong> is already in use :(");
    } else if (str == QLatin1String("error-file-too-large")) {
        return tr("File is too large");
    } else if (str == QLatin1String("error-importer-not-defined")) {
        return tr("The importer was not defined correctly, it is missing the Import class.");
    } else if (str == QLatin1String("error-import-file-extract-error")) {
        return tr("Failed to extract import file.");
    } else if (str == QLatin1String("error-import-file-is-empty")) {
        return tr("Imported file seems to be empty.");
    } else if (str == QLatin1String("error-import-file-missing")) {
        return tr("The file to be imported was not found on the specified path.");
    } else if (str == QLatin1String("error-input-is-not-a-valid-field")) {
        return tr("__input__ is not a valid __field__");
    } else if (str == QLatin1String("error-invalid-actionlink")) {
        return tr("Invalid action link");
    } else if (str == QLatin1String("error-invalid-account")) {
        return tr("Invalid Account");
    } else if (str == QLatin1String("error-invalid-arguments")) {
        return tr("Invalid arguments");
    } else if (str == QLatin1String("error-invalid-asset")) {
        return tr("Invalid asset");
    } else if (str == QLatin1String("error-invalid-channel")) {
        return tr("Invalid channel.");
    } else if (str == QLatin1String("error-invalid-channel-start-with-chars")) {
        return tr("Invalid channel. Start with @ or #");
    } else if (str == QLatin1String("error-invalid-custom-field")) {
        return tr("Invalid custom field");
    } else if (str == QLatin1String("error-invalid-custom-field-name")) {
        return tr("Invalid custom field name. Use only letters, numbers, hyphens and underscores.");
    } else if (str == QLatin1String("error-invalid-date")) {
        return tr("Invalid date provided.");
    } else if (str == QLatin1String("error-invalid-description")) {
        return tr("Invalid description");
    } else if (str == QLatin1String("error-invalid-domain")) {
        return tr("Invalid domain");
    } else if (str == QLatin1String("error-invalid-email")) {
        return tr("Invalid email __email__");
    } else if (str == QLatin1String("error-invalid-email-address")) {
        return tr("Invalid email address");
    } else if (str == QLatin1String("error-invalid-file-height")) {
        return tr("Invalid file height");
    } else if (str == QLatin1String("error-invalid-file-type")) {
        return tr("Invalid file type");
    } else if (str == QLatin1String("error-invalid-file-width")) {
        return tr("Invalid file width");
    } else if (str == QLatin1String("error-invalid-from-address")) {
        return tr("You informed an invalid FROM address.");
    } else if (str == QLatin1String("error-invalid-integration")) {
        return tr("Invalid integration");
    } else if (str == QLatin1String("error-invalid-message")) {
        return tr("Invalid message");
    } else if (str == QLatin1String("error-invalid-method")) {
        return tr("Invalid method");
    } else if (str == QLatin1String("error-invalid-name")) {
        return tr("Invalid name");
    } else if (str == QLatin1String("error-invalid-password")) {
        return tr("Invalid password");
    } else if (str == QLatin1String("error-invalid-permission")) {
        return tr("Invalid permission");
    } else if (str == QLatin1String("error-invalid-redirectUri")) {
        return tr("Invalid redirectUri");
    } else if (str == QLatin1String("error-invalid-role")) {
        return tr("Invalid role");
    } else if (str == QLatin1String("error-invalid-room")) {
        return tr("Invalid room");
    } else if (str == QLatin1String("error-invalid-room-name")) {
        return tr("<strong>__room_name__</strong> is not a valid room name");
    } else if (str == QLatin1String("error-invalid-room-type")) {
        return tr("<strong>__type__</strong> is not a valid room type.");
    } else if (str == QLatin1String("error-invalid-settings")) {
        return tr("Invalid settings provided");
    } else if (str == QLatin1String("error-invalid-subscription")) {
        return tr("Invalid subscription");
    } else if (str == QLatin1String("error-invalid-token")) {
        return tr("Invalid token");
    } else if (str == QLatin1String("error-invalid-triggerWords")) {
        return tr("Invalid triggerWords");
    } else if (str == QLatin1String("error-invalid-urls")) {
        return tr("Invalid URLs");
    } else if (str == QLatin1String("error-invalid-user")) {
        return tr("Invalid user");
    } else if (str == QLatin1String("error-invalid-username")) {
        return tr("Invalid username");
    } else if (str == QLatin1String("error-invalid-webhook-response")) {
        return tr("The webhook URL responded with a status other than 200");
    } else if (str == QLatin1String("error-message-deleting-blocked")) {
        return tr("Message deleting is blocked");
    } else if (str == QLatin1String("error-message-editing-blocked")) {
        return tr("Message editing is blocked");
    } else if (str == QLatin1String("error-message-size-exceeded")) {
        return tr("Message size exceeds Message_MaxAllowedSize");
    } else if (str == QLatin1String("error-missing-unsubscribe-link")) {
        return tr("You must provide the [unsubscribe] link.");
    } else if (str == QLatin1String("error-no-tokens-for-this-user")) {
        return tr("There are no tokens for this user");
    } else if (str == QLatin1String("error-not-allowed")) {
        return tr("Not allowed");
    } else if (str == QLatin1String("error-not-authorized")) {
        return tr("Not authorized");
    } else if (str == QLatin1String("error-password-policy-not-met")) {
        return tr("Password does not meet the server's policy");
    } else if (str == QLatin1String("error-password-policy-not-met-maxLength")) {
        return tr("Password does not meet the server's policy of maximum length (password too long)");
    } else if (str == QLatin1String("error-password-policy-not-met-minLength")) {
        return tr("Password does not meet the server's policy of minimum length (password too short)");
    } else if (str == QLatin1String("error-password-policy-not-met-oneLowercase")) {
        return tr("Password does not meet the server's policy of at least one lowercase character");
    } else if (str == QLatin1String("error-password-policy-not-met-oneNumber")) {
        return tr("Password does not meet the server's policy of at least one numerical character");
    } else if (str == QLatin1String("error-password-policy-not-met-oneSpecial")) {
        return tr("Password does not meet the server's policy of at least one special character");
    } else if (str == QLatin1String("error-password-policy-not-met-oneUppercase")) {
        return tr("Password does not meet the server's policy of at least one uppercase character");
    } else if (str == QLatin1String("error-password-policy-not-met-repeatingCharacters")) {
        return tr("Password does not meet the server's policy of forbidden repeating characters (you have too many of the same characters next to each other)");
    } else if (str == QLatin1String("error-push-disabled")) {
        return tr("Push is disabled");
    } else if (str == QLatin1String("error-remove-last-owner")) {
        return tr("This is the last owner. Please set a new owner before removing this one.");
    } else if (str == QLatin1String("error-role-in-use")) {
        return tr("Cannot delete role because it's in use");
    } else if (str == QLatin1String("error-role-name-required")) {
        return tr("Role name is required");
    } else if (str == QLatin1String("error-room-is-not-closed")) {
        return tr("Room is not closed");
    } else if (str == QLatin1String("error-the-field-is-required")) {
        return tr("The field __field__ is required.");
    } else if (str == QLatin1String("error-this-is-not-a-livechat-room")) {
        return tr("This is not a Livechat room");
    } else if (str == QLatin1String("error-personal-access-tokens-are-current-disabled")) {
        return tr("Personal Access Tokens are currently disabled");
    } else if (str == QLatin1String("error-token-already-exists")) {
        return tr("A token with this name already exists");
    } else if (str == QLatin1String("error-token-does-not-exists")) {
        return tr("Token does not exists");
    } else if (str == QLatin1String("error-too-many-requests")) {
        return tr("Error, too many requests. Please slow down. You must wait __seconds__ seconds before trying again.");
    } else if (str == QLatin1String("error-user-has-no-roles")) {
        return tr("User has no roles");
    } else if (str == QLatin1String("error-user-is-not-activated")) {
        return tr("User is not activated");
    } else if (str == QLatin1String("error-user-limit-exceeded")) {
        return tr("The number of users you are trying to invite to #channel_name exceeds the limit set by the administrator");
    } else if (str == QLatin1String("error-user-not-in-room")) {
        return tr("User is not in this room");
    } else if (str == QLatin1String("error-logged-user-not-in-room")) {
        return tr("You are not in the room `%s`");
    } else if (str == QLatin1String("error-user-registration-disabled")) {
        return tr("User registration is disabled");
    } else if (str == QLatin1String("error-user-registration-secret")) {
        return tr("User registration is only allowed via Secret URL");
    } else if (str == QLatin1String("error-you-are-last-owner")) {
        return tr("You are the last owner. Please set new owner before leaving the room.");
    } else if (str == QLatin1String("error-room-archived")) {
        return tr("The private group is archived");
    } else if (str == QLatin1String("error-user-already-owner")) {
        return tr("User is already an owner");
    } else if (str == QLatin1String("error-invalid-message_id")) {
        return tr("Invalid message id");
    } else if (str == QLatin1String("error-user-not-leader")) {
        return tr("User is not a leader");
    } else if (str == QLatin1String("error-app-user-is-not-allowed-to-login")) {
        return tr("App user is not allowed to login");
    } else if (str == QLatin1String("error-direct-message-room")) {
        return tr("Direct Messages can not be archived");
    } else if (str == QLatin1String("error-message-not-found")) {
        return tr("Message not found.");
    } else {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << " unknown error type " << str;
        return {};
    }
}

QString RestApiAbstractJob::jobName() const
{
    return {};
}

QNetworkReply *RestApiAbstractJob::submitGetRequest()
{
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    reply->setProperty("job", QVariant::fromValue(this));
    return reply;
}

QNetworkReply *RestApiAbstractJob::submitPostRequest(const QJsonDocument &doc)
{
    const QByteArray baPostData = doc.toJson(QJsonDocument::Compact);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    reply->setProperty("job", QVariant::fromValue(this));
    addLoggerInfo(QByteArray(metaObject()->className()) + " started " + baPostData);
    return reply;
}

QueryParameters::QueryParameters()
{
}

int QueryParameters::offset() const
{
    return mOffset;
}

void QueryParameters::setOffset(int offset)
{
    mOffset = offset;
}

int QueryParameters::count() const
{
    return mCount;
}

void QueryParameters::setCount(int count)
{
    mCount = count;
}

bool QueryParameters::isValid() const
{
    return (mCount >= 0) || (mOffset >= 0) || (!mSorting.isEmpty());
}

QMap<QString, QueryParameters::SortOrder> QueryParameters::sorting() const
{
    return mSorting;
}

void QueryParameters::setSorting(const QMap<QString, QueryParameters::SortOrder> &sorting)
{
    mSorting = sorting;
}
