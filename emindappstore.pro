#-------------------------------------------------
#
# Project created by QtCreator 2017-05-24T14:55:47
#
#-------------------------------------------------

QT       += core gui network dbus
QT       += quickwidgets
QT       += quick qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = emindappstore
TEMPLATE = app

#DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp \
    softwarecenterwindow.cpp \
    homewidget.cpp \
    titlebar.cpp \
    ClassAndMorePage/classtop.cpp \
    ClassAndMorePage/classwidget.cpp \
    ClassAndMorePage/element.cpp \
    ClassAndMorePage/showmore.cpp \
    updatepage/appwidget.cpp \
    updatepage/funcwidget.cpp \
    updatepage/taskbar.cpp \
    detailwidget.cpp \
    customlineedit.cpp \
    searchwidget.cpp \
    datamanager.cpp \
    ClassAndMorePage/classpage.cpp \
    ClassAndMorePage/morepage.cpp \
    ClassAndMorePage/recommendwidget.cpp \
    managerpage/managerpage.cpp \
    managerpage/managerwidget.cpp \
    updatepage/pkupdates.cpp \
    updatepage/updatepage.cpp \
    jsonfunc.cpp \
    commentlistmodel.cpp \
    homeimglistmodel.cpp \
    detailimglistmodel.cpp \
    tipswidget.cpp \
    managerpage/pkdetailthread.cpp \
    dbus-pkdbus/pkdbusInterface.cpp

HEADERS  += \
    softwarecenterwindow.h \
    homewidget.h \
    titlebar.h \
    ClassAndMorePage/classtop.h \
    ClassAndMorePage/classwidget.h \
    ClassAndMorePage/element.h \
    ClassAndMorePage/showmore.h \
    updatepage/appwidget.h \
    updatepage/funcwidget.h \
    updatepage/taskbar.h \
    detailwidget.h \
    customlineedit.h \
    searchwidget.h \
    datamanager.h \
    ClassAndMorePage/classpage.h \
    ClassAndMorePage/morepage.h \
    ClassAndMorePage/recommendwidget.h \
    managerpage/managerpage.h \
    managerpage/managerwidget.h \
    updatepage/pkupdates.h \
    updatepage/updatepage.h \
    jsonfunc.h \
    qreplytimeout.h \
    sharedata.h \
    commentlistmodel.h \
    homeimglistmodel.h \
    detailimglistmodel.h \
    tipswidget.h \r \
    managerpage/pkdetailthread.h \
    dbus-pkdbus/pkdbusInterface.h

RESOURCES += \
    style.qrc \
    qml.qrc \
    image.qrc

DISTFILES += \
    style.qss \
    updatepage/image/head.png \
    updatepage/image/hide.png \
    README.md \
    app_zh.ts \
    ipaddress \
    misc/emindappstore.desktop \
    debian/source/format \
    debian/compat \
    debian/control \
    debian/changelog \
    debian/install \
    debian/rules \
    DelegateItem.qml


INCLUDEPATH += /usr/include/packagekitqt5/PackageKit

LIBS += /usr/lib/x86_64-linux-gnu/libpackagekitqt5.so

CONFIG   += c++11

lupdate_only{
SOURCES += *.qml \
           *.cpp
}

TRANSLATIONS+=app_zh.ts
