#-------------------------------------------------
#
# Project created by QtCreator 2017-09-25T20:16:40
#
#-------------------------------------------------

QT += core gui
QT += network
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SUA
TEMPLATE = app

TRANSLATIONS = SUA_ru.ts \
               SUA_en.ts \

CODECFORTR   = Windows-1251

SOURCES += \
    ImageStatus.cpp \
    main.cpp \
    NetworkBase.cpp \
    NetworkSettings.cpp \
    Protocol.cpp \
    RecievedArray.cpp \
    Spoiler.cpp \
    sua.cpp \
    SUASerializer.cpp \
    SUASettings.cpp \
    TelemetryDecoder.cpp

HEADERS  += \
    ImageStatus.h \
    NetworkBase.h \
    NetworkSettings.h \
    Protocol.h \
    RecievedArray.h \
    Spoiler.h \
    sua.h \
    SUASerializer.h \
    SUASettings.h \
    TelemetryDecoder.h \
    TelemetryPacket.h

FORMS    += \
    sua.ui

RESOURCES += \
    sua.qrc

RC_ICONS = images\icon.ico
