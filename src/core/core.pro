TEMPLATE = lib
TARGET = ruqolacore
TARGET = $$qt5LibraryTarget($$TARGET$$QT_LIBINFIX)  # Do this towards the end
CONFIG += c++11
CONFIG += build_all
    CONFIG += skip_target_version_ext

CONFIG += relative_qt_rpath
QT+= network websockets quick network
load(qt_targets)
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
osx:DESTDIR = $$OUT_PWD/../../../lib/osx

LIBS += -L$$OUT_PWD/../../../plugins/osx/authentication/ -lruqola_passwordauthenticationplugin
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


include(core.pri)

CMAKE_MODULE_TESTS = '-'
#load(qt_build_config)
#VERSION = 1.0.0
#load(qt_module)

header_files.files = $$HEADERS
header_files.path = $$OUT_PWD/../../../include/ruqola
INSTALLS += header_files

#find . -name "*.bak" -type f -delete # - clean CPP
#error($$header_files.files)
