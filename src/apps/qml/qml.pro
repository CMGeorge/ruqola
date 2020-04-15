TEMPLATE = app
QT += quick multimedia websockets network webview
DESTDIR += $$OUT_PWD/../../../bin/
SOURCES += \
    main.cpp

RESOURCES += \
    qml/qml.qrc

INCLUDEPATH += $$PWD/../../core
INCLUDEPATH += $$PWD/../../../
#error($$INCLUDEPATH)
#LIBS += -L$$OUT_PWD/../../../lib -lruqolacore

#error($$OUT_PWD/../../lib)
windows{
LIBS += -L$$OUT_PWD/../../../lib -lruqolacored
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

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android

    ANDROID_EXTRA_LIBS = \
        $$PWD/../../../android_libs/arm/libcrypto_1_1.so \
        $$PWD/../../../android_libs/arm/libssl_1_1.so
}
