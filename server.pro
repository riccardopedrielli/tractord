######################################################################
# Tractor Server qmake project file
######################################################################

TEMPLATE = app
TARGET = tractord
DEPENDPATH += .
INCLUDEPATH += .
QT += network
QT += sql

# Input
HEADERS += md5.h TClientSocket.h TDatabase.h TParser.h TServer.h TThread.h
SOURCES += main.cpp md5.cpp TClientSocket.cpp TDatabase.cpp TParser.cpp TServer.cpp TThread.cpp
