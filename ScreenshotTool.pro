QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    drawbox.cpp \
    hook.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    drawbox.h \
    hook.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = $$PWD/Icon/app.ico

RESOURCES += \
    Resources.qrc

# 使用了钩子需要显式添加依赖
LIBS += -lUser32

