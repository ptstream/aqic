# Copy files needed.
win32 {
CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release/
CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/debug/
DESTDIR=$$replace(DESTDIR, /, \\)
_WIN_PRO_FILE_PWD_=$$replace(_PRO_FILE_PWD_, /, \\)
QMAKE_POST_LINK=$$_PRO_FILE_PWD_\dependencies\dep.cmd $$_WIN_PRO_FILE_PWD_ $$DESTDIR
}

#Copy files needed.
unix {
QMAKE_POST_LINK=$$_PRO_FILE_PWD_/dependencies/dep.sh $$_PRO_FILE_PWD_ $$OUT_PWD
}

#Copy files needed.
macx {
QMAKE_POST_LINK=$$_PRO_FILE_PWD_/dependencies/dep.sh $$_PRO_FILE_PWD_ $$OUT_PWD/aqic.app/Contents/MacOS
}

# To convert atmpol64.png https://anyconv.com/png-to-icns-converter#
macx:ICON=$$PWD/aqic/icons/aqic128.icns
