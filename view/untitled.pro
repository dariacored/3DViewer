QT += core gui opengl widgets
LIBS += -lGLU -lgif
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20
QMAKE_CXXFLAGS += -msse -mavx
SOURCES += \
    main.cc \
    mainwindow.cc \
    ../controller/facade.cc \
    ../model/edge.cc \
    ../model/figure.cc \
    ../model/objparser.cc \
    ../model/transformmatrix.cc \
    ../model/transformmatrixbuilder.cc \
    ../model/vertex.cc \
    qtscenedrawer.cc \
    myglwidget.cc \
    gifrecorder.cc \
    ../model/point.cc


    

HEADERS += \
    mainwindow.h \
    ../model/model.h \
    ../controller/facade.h \
    qtscenedrawer.h \
    scenedrawerbase.h \
    myglwidget.h \
    gifrecorder.h

FORMS += \
    mainwindow.ui

# Добавление шрифтов в ресурсы
RESOURCES += resources.qrc


