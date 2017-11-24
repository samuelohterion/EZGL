#-------------------------------------------------
#
# Project created by QtCreator 2017-11-24T10:00:00
#
#-------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShaderFun
TEMPLATE = app
CONFIG += c++11

SOURCES += \
	main.cpp \
	widget.cpp \
	shaderprogram.cpp

HEADERS += \
	widget.hpp \
	clock.hpp \
	shaderprogram.hpp

FORMS += \
	widget.ui

DISTFILES += \
	../shaders/frag_es_2_0.glsl \
	../shaders/vert_es_2_0.glsl \
	../shaders/frag_desktop.glsl \
	../shaders/vert_desktop.glsl \
	../shaders/frag.glsl \
	../shaders/vert.glsl \
	../shaders/hole.glsl