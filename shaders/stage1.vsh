#version 330 core

precision highp float;
precision highp int;

//uniform vec2
//vars[ 3 ];

uniform vec2
ratio,
cntr;

uniform float
zoom;

in vec2
verts;

out vec2
vPos;

void
main( void ) {

	vPos = ratio * ( zoom * verts - cntr );

	gl_Position = vec4( verts, 0, 1 );
}
