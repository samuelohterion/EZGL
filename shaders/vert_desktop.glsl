#version 330

precision highp float;

in vec2 iPos;
uniform int  width;
uniform int  height;

out vec2
oidims;

out vec2
oirecidims;

out vec2
oipos;

void
main( void ) {

	gl_Position = vec4( iPos, 0., 1. );
	oidims = vec2( width, height );
	oirecidims = vec2( 1./ width, 1. / height );
	oipos = .5 * ( vec2( 1. ) + iPos ) * ( oidims - 1. );
}

