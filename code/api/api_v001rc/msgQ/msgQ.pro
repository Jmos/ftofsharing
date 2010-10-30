################################ Template ####################################

TEMPLATE = app

############################# Header Files ##################################

HEADERS += ./include/msgQ.h \
           ./include/msgList.h

############################# Source Files ##################################

SOURCES +=  ./main.cpp \
            ./src/msgQ.cpp \
            ./src/msgList.cpp

############################# IncludePath  ##################################

INCLUDEPATH += ./include \
               ./src 

############################# Resource Files ################################

TARGET = ./msgQ
TARGET += ./msgQ

############################# Dependencies ##################################

DEPENDPATH += ;$$INCLUDEPATH

############################# QT Libraries ##################################

QT += core xml xmlpatterns network

########################## Configuration Options ############################

CONFIG +=   warn_on \
            qt \
            silent \
            resources \
            console \
            network

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
