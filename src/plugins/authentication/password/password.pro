TEMPLATE =lib
#CONFIG +=
#CONFIG += plugin
QT+=network
#SOURCES += $$files($$PWD)
INCLUDEPATH += $$PWD/../../../corelib
INCLUDEPATH += $$OUT_PWD/../../../../include/rocketchatrestapi-qt5
mac{
    LIBS+=-F$$OUT_PWD/../../../../lib -framework corelib
}
winddows{
LIBS += -L$$OUT_PWD/../../../../lib #rocketchatrestapi-qt5.prl
LIBS += -LC:\Development\Apps\Qt\CMGeorge\build-ruqola-Desktop_Qt_5_13_2_MSVC2017_64bit-Debug\src\core\debug -lcorelib
}
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
