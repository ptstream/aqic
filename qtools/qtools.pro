QT -= gui
QT += network quickwidgets quick qml positioning

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

SOURCES += \
    applocations.cpp \
    boundingbox.cpp \
    csvparser.cpp \
    datastream.cpp \
    downloadcache.cpp \
    downloader.cpp \
    externalipaddr.cpp \
    globals.cpp \
    imageexporter.cpp \
    ipaddrposition.cpp \
    lastcession.cpp \
    leastsquare.cpp \
    stringmanip.cpp \
    tinyhtml.cpp \
    translators.cpp \
    urls.cpp

HEADERS += \
    applocations.hpp \
    boundingbox.hpp \
    csvparser.hpp \
    datastream.hpp \
    downloadcache.hpp \
    downloader.hpp \
    externalipaddr.hpp \
    globals.hpp \
    imageexporter.hpp \
    ipaddrposition.hpp \
    lastcession.hpp \
    leastsquare.hpp \
    stringmanip.hpp \
    tinyhtml.hpp \
    translators.hpp \
    urls.hpp

include(../optimize.pri)
LIBNAME = tools
include (../pretargetdeps.pri)
