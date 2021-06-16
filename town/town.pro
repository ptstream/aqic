QT -= gui
QT += positioning

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

SOURCES += \
    airqualities.cpp \
    airquality.cpp \
    aqlegend.cpp \
    town.cpp \
    towncodes.cpp \
    towns.cpp

HEADERS += \
    airqualities.hpp \
    airquality.hpp \
    aqlegend.hpp \
    town.hpp \
    towncodes.hpp \
    towns.hpp

include(../optimize.pri)
LIBNAME = tools
include (../pretargetdeps.pri)
LIBNAME = qtools
include (../pretargetdeps.pri)
