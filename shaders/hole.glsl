#version 330

precision highp float;

in vec2 oimouse;
in vec2 oidims;
in vec2 oirecidims;
in vec2 oipos;


out vec4
oCol;

uniform float time;

mat2
projector( vec2 v ) {

	return outerProduct( v, v ) / dot( v, v );
}

mat2
lot( vec2 v ) {

	return mat2( 1. ) - projector( v );
}

vec3
hole( vec2 pos, float rad, vec3 col ) {

	return
	( vec3( 0 ) + ( vec3( 1 ) - col ) * ( smoothstep( 0, .25 * min( oidims.x, oidims.y ), length( oimouse - oipos ) ) ) );
}

vec3
checkerboard( vec3 ray ) {

	return vec3( 1. );
}


void
main( void ) {

	vec3
	eye  = vec3( 0., 5., 5. );

	vec3
	pos = vec3( 2. * oipos * oirecidims - 1., 0. ),
	col = checkerboard( pos - eye );

	oCol = vec4( col, 1. );
}
