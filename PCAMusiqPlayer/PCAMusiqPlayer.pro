######################################################################
# Automatically generated by qmake (2.01a) Wed Sep 5 17:30:24 2012
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . external/cnpy external

HEADERS += MainWindow.h PCAPlayer.h external/cnpy/cnpy.h \
    PCA.h \
    LevelView.h \
    Config.h \
    MathUtil.h \
    CVUtil.h \
    OscSender.h \
    PvAPI.h \
    VideoSource/VideoSource.h \
    VideoSource/VideoSourceFile.h \
    VideoSource/VideoSourceProsilicaCamera.h

SOURCES += main.cpp MainWindow.cpp PCAPlayer.cpp external/cnpy/cnpy.cpp \
    PCA.cpp \
    LevelView.cpp \
    CVUtil.cpp \
    OscSender.cpp \
    VideoSource/VideoSource.cpp \
    VideoSource/VideoSourceFile.cpp \
    VideoSource/VideoSourceProsilicaCamera.cpp

LIBS += -L$$PWD/bin -lPvAPI

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv liblo
}

mac {
  CONFIG -= app_bundle
  LIBS += -L/usr/local/Cellar/boost/1.51.0/lib -lboost_program_options-mt
  DEFINES += _OSX _x64 #for PvAPI
}

QT += opengl

OTHER_FILES +=

unix:*-g++*: QMAKE_CXXFLAGS += -fpermissive # OS X SDK Fix
