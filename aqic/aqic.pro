QT += core gui widgets quickwidgets positioning location charts

CONFIG += c++11 no_lflags_merge

include(../optimize.pri)

SOURCES += \
    about.cpp \
    displaydata.cpp \
    downloaderdata.cpp \
    downloadstatistics.cpp \
    exportmovieresults.cpp \
    globallegend.cpp \
    imageexporterparams.cpp \
    legend.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindowevents.cpp \
    mainwindowslots.cpp \
    mapslots.cpp \
    mapview.cpp \
    moviedata.cpp \
    moviemanager.cpp \
    networktimeout.cpp \
    preferences.cpp \
    toolbar.cpp \
    towndetails.cpp \
    towntooltip.cpp

HEADERS += \
    about.hpp \
    appversion.hpp \
    displaydata.hpp \
    downloaderdata.hpp \
    downloadstatistics.hpp \
    exportmovieresults.hpp \
    globallegend.hpp \
    imageexporterparams.hpp \
    legend.hpp \
    mainwindow.hpp \
    mapview.hpp \
    moviedata.hpp \
    networktimeout.hpp \
    preferences.hpp \
    towndetails.hpp \
    towntooltip.hpp

FORMS += \
    about.ui \
    downloadstatistics.ui \
    exportmovieresults.ui \
    globallegend.ui \
    legend.ui \
    mainwindow.ui \
    networktimeout.ui \
    preferences.ui \
    towndetails.ui

DISTFILES += \
  circle.qml \
  dependencies/dep.cmd \
  dependencies/dep.sh \
  map.qml \
  polygon.qml

RESOURCES += \
  aqic.qrc

RC_FILE += \
    aqic.rc

DISTFILES += \
  aqic.rc

include(../optimize.pri)
LIBNAME = town
include(../pretargetdeps.pri)
include(../libneeded.pri)
LIBNAME = wtools
include(../pretargetdeps.pri)
include(../libneeded.pri)
LIBNAME = qtools
include(../pretargetdeps.pri)
include(../libneeded.pri)
LIBNAME = tools
include(../pretargetdeps.pri)
include(../libneeded.pri)
include(../postlink.pri)
