################################ Template ####################################

TEMPLATE = app

############################# Header Files ##################################

HEADERS += ./include/udpsocket.h \

############################# Source Files ##################################

SOURCES +=  ./main.cpp \
            ./src/udpsocket.cpp \
            

############################# IncludePath  ##################################

INCLUDEPATH += ./include \
               ./src 

############################# Resource Files ################################

TARGET = ./udpSocket
TARGET += ./udpSocket

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
            release

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
