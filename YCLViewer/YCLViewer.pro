QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/../YukesCloth/CClothStructs.cpp \
    $$PWD/../YukesCloth/CSimMeshData.cpp \
    $$PWD/../YukesCloth/CSimObj.cpp \
    $$PWD/../YukesCloth/ClothContainer.cpp \
    $$PWD/../YukesCloth/include/BinaryIO.cpp \
    defwidgetitem.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    $$PWD/../YukesCloth/CClothStructs.h \
    $$PWD/../YukesCloth/CSimMeshData.h \
    $$PWD/../YukesCloth/CSimObj.h \
    $$PWD/../YukesCloth/ClothContainer.h \
    $$PWD/../YukesCloth/include/BinaryIO.h \
    $$PWD/../YukesCloth/YukesCloth \
    defwidgetitem.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += $$PWD/../YukesCloth
INCLUDEPATH += $$PWD/../YukesCloth/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DEFINES += DEBUG_DISP_BINARY
DEFINES += DEBUG_CONSOLE
