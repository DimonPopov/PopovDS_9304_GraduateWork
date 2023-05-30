QT       += core gui datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    abstractpointcontainer.cpp \
    antennamodel.cpp \
    controllPanel.cpp \
    errorcomplexityhandler.cpp \
    interpolation.cpp \
    main.cpp \
    mainwindow.cpp \
    scatterGraph.cpp

HEADERS += \
    abstractpointcontainer.h \
    antennamodel.h \
    controllPanel.h \
    errorcomplexityhandler.h \
    interpolation.h \
    mainwindow.h \
    scatterGraph.h \
    settingsDefine.h

FORMS += \
    controllPanel.ui \
    mainWidget.ui \
    mainwindow.ui

INCLUDEPATH += C:\Users\User\Downloads\boost_1_82_0\boost_1_82_0

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
