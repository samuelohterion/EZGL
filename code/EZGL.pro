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

SOURCES += \
	main.cpp \
	widget.cpp \
    myglproject.cpp

HEADERS += \
	widget.hpp \
	clock.hpp  \
	glrenderer.hpp \
    myglproject.hpp \
    funwithtextures.hpp

FORMS += \
	widget.ui

DISTFILES += \
	../shaders/frag_es_2_0.glsl \
	../shaders/vert_es_2_0.glsl \
	../shaders/frag_desktop.glsl \
	../shaders/vert_desktop.glsl \
	../shaders/my_frag.glsl \
	../shaders/frag.glsl \
	../shaders/vert.glsl \
	../shaders/hole.glsl \
	../shaders/ubo_frag.glsl \
	../shaders/ubo_vert.glsl \
	../shaders/m_frag.glsl \
	../shaders/m_vert.glsl \
	../shaders/raytrace.vsh \
	../shaders/raytrace.fsh \
	../shaders/stage1.vsh \
	../shaders/stage1.fsh \
	../shaders/stage2.vsh \
	../shaders/stage2.fsh \
	../shaders/stage3.vsh \
	../shaders/stage3.fsh \
	../shaders/fun1.vsh \
	../shaders/fun1.fsh \
    ../shaders/mousecontrolled.vsh \
    ../shaders/phong.fsh
