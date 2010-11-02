TEMPLATE	= app
CONFIG		+= qt warn_on debug thread
HEADERS		= browser.h
		  
SOURCES		= main.cpp browser.cpp
		  
TARGET		= browser

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

