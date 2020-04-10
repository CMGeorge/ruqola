TEMPLATE = app
QT += quick
SOURCES += \
    main.cpp

RESOURCES += \
    qml/qml.qrc

INCLUDEPATH += $$PWD/../../core
INCLUDEPATH += $$PWD/../../../

windows{
LIBS += -L$$OUT_PWD/../../core/debug -lcore
#LIBS+= -L$$OUT_PWD/../../../lib/ -lrocketchatrestapi-qt5
#error($$OUT_PWD/../../)
}
