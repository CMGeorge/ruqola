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

#ifndef DOWNLOADFILEJOB_H
#define DOWNLOADFILEJOB_H

#include "restapiabstractjob.h"

#include <QUrl>

class DownloadFileJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit DownloadFileJob(QObject *parent = nullptr);
    ~DownloadFileJob() override;

    bool start() override;
    bool requireHttpAuthentication() const override;
    bool canStart() const override;

    QUrl url() const;
    void setUrl(const QUrl &url);

    QString mimeType() const;
    void setMimeType(const QString &mimeType);

    QNetworkRequest request() const;

    bool storeInCache() const;
    void setStoreInCache(bool storeInCache);

    QUrl localFileUrl() const;
    void setLocalFileUrl(const QUrl &localFileUrl);

Q_SIGNALS:
    void downloadFileDone(const QByteArray &data, const QUrl &url, bool useCache, const QUrl &localFileUrl);

private:
    Q_DISABLE_COPY(DownloadFileJob)
    void slotDownloadDone();
    QUrl mUrl;
    QString mMimeType;
    QUrl mLocalFileUrl;
    bool mStoreInCache = true;
};

#endif // DOWNLOADFILEJOB_H
