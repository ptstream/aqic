CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

!win32-g++: {
DEFINES += _USE_MATH_DEFINES
}

include(../optimize.pri)

SOURCES += \
    geoconverter.cpp \
    lsl.cpp

HEADERS += \
    geoconstants.hpp \
    geoconverter.hpp \
    geoconverter_templates.hpp \
    globalinstance.hpp \
    kdtree.hpp \
    kdtree_impl.hpp \
    lsl.hpp \
    status.hpp \
    vector.hpp
