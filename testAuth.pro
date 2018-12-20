TEMPLATE = app



QT += qml quick widgets

MOC_DIR = ./build/moc
OBJECTS_DIR = ./build/obj
#DESTDIR = ../build
RCC_DIR = ./build/rcc

SOURCES += main.cpp \
    gui/guimanager.cpp \
    core/serverconnection.cpp \
    cmain.cpp \
    core/commands/abstractcommand.cpp \
    core/commands/commandgettoken.cpp \
    core/commands/commandmgr.cpp \
    core/ccore.cpp \
    core/commands/commandgetnonce.cpp \
    core/commands/abstractnetworkcommand.cpp \
    core/commands/commandregister.cpp \
    core/commands/commandlogin.cpp \
    core/commands/commanditemscountrequest.cpp \
    core/commands/commanditemdescriptionrequest.cpp \
    core/commands/commandrateitem.cpp \
    core/commands/commandskipitem.cpp \
    core/commands/commandrecommenditemscountrequest.cpp \
    common/itemsmodel.cpp

RESOURCES += \
    qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

CONFIG += release

HEADERS += \
    gui/guimanager.h \
    common/commondefines.h \
    core/serverconnection.h \
    cmain.h \
    common/declare.h \
    core/commands/abstractcommand.h \
    core/commands/commandgettoken.h \
    core/commands/events.h \
    core/commands/commandmgr.h \
    core/ccore.h \
    core/commands/commandgetnonce.h \
    core/commands/abstractnetworkcommand.h \
    core/commands/commandregister.h \
    core/commands/commandlogin.h \
    core/commands/commanditemscountrequest.h \
    core/commands/commanditemdescriptionrequest.h \
    core/commands/commandrateitem.h \
    core/commands/commandskipitem.h \
    core/commands/commandrecommenditemscountrequest.h \
    common/itemsmodel.h

DISTFILES += \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
