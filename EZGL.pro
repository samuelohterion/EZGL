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
#glprojects/triangle2d/triangle2d.cpp \
#glprojects/quad3d/quad3d.cpp \
#glprojects/coordinateSystem/coordinatesystem.cpp \
#glprojects/cubewithtexture/cubewithtexture.cpp \
#glprojects/zbuffertexture/zbuffertexture.cpp \
#glprojects/cube/cube.cpp \
#glprojects/funIn2D/funIn2D.cpp \
#	glprojects/lighttest/lighttest.cpp \
#	glprojects/sphere/sphere.cpp \
#glprojects/spheredynamicsurface/spheredynamicsurface.cpp \
#	glprojects/robertslighttest2D/robertslighttest2D.cpp \
#	glprojects/ballandlight/ballandlight.cpp \
#	glprojects/ballandlight/interaction.cpp \
#	glprojects/texturetest/texturetest.cpp \
#glprojects/multisampling/multisampling.cpp \
#glprojects/sometext/sometext.cpp \
#glprojects/yetanothersphere/yetanothersphere.cpp \
#	glprojects/spherewithtexture/spherewithtexture.cpp \
#glprojects/solarsystem/solarsystem.cpp \
#glprojects/space/space.cpp \
glprojects/dummy/dummy.cpp \
#glprojects/sceneexample/sceneexample.cpp \
#glprojects/obj/obj.cpp \
#glprojects/mstest/mstest.cpp \
glprojects/fbtest/fbtest.cpp

HEADERS += \
code/widget.hpp \
code/clock.hpp \
code/glmprinter.hpp \
code/glr.hpp \
code/objimporter.hpp \
code/filesys.hpp \
#glprojects/triangle2d/triangle2d.hpp \
#glprojects/quad3d/quad3d.hpp \
#glprojects/coordinateSystem/coordinatesystem.hpp \
#glprojects/cubewithtexture/cubewithtexture.hpp \
#glprojects/zbuffertexture/zbuffertexture.hpp \
#glprojects/multisampling/multisampling.hpp \
#glprojects/cube/cube.hpp \
#glprojects/funIn2D/funIn2D.hpp \
#	glprojects/lighttest/lighttest.hpp \
#	glprojects/sphere/sphere.hpp \
#glprojects/spheredynamicsurface/spheredynamicsurface.hpp \
#	glprojects/robertslighttest2D/robertslighttest2D.hpp \
#	glprojects/ballandlight/ballandlight.hpp \
#	glprojects/ballandlight/interaction.h \
#	glprojects/texturetest/texturetest.hpp \
#glprojects/sometext/sometext.hpp \
#glprojects/yetanothersphere/yetanothersphere.hpp \
#	glprojects/spherewithtexture/spherewithtexture.hpp \
#glprojects/solarsystem/solarsystem.hpp \
#glprojects/space/space.hpp \
glprojects/dummy/dummy.hpp \
#glprojects/sceneexample/sceneexample.hpp \
#glprojects/obj/obj.hpp \
#glprojects/mstest/mstest.hpp \
glprojects/fbtest/fbtest.hpp


FORMS += \
code/widget.ui

DISTFILES += \
glprojects/funIn2D/fun1.vsh \
glprojects/funIn2D/fun1.fsh \
glprojects/robertslighttest2D/robertslighttest2D.vsh \
glprojects/robertslighttest2D/robertslighttest2D.fsh
