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
	glprojects/cube/cube.cpp \
	glprojects/funIn2D/funIn2D.cpp \
	glprojects/lighttest/lighttest.cpp \
	glprojects/coordinateSystem/coordinatesystem.cpp \
	glprojects/simpleTriangle/simpletriangle.cpp \
	glprojects/sphere/sphere.cpp \
	glprojects/spheredynamicsurface/spheredynamicsurface.cpp \
	glprojects/simplecoloredtriangle/simplecoloredtriangle.cpp \
	glprojects/robertslighttest2D/robertslighttest2D.cpp \
	glprojects/ballandlight/ballandlight.cpp \
	glprojects/ballandlight/interaction.cpp \
	glprojects/texturetest/texturetest.cpp \
	glprojects/multisampling/multisampling.cpp \
	glprojects/sometext/sometext.cpp \
	glprojects/yetanothersphere/yetanothersphere.cpp \
	glprojects/cubewithtexture/cubewithtexture.cpp \
	glprojects/spherewithtexture/spherewithtexture.cpp


HEADERS += \
	code/widget.hpp \
	code/clock.hpp \
	code/glrenderer.hpp \
	code/glmprinter.hpp \
	glprojects/cube/cube.hpp \
	glprojects/funIn2D/funIn2D.hpp \
	glprojects/lighttest/lighttest.hpp \
	glprojects/coordinateSystem/coordinatesystem.hpp \
	glprojects/simpleTriangle/simpletriangle.hpp \
	glprojects/sphere/sphere.hpp \
	glprojects/spheredynamicsurface/spheredynamicsurface.hpp \
	glprojects/simplecoloredtriangle/simplecoloredtriangle.hpp \
	glprojects/robertslighttest2D/robertslighttest2D.hpp \
	glprojects/ballandlight/ballandlight.hpp \
	glprojects/ballandlight/interaction.h \
	glprojects/texturetest/texturetest.hpp \
	glprojects/multisampling/multisampling.hpp \
	glprojects/sometext/sometext.hpp \
	glprojects/yetanothersphere/yetanothersphere.hpp \
	glprojects/cubewithtexture/cubewithtexture.hpp \
	glprojects/spherewithtexture/spherewithtexture.hpp

FORMS += \
	code/widget.ui

DISTFILES += \
	glprojects/cube/cube.vsh \
	glprojects/cube/cube.fsh \
	glprojects/cube/wall.vsh \
	glprojects/cube/wall.fsh \
	glprojects/funIn2D/fun1.vsh \
	glprojects/funIn2D/fun1.fsh \
	glprojects/robertslighttest2D/robertslighttest2D.vsh \
	glprojects/robertslighttest2D/robertslighttest2D.fsh
