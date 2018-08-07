#version 330 core

precision highp float;
precision highp int;

uniform float
time;

in vec4
vNorms,
vColors;

out vec4
fCol;

void
main( void ) {

	fCol = vColors;
}
