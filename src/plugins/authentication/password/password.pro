TEMPLATE =lib
TARGET = ruqola_passwordauthenticationplugin
CONFIG += plugin
QT+=network websockets quick quickcontrols2
#SOURCES += $$files($$PWD)
#INCLUDEPATH += $$OUT_PWD/../../../../include/rocketchatrestapi-qt5
INCLUDEPATH += $$PWD/../../../corelib \
                $$PWD/../../../rocketchatrestapi-qt5 \
                $$PWD/../../../rocketchatrestapi-qt5/channels \
                $$PWD/../../../rocketchatrestapi-qt5/commands \
                $$PWD/../../../rocketchatrestapi-qt5/kde
mac|android{
    LIBS+=-L$$OUT_PWD/../../../corelib -lruqolacore

#INCLUDEPATH += $$PWD/../../../corelib \
#                $$PWD/../../../rocketchatrestapi-qt5 \
#                $$PWD/../../../rocketchatrestapi-qt5/channels \
#                $$PWD/../../../rocketchatrestapi-qt5/commands \
#                $$PWD/../../../rocketchatrestapi-qt5/kde
}
windows{
#INCLUDEPATH += $$PWD/../../../corelib \
#                $$PWD/../../../rocketchatrestapi-qt5 \
#                $$PWD/../../../rocketchatrestapi-qt5/channels \
#                $$PWD/../../../rocketchatrestapi-qt5/commands \
#                $$PWD/../../../rocketchatrestapi-qt5/kde
#\
#                $$OUT_PWD/../../../../include/rocketchatrestapi-qt5
LIBS += -L$$OUT_PWD/../../../../lib -lruqolacore
}
#error($$LIBS)
#error($$OUT_PWD/../../../../include/rocketchatrestapi-qt5)
#$$PWD/../../../core/KDE
#include($$PWD/../../../rocketchatrestapi-qt5/rocketchatrestapi-qt5.pri)

HEADERS += \
    passwordauthenticationinterface.h \
    passwordauthenticationplugin_debug.h \
    passwordpluginauthentication.h

SOURCES += \
    passwordauthenticationinterface.cpp \
    passwordpluginauthentication.cpp

#CMAKE_MODULE_TESTS = '-'
#load(qt_build_config)
#PLUGIN_TYPE=auth
#load(qt_plugin)

!ios:DESTDIR = $$OUT_PWD/../../../../bin/rocketlugins/authentication
#error("$$OUT_PWD/../../../../bin/ruqolaplugins/authentication")
