TEMPLATE = lib
TARGET=rocketchatrestapi-qt5
QT += core network networkauth
CONFIG += c++11
DEFINES += IS_LIBROCKETCHATRESTAPI_QT5_EXPORT
#DEFINE += ROCKETCHATQTRESTAPI_LOG=RoketChatQtRestAPI
include($$PWD/rocketchatrestapi-qt5.pri)

DISTFILES += \
    autotests/data/inviteusers/test1.json
win32{
    CONFIG -= debug_and_release debug_and_release_target
    CONFIG += skip_target_version_ext
    DESTDIR = $$OUT_PWD/../../lib/
}


#CMAKE_MODULE_TESTS = '-'
#load(qt_build_config)
#VERSION = 1.0.0
#load(qt_module)
