QT       += core gui
unix {
    QT += gui-private
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# "embed" uglobalhotkey
DEFINES += UGLOBALHOTKEY_NOEXPORT

SOURCES += \
    UGlobalHotkey/uexception.cpp \
    UGlobalHotkey/uglobalhotkeys.cpp \
    UGlobalHotkey/ukeysequence.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    UGlobalHotkey/hotkeymap.h \
    UGlobalHotkey/uexception.h \
    UGlobalHotkey/uglobal.h \
    UGlobalHotkey/uglobalhotkeys.h \
    UGlobalHotkey/ukeysequence.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

include(UGlobalHotkey/uglobalhotkey-libs.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#SUBDIRS += \
#    UGlobalHotkey/uglobalhotkey.pro
#
#DISTFILES += \
#    UGlobalHotkey/README.md \
#    UGlobalHotkey/uglobalhotkey-headers.pri \
#    UGlobalHotkey/uglobalhotkey-libs.pri \
#    UGlobalHotkey/uglobalhotkey-sources.pri \
#    UGlobalHotkey/uglobalhotkey.pri
