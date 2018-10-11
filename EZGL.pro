#-------------------------------------------------
#
# Project created by QtCreator 2017-11-24T10:00:00
#
#-------------------------------------------------

QT += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EZGL
TEMPLATE = app
CONFIG += c++11

#INCLUDEPATH += ../soil/src
#LIBPATH += ../soil/lib
#LIBS += -L -llibSOIL

SOURCES += \
code/main.cpp \
code/widget.cpp \
code/glmprinter.cpp \
glprojects/dummy/dummy.cpp \
glprojects/triangle2d/triangle2d.cpp \
glprojects/quad3d/quad3d.cpp \
glprojects/coordinateSystem/coordinatesystem.cpp \
glprojects/cubewithtexture/cubewithtexture.cpp \
glprojects/solarsystem/solarsystem.cpp \
glprojects/zbuffertexture/zbuffertexture.cpp \
glprojects/cube/cube.cpp \
glprojects/funIn2D/funIn2D.cpp \
glprojects/lightedcheckerboardcave/lightedcheckerboardcave.cpp \
glprojects/sphere/sphere.cpp \
glprojects/spherewithtexture/spherewithtexture.cpp \
glprojects/sometext/sometext.cpp \
glprojects/yetanothersphere/yetanothersphere.cpp \
glprojects/texturetest/texturetest.cpp \
glprojects/obj/obj.cpp \
#glprojects/ballandlight/ballandlight.cpp \
#glprojects/ballandlight/interaction.cpp \
#glprojects/spheredynamicsurface/spheredynamicsurface.cpp \
#glprojects/robertslighttest2D/robertslighttest2D.cpp \
#glprojects/multisampling/multisampling.cpp \
#glprojects/space/space.cpp \
#glprojects/sceneexample/sceneexample.cpp \
#glprojects/mstest/mstest.cpp \
glprojects/fbtest/fbtest.cpp

HEADERS += \
code/widget.hpp \
code/clock.hpp \
code/glmprinter.hpp \
code/glr.hpp \
code/objimporter.hpp \
code/filesys.hpp \
glprojects/dummy/dummy.hpp \
glprojects/triangle2d/triangle2d.hpp \
glprojects/quad3d/quad3d.hpp \
glprojects/coordinateSystem/coordinatesystem.hpp \
glprojects/cubewithtexture/cubewithtexture.hpp \
glprojects/solarsystem/solarsystem.hpp \
glprojects/zbuffertexture/zbuffertexture.hpp \
glprojects/cube/cube.hpp \
glprojects/funIn2D/funIn2D.hpp \
glprojects/lightedcheckerboardcave/lightedcheckerboardcave.hpp \
glprojects/sphere/sphere.hpp \
glprojects/spherewithtexture/spherewithtexture.hpp \
glprojects/sometext/sometext.hpp \
glprojects/yetanothersphere/yetanothersphere.hpp \
glprojects/texturetest/texturetest.hpp \
glprojects/obj/obj.hpp \
#glprojects/ballandlight/ballandlight.hpp \
#glprojects/ballandlight/interaction.h \
#glprojects/multisampling/multisampling.hpp \
#glprojects/spheredynamicsurface/spheredynamicsurface.hpp \
#	glprojects/robertslighttest2D/robertslighttest2D.hpp \
#glprojects/space/space.hpp \
#glprojects/sceneexample/sceneexample.hpp \
#glprojects/mstest/mstest.hpp \
glprojects/fbtest/fbtest.hpp


FORMS += \
code/widget.ui

DISTFILES += \
glprojects/funIn2D/fun1.vsh \
glprojects/funIn2D/fun1.fsh \
glprojects/robertslighttest2D/robertslighttest2D.vsh \
glprojects/robertslighttest2D/robertslighttest2D.fsh
