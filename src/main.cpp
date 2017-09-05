/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QApplication>
#include <QtQml>
#include <KLocalizedString>
#include <KCrash>
#include "usersforroommodel.h"
#include "usersmodel.h"
#include "ruqola.h"
#include "roomwrapper.h"
#include "ruqolautils.h"
#include "notification.h"
#include "config-ruqola.h"
#include "rocketchataccount.h"
#include "roomfilterproxymodel.h"
#include "message.h"

#include <QJsonDocument>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    app.setWindowIcon(QIcon(QStringLiteral(":/icons/systray.png")));

    KCrash::initialize();

    KLocalizedString::setApplicationDomain("ruqola");
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QCoreApplication::setApplicationName(QStringLiteral("Ruqola"));
    QCoreApplication::setApplicationVersion(QStringLiteral(RUQOLA_VERSION));
    QCommandLineParser parser;

    //TODO improve it.
    parser.setApplicationDescription(i18n("Qt Rocket Chat"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    qmlRegisterSingletonType<Ruqola>("KDE.Ruqola.Ruqola", 1, 0, "Ruqola", ruqola_singletontype_provider);
    qmlRegisterSingletonType<RuqolaUtils>("KDE.Ruqola.RuqolaUtils", 1, 0, "RuqolaUtils", ruqolautils_singletontype_provider);
    qmlRegisterType<MessageModel>("KDE.Ruqola.MessageModel", 1, 0, "MessageModel");
    qmlRegisterType<DDPClient>("KDE.Ruqola.DDPClient", 1, 0, "DDPClient");
    qmlRegisterType<RoomWrapper>("KDE.Ruqola.RoomWrapper", 1, 0, "RoomWrapper");
    qmlRegisterType<UsersForRoomModel>("KDE.Ruqola.UsersForRoomModel", 1, 0, "UsersForRoomModel");
    qmlRegisterType<RocketChatAccount>("KDE.Ruqola.RocketChatAccount", 1, 0, "RocketChatAccount");
    qmlRegisterType<UsersModel>("KDE.Ruqola.UsersModel", 1, 0, "UsersModel");
    qmlRegisterType<User>("KDE.Ruqola.User", 1, 0, "User");
    qmlRegisterType<RoomFilterProxyModel>("KDE.Ruqola.RoomFilterProxyModel", 1, 0, "RoomFilterProxyModel");
    qRegisterMetaType<Message::MessageType>();
    qmlRegisterUncreatableType<Message>("KDE.Ruqola.Message", 1, 0, "Message", QStringLiteral("MessageType is an enum container"));

    (void)Ruqola::self();
    QQmlApplicationEngine engine;

#if !defined(Q_OS_ANDROID) || !defined(Q_OS_IOS)
    QQmlContext *ctxt = engine.rootContext();
    qmlRegisterType<Notification>("KDE.Ruqola.Notification", 1, 0, "Notification");
    ctxt->setContextProperty(QStringLiteral("systrayIcon"), Ruqola::self()->notification());
#endif

    engine.load(QUrl(QStringLiteral("qrc:/Desktop.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
