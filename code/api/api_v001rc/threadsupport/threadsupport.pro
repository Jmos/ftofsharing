################################ Template ####################################

TEMPLATE = app

############################# Header Files ##################################

HEADERS +=	include/thread.h\
		include/test.h

############################# Source Files ##################################

SOURCES +=  src/main.cpp\
	    src/test.cpp

############################# Resource Files ##################################

TARGET = ./release/thread
TARGET += ./release/thread

############################# Dependencies ##################################

INCLUDEPATH += ./include \
               ./src    \
               ../msgQ/include

############################# Dependencies ##################################

DEPENDPATH += ;$$INCLUDEPATH

############################# XML2header ##################################

XML2HEADER=xxml2header

############################# QT Libraries ##################################

QT += core xml xmlpatterns

########################## Configuration Options ############################

CONFIG +=   warn_on \
            qt \
            silent \
            resources

###################### Intermediate Output Directories ######################

win32 {
    MOC_DIR = ./moc-rel
    OBJECTS_DIR = ./obj-rel
} else {
    MOC_DIR = ./lmoc-rel
    OBJECTS_DIR = ./lobj-rel
}

############################# Defines ##################################

DEFINES +=
DEFINES -= UNICODE

#############################################################################
