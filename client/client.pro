#-------------------------------------------------
#
# Project created by QtCreator 2019-02-22T17:33:35
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainnetworkmanger.cpp \
    jsoncoder.cpp \
    login.cpp \
    menu.cpp \
    windowprocessslot.cpp \
    chess_place.cpp \
    ranking.cpp \
    loading.cpp \
    chess_place_info.cpp \
    chat.cpp \
    eventwidget.cpp \
    event.cpp

HEADERS += \
        mainnetworkmanger.h \
    jsoncoder.h \
    login.h \
    menu.h \
    windowprocessslot.h \
    chess_place.h \
    ranking.h \
    loading.h \
    chess_place_info.h \
    chat.h \
    eventwidget.h \
    event.h

FORMS += \
        mainnetworkmanger.ui \
    login.ui \
    menu.ui \
    chess_place.ui \
    ranking.ui \
    loading.ui \
    chess_place_info.ui \
    chat.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    main.ico

RESOURCES += \
    icon.qrc
RC_ICONS = "main.ico"
