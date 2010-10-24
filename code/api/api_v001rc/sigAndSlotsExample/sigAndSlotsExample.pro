################################ Template ####################################

TEMPLATE = app

############################# Header Files ##################################

HEADERS +=	include/example.h

############################# Source Files ##################################

SOURCES += 	src/example.cpp \
            src/main.cpp

############################# Resource Files ##################################

TARGET = ./release/sigslotexample
TARGET += ./release/sigslotexample

############################# Dependencies ##################################

INCLUDEPATH += ./include \
               ./src    \

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
