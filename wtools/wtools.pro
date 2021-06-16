QT += widgets charts

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

include(../optimize.pri)

SOURCES += \
    magglass.cpp \
    player.cpp \
    slider.cpp \
    sliderstyle.cpp \
    tickvalues.cpp \
    waitwidget.cpp

HEADERS += \
    magglass.hpp \
    player.hpp \
    slider.hpp \
    sliderstyle.hpp \
    tickvalues.hpp \
    waitwidget.hpp

FORMS += \
  player.ui

RESOURCES += \
  wtools.qrc
