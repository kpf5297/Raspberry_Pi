QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lgpiod -lrt -lpthread

SOURCES += \
    ADS1115.cpp \
    LightController.cpp \
    Logging.cpp \
    SoilSensor.cpp \
    SystemController.cpp \
    SystemDriver.cpp \
    WaterPump.cpp \
    main.cpp \
    plantcaresystemgui.cpp

HEADERS += \
    ADS1115.h \
    LightController.h \
    Logging.h \
    SoilSensor.h \
    SystemController.h \
    WaterPump.h \
    plantcaresystemgui.h

FORMS += \
    plantcaresystemgui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
