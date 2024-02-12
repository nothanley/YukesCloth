QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/../YukesCloth/src/Cloth/ClothStructs.cpp \
    $$PWD/../YukesCloth/src/Cloth/SimMesh.cpp \
    $$PWD/../YukesCloth/src/Cloth/SimObj.cpp \
    $$PWD/../YukesCloth/src/Container/ClothContainer.cpp \
    $$PWD/../YukesCloth/src/Encoder/ClothEncoder.cpp \
    $$PWD/../YukesCloth/src/Encoder/ClothSave.cpp \
    $$PWD/../YukesCloth/include/BinaryIO.cpp \
    defwidgetitem.cpp \
    main.cpp \
    mainwindow.cpp \
    nodeselectwindow.cpp

HEADERS += \
    $$PWD/../YukesCloth/src/Cloth/ClothStructs.h \
    $$PWD/../YukesCloth/src/Cloth/SimMesh.h \
    $$PWD/../YukesCloth/src/Cloth/SimObj.h \
    $$PWD/../YukesCloth/src/Container/ClothContainer.h \
    $$PWD/../YukesCloth/src/Encoder/ClothEncoder.h \
    $$PWD/../YukesCloth/src/Encoder/ClothSave.h \
    $$PWD/../YukesCloth/include/BinaryIO.h \
    $$PWD/../YukesCloth/YukesCloth \
    defwidgetitem.h \
    mainwindow.h \
    nodeselectwindow.h

FORMS += \
    mainwindow.ui \
    nodeselectwindow.ui

INCLUDEPATH += $$PWD/../YukesCloth
INCLUDEPATH += $$PWD/../YukesCloth/src
INCLUDEPATH += $$PWD/../YukesCloth/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DEFINES += DEBUG_DISP_BINARY
DEFINES += DEBUG_CONSOLE

RESOURCES += \
    icons.qrc
