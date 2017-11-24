#version 330

precision highp float;

uniform int  width;
uniform int  height;
uniform int  mousex;
uniform int  mousey;

in vec2 iPos;

out vec2
oimouse;

out vec2
oidims;

out vec2
oirecidims;

out vec2
oipos;

void
main( void ) {

	gl_Position = vec4( iPos, 0., 1. );
	oimouse = vec2( mousex,mousey );
	oidims = vec2( width, height );
	oirecidims = vec2( 1./ width, 1. / height );
	oipos = .5 * ( vec2( 1. ) + iPos ) * ( oidims - 1. );
}

