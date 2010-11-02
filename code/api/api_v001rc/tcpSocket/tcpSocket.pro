################################ Template ####################################

TEMPLATE = app

############################# Header Files ##################################

HEADERS += ./include/tcpsocket.h \
           ./include/pop3client.h \ 

############################# Source Files ##################################

SOURCES +=  ./main.cpp \
            ./src/tcpsocket.cpp \
            ./src/pop3client.cpp \
            

############################# IncludePath  ##################################

INCLUDEPATH += ./include \
               ./src 

############################# Resource Files ################################

TARGET = ./tcpSocket
TARGET += ./tcpSocket

############################# Dependencies ##################################

DEPENDPATH += ;$$INCLUDEPATH

############################# QT Libraries ##################################

QT += core xml xmlpatterns network gui

########################## Configuration Options ############################

CONFIG +=   warn_on \
            qt \
            silent \
            resources \
            console \
            release \
            thread  \

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
