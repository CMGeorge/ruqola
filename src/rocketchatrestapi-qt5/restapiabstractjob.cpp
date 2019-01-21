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

#include "restapiabstractjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "abstractlogger.h"

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
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RestaApiMethod not defined";
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
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
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
        if (mQueryParameters.offset() >=0) {
            urlQuery.addQueryItem(QStringLiteral("offset"), QString::number(mQueryParameters.offset()));
        }
        if (!mQueryParameters.sorting().isEmpty()) {
            //example    sort={"name":-1,"status":1}
            QMapIterator<QString, QueryParameters::SortOrder> i(mQueryParameters.sorting());
            QString str;
            while (i.hasNext()) {
                i.next();
                if (!str.isEmpty()) {
                    str += QLatin1Char(',');
                }
                str += QLatin1Char('"') + i.key() + QLatin1Char('"') + QLatin1Char(':');
                switch(i.value()) {
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
    if (mRestApiLogger) {
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
