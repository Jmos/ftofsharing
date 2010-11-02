CONFIG += plugin \
 qt \
 thread \
 warn_on \
 release
HEADERS = lhexternalapp.h \
 lhfiletools.h \
 lhhtmltools.h \
 lhmail.h \
 lhmailaddr.h \
 lhmailbase.h \
 lhmailbrowser.h \
 lhmailcert.cpp \
 lhmailerr.h \
 lhmailheader.h \
 lhmailkey.cpp \
 lhmailpart.h \
 lhmailsigcheck.h \
 lhmailsignature.h \
 lhmailsocket.h \
 lhmime.h \
 lhpop3.h \
 lhsmtp.h \
 lhsmtpauth.h \
 mime-types.h
QT += core network
SOURCES = lhexternalapp.cpp \
 lhfiletools.cpp \
 lhhtmltools.cpp \
 lhmail.cpp \
 lhmailaddr.cpp \
 lhmailbase.cpp \
 lhmailbrowser.cpp \
 lhmailcert.cpp \
 lhmailerr.cpp \
 lhmailheader.cpp \
 lhmailkey.cpp \
 lhmailpart.cpp \
 lhmailsigcheck.cpp \
 lhmailsignature.cpp \
 lhmailsocket.cpp \
 lhmime.cpp \
 lhpop3.cpp \
 lhsmtp.cpp \
 lhsmtpauth.cpp
TARGET = lhmail
TEMPLATE = lib
mac {
 CONFIG -=  lib  plugin
}
unix {
 MOC_DIR =  .moc
 OBJECTS_DIR =  .obj
 UI_DIR =  .ui
}
