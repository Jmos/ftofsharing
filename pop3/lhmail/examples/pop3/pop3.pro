TEMPLATE	= app
CONFIG		+= qt warn_on debug
HEADERS		= receiver.h
		  
SOURCES		= main.cpp receiver.cpp
		  
TARGET		= pop3

INCLUDEPATH += ../..

unix:LIBS += -llhmail
unix:QMAKE_LIBDIR += ../..
win32:LIBS += -llhmail
win32:QMAKE_LIBDIR += ..\..

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

