################################ Template ####################################

TEMPLATE = app

############################# Header Files ##################################

HEADERS +=

############################# Source Files ##################################

SOURCES +=  ./main.cpp

############################# Resource Files ##################################


TARGET = ./msgQ
TARGET += ./msgQ

############################# Dependencies ##################################

DEPENDPATH += ;$$INCLUDEPATH

############################# QT Libraries ##################################

QT += core xml xmlpatterns

########################## Configuration Options ############################

CONFIG +=   warn_on \
            qt \
            silent \
            resources \
            console \

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
