TEMPLATE = subdirs

EXETARGET = aqic
cache(EXETARGET, set)
CONFIG += ordered

SUBDIRS += \
  tools \
  qtools \
  town \
  wtools \
  aqic

TRANSLATIONS = $$PWD/languages/aqic_fr.ts
message (Build $$EXETARGET for $$QMAKE_HOST.os ($$QMAKE_HOST.arch))
