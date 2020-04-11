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

#ifndef PASSWORDPLUGINAUTHENTICATION_H
#define PASSWORDPLUGINAUTHENTICATION_H

#include "plugins/pluginauthentication.h"

//#define K_PLUGIN_FACTORY_DECLARATION_WITH_BASEFACTORY_SKEL(name, baseFactory, ...) \
//    class name : public KPluginFactory \
//    { \
//        Q_OBJECT \
//        Q_INTERFACES(KPluginFactory) \
//        __VA_ARGS__ \
//    public: \
//        explicit name(); \
//        ~name(); \
//    };

//#define K_PLUGIN_FACTORY_DECLARATION_WITH_BASEFACTORY_JSON(name, baseFactory, json) \
//    K_PLUGIN_FACTORY_DECLARATION_WITH_BASEFACTORY_SKEL(name, baseFactory, Q_PLUGIN_METADATA(IID KPluginFactory_iid FILE json))

//#define K_PLUGIN_FACTORY_WITH_BASEFACTORY_JSON(name, baseFactory, jsonFile, pluginRegistrations) \
//    K_PLUGIN_FACTORY_DECLARATION_WITH_BASEFACTORY_JSON(name, baseFactory, jsonFile) \
//    K_PLUGIN_FACTORY_DEFINITION_WITH_BASEFACTORY(name, baseFactory, pluginRegistrations)

//#define K_PLUGIN_FACTORY_WITH_JSON(name, jsonFile, pluginRegistrations)  K_PLUGIN_FACTORY_WITH_BASEFACTORY_JSON(name, KPluginFactory, jsonFile, pluginRegistrations)


//#define K_PLUGIN_CLASS_WITH_JSON(classname,jsonFile) K_PLUGIN_FACTORY_WITH_JSON(classname ## Factory, jsonFile, registerPlugin<classname >();)

class PasswordPluginAuthentication : public PluginAuthentication
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDummyPlugin" FILE "ruqola_passwordauthenticationplugin.json")

public:
    explicit PasswordPluginAuthentication(QObject *parent = nullptr, const QVariantList & = {});
    ~PasswordPluginAuthentication() override;

    PluginAuthenticationInterface *createInterface(QObject *parent) override;
    AuthenticationManager::OauthType type() const override;
    QString name() const override;
    QString iconName() const override;
};

#endif // PASSWORDPLUGINAUTHENTICATION_H
