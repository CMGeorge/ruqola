TEMPLATE = app
QT += quick multimedia
DESTDIR += $$OUT_PWD/../../../bin/
SOURCES += \
    main.cpp

RESOURCES += \
    qml/qml.qrc

INCLUDEPATH += $$PWD/../../corelib
INCLUDEPATH += $$PWD/../../../

windows{
LIBS += -L$$OUT_PWD/../../../lib -lruqolacore
#LIBS+= -L$$OUT_PWD/../../../lib/ -lrocketchatrestapi-qt5
#error($$OUT_PWD/../../)
}
#error($$OUT_PWD/../../../lib)
mac{
    LIBS += -F$$OUT_PWD/../../../lib -framework corelib
}
#error($$OUT_PWD/../../../lib)
