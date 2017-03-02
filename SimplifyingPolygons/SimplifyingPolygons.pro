#-------------------------------------------------
#
# Project created by QtCreator 2016-12-27T20:28:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimplifyingPolygons
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        mainwindow.cpp \
    display.cpp \
    polygon.cpp \
    point.cpp \
    ramerdouglaspeucker.cpp \
    visvalingamwhyatt.cpp \
    simplepolygon.cpp \
    aboutwindow.cpp \
    colorwindow.cpp

HEADERS  += mainwindow.h \
    display.h \
    polygon.h \
    point.h \
    ramerdouglaspeucker.h \
    visvalingamwhyatt.h \
    simplepolygon.h \
    aboutwindow.h \
    colorwindow.h

FORMS    += mainwindow.ui \
    aboutwindow.ui \
    colorwindow.ui

isEmpty(TARGET_EXT) {
    win32 {
        TARGET_CUSTOM_EXT = .exe
    }
    macx {
        TARGET_CUSTOM_EXT = .app
    }
} else {
    TARGET_CUSTOM_EXT = $${TARGET_EXT}
}

win32 {
    DEPLOY_COMMAND = windeployqt
}
macx {
    DEPLOY_COMMAND = macdeployqt
}

CONFIG( debug, debug|release ) {
    # debug
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
} else {
    # release
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGET_CUSTOM_EXT}))
}

#  # Uncomment the following line to help debug the deploy command when running qmake
#  warning($${DEPLOY_COMMAND} $${DEPLOY_TARGET})

# Use += instead of = if you use multiple QMAKE_POST_LINKs
QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}
