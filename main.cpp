#include <QApplication>
#include <QtQml>

#include <QSystemTrayIcon>
#include <QMessageBox>

// only if deskop
#include "src/roommodel.h"
#include "src/rocketchatbackend.h"
#include "src/userdata.h"

#include <QDebug>
#include <QtCore/QJsonDocument>

#include <QtCore>
#include <QAction>
#include <QMenu>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/systray.png"));
    
    QCoreApplication::setOrganizationName("KDE");
    QCoreApplication::setOrganizationDomain("kde.org");
    QCoreApplication::setApplicationName("Ruqola");
    
    qmlRegisterSingletonType<UserData>("KDE.Ruqola.UserData", 1, 0, "UserData", userdata_singletontype_provider);
    qmlRegisterType<MessageModel>("KDE.Ruqola.Models", 1, 0, "MessageModel");
    qmlRegisterType<DDPClient>("KDE.Ruqola.DDPClient", 1, 0, "DDPClient");
    qmlRegisterType<RoomModel>("KDE.Ruqola.RoomModel", 1, 0, "RoomModel");
    qmlRegisterType<RoomWrapper>("KDE.Ruqola.RoomWrapper", 1, 0, "RoomWrapper");
    RocketChatBackend c;
    
    QQmlApplicationEngine engine;

/*   QQmlContext *ctxt = engine.rootContext();

    QMenu menu;
    auto quit = menu.addAction("&Quit");
    QObject::connect(quit, &QAction::triggered, &app, &QApplication::quit);

    QSystemTrayIcon systray;
    systray.setIcon(QIcon(":/systray.png"));
    systray.setContextMenu(&menu);
    systray.setVisible(true);
    systray.setToolTip("Ruqola");
    ctxt->setContextProperty("systrayIcon", &systray);
*/

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
