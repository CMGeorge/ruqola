TEMPLATE = lib
TARGET = ruqolacore
CONFIG += c++11
QT+= network websockets quick network

#CONFIG += plugin
DEFINES += IS_LIBRUQOLACORE_EXPORT
#also methids from restapi should be set as dllexport
DEFINES += IS_LIBROCKETCHATRESTAPI_QT5_EXPORT
#android{
#        LIBS+=-L$$PWD/../../android_libs/arm/ -lcrypto
#        LIBS+=-L$$PWD/../../android_libs/arm/ -lssl
#}
win32{
    CONFIG -= debug_and_release debug_and_release_target
    CONFIG += skip_target_version_ext
    DESTDIR = $$OUT_PWD/../../lib/
}

DISTFILES += \
    corelib.pri \
    emoticons/emoji.json \
    icons/systray.png \
    settings/ruqolaglobalconfig.kcfg \
    settings/ruqolaglobalconfig.kcfgc



#INCLUDEPATH += $$OUT_PWD/../../include/rocketchatrestapi-qt5/
#LIBS+=-L$$OUT_PWD/../../lib/ -lrocketchatrestapi-qt5
#error($$INCLUDEPATH)
include($$PWD/../rocketchatrestapi-qt5/rocketchatrestapi-qt5.pri)

#INCLUDEPATH += $$PWD/../plugins/
#INCLUDEPATH += $$PWD/KDE/


include(corelib.pri)

#CMAKE_MODULE_TESTS = '-'
#load(qt_build_config)
#VERSION = 1.0.0
#load(qt_module)
