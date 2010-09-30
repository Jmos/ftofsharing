################################ Template ####################################

TEMPLATE = app

############################# Header Files ##################################

HEADERS +=	./data_defines.h

############################# Source Files ##################################

SOURCES += 	./main.cpp	

############################# Resource Files ##################################


TARGET = ./release/xmlDataLib
TARGET += ./release/xmlDataLib

############################# Dependencies ##################################

DEPENDPATH += ;$$INCLUDEPATH

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
