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

#ifndef AUTHENTICATIONMANAGER_H
#define AUTHENTICATIONMANAGER_H

#include "KDE/kpluginmetadata.h"
#include <QObject>
#include "libruqolacore_export.h"
#include <QVector>
#include <QDebug>
#include <QMutex>
#include <QPluginLoader>
#include <QDir>
#include<QCoreApplication>
#include <QDirIterator>

//#ifdef Q_OS_WINDOWS
//#include "authentication/password/passwordpluginauthentication.h"

#include <QFileInfo>
#include "KDE/kpluginfactory.h"
#include "KDE/kpluginmetadata.h"
//#include <QPluginLoader>
#include <QLibrary>
//class KPluginLoader;
//class KPluginMetaDataPrivate : public QSharedData
//{
//public:
//    QString metaDataFileName;
//};
//class KPluginMetaData /*public QObject*/{
//    //Q_OBJECT
//public:
//    inline KPluginMetaData(){}
//    //    KPluginMetaData(const KPluginLoader &loader);
//    //    KPluginMetaData(const QPluginLoader &loader);
//    //    KPluginMetaData(const QString &file);

//    //    KPluginMetaData(const QJsonObject &metaData, const QString &file);
//    //    KPluginMetaData(const QJsonObject &metaData, const QString &pluginFile, const QString &metaDataFile);
//    inline KPluginMetaData(const KPluginMetaData &other) : m_metaData(other.m_metaData), m_fileName(other.fileName()), d(other.d){}
//    inline KPluginMetaData &operator=(const KPluginMetaData &other){
//        m_metaData = other.m_metaData;
//        m_fileName = other.m_fileName;
//        d = other.d;
//        return *this;
//    }

//    inline ~KPluginMetaData(){}



//    inline QString name() const {return "Not Implemented";}
//    inline int pluginId() const  {return -1;}
//    inline  QString fileName() const {return "AuthFIleNameHere";}

//private:
//    QJsonObject m_metaData;
//    QString m_fileName;
//    QExplicitlySharedDataPointer<KPluginMetaDataPrivate> d; // for future binary compatible extensions
//};



//#endif
class PluginAuthentication;

class PluginUtilData
{
public:
    PluginUtilData()
    {
    }

    QStringList mExtraInfo;
    QString mIdentifier;
    QString mName;
};

class AuthenticationManagerInfo
{
public:
    AuthenticationManagerInfo()
    {
    }

    QString metaDataFileNameBaseName;
    QString metaDataFileName;
    PluginUtilData pluginData;
    PluginAuthentication *plugin = nullptr;
};
//class KPluginLoader:public QObject{
//    Q_OBJECT
//public:
//    explicit KPluginLoader(const QString &plugin, QObject *parent = nullptr){

//    }
//    QObject *instance(){
//#ifndef Q_OS_WINDOWS
//        Q_D(const KPluginLoader);

//        if (!load()) {
//            return nullptr;
//        }

//        return d->loader->instance();
//#else
//        qDebug()<<"============QObject *instance()======================";
//        return new QObject();
//     #endif
//    }
//    KPluginFactory *factory(){
//        #ifndef Q_OS_WINDOWS
//        Q_D(KPluginLoader);

//        QObject *obj = instance();

//        if (!obj) {
//            return nullptr;
//        }

//        KPluginFactory *factory = qobject_cast<KPluginFactory *>(obj);

//        if (factory == nullptr) {
//            qDebug() << "Expected a KPluginFactory, got a" << obj->metaObject()->className();
//            delete obj;
//            d->errorString = tr("The library %1 does not offer a KPluginFactory.").arg(d->name);
//        }

//        return factory;
//#else
//        return qobject_cast<KPluginFactory *>(instance());
//#endif
//    }
//    static QString findPlugin(const QString &name){
//        static QMutex s_qtWorkaroundMutex;
//        QMutexLocker lock(&s_qtWorkaroundMutex);

//        QPluginLoader loader(name);
//        return loader.fileName();
//    }
//    static QVector<KPluginMetaData> findPlugins(const QString &directory,
//                                                std::function<bool(const KPluginMetaData &)> filter = std::function<bool(const KPluginMetaData &)>()){
//        QVector<KPluginMetaData> ret;
//        forEachPlugin(directory, [&](const QString &pluginPath) {
//            KPluginMetaData metadata(pluginPath);
//            if (!metadata.isValid()) {
//                return;
//            }
//            if (filter && !filter(metadata)) {
//                return;
//            }
//            ret.append(metadata);
//        });
//        return ret;
//    }
//    static void forEachPlugin(const QString &directory, std::function<void(const QString &)> callback)
//    {
//        QStringList dirsToCheck;
//        if (QDir::isAbsolutePath(directory)) {
//            dirsToCheck << directory;
//        } else {
//            const QStringList listPaths = QCoreApplication::libraryPaths();
//            for (const QString &libDir : listPaths) {
//                dirsToCheck << libDir + QLatin1Char('/') + directory;
//            }
//        }

//        qDebug() << "Checking for plugins in" << dirsToCheck;

//        for (const QString &dir : qAsConst(dirsToCheck)) {
//            QDirIterator it(dir, QDir::Files);
//            while (it.hasNext()) {
//                it.next();
//                if (QLibrary::isLibrary(it.fileName())) {
//                    callback(it.fileInfo().absoluteFilePath());
//                }
//            }
//        }
//    }
//};

class LIBRUQOLACORE_EXPORT AuthenticationManager : public QObject
{
    Q_OBJECT
public:
    enum OauthType {
        Unknown = 0,
        Twitter = 1,
        FaceBook = 2,
        GitHub = 4,
        GitLab = 8,
        Google = 16,
        Linkedin = 32,
        Wordpress = 64,
        Password = 128
    };
    Q_ENUM(OauthType)
    Q_DECLARE_FLAGS(OauthTypes, OauthType)

    explicit AuthenticationManager(QObject *parent = nullptr);
    ~AuthenticationManager() override;

    static AuthenticationManager *self();

    Q_REQUIRED_RESULT QVector<PluginAuthentication *> pluginsList() const;

private:
    Q_DISABLE_COPY(AuthenticationManager)
    bool initializePluginList();
    void loadPlugin(AuthenticationManagerInfo *item);
    PluginUtilData createPluginMetaData(const KPluginMetaData &metaData);
    QVector<AuthenticationManagerInfo> mPluginList;
    QVector<PluginUtilData> mPluginDataList;
};
Q_DECLARE_METATYPE(AuthenticationManager::OauthTypes)
Q_DECLARE_METATYPE(AuthenticationManager::OauthType)

#endif // AUTHENTICATIONMANAGER_H
