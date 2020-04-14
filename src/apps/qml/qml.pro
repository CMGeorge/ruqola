TEMPLATE = app
QT += quick multimedia websockets
DESTDIR += $$OUT_PWD/../../../bin/
SOURCES += \
    main.cpp

RESOURCES += \
    qml/qml.qrc

INCLUDEPATH += $$PWD/../../corelib
INCLUDEPATH += $$PWD/../../../

LIBS += -L$$OUT_PWD/../../lib -lruqolacore
windows{
#LIBS+= -L$$OUT_PWD/../../../lib/ -lrocketchatrestapi-qt5
#error($$OUT_PWD/../../)
}
#error($$OUT_PWD/../../../lib)
mac{
#    LIBS += -L$$OUT_PWD/../../corelib -lruqolacore
}
#error($$OUT_PWD/../../../lib)
ios{
    Q_ENABLE_BITCODE.name = ENABLE_BITCODE
    Q_ENABLE_BITCODE.value = NO
    QMAKE_MAC_XCODE_SETTINGS += Q_ENABLE_BITCODE
#    LIBS += -L$$OUT_PWD/../../corelib -lruqolacore
    LIBS += -L$$OUT_PWD/../../plugins/authentication/password/ -lruqola_passwordauthenticationplugin

}
