TEMPLATE	= app
CONFIG		+= qt warn_on debug
HEADERS		= composer.h
		  
SOURCES		= main.cpp composer.cpp 
		  
TARGET		= smtp

INCLUDEPATH += ../..

unix:LIBS += -llhmail
unix:QMAKE_LIBDIR += ../..

win32:LIBS += -llhmail
win32:QMAKE_LIBDIR += ..\..

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  LIBS += /home/iwasz/openssl-0.9.8/libssl.a /home/iwasz/openssl-0.9.8/libcrypto.a
  
}

