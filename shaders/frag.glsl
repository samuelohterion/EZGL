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

vec4
line( vec4 p_line, vec4 p_fg, float p_thickness) {

	vec2
	l = p_line.zw - p_line.xy,
	r = oipos - p_line.xy;

	vec2
	d = lot( l ) * r;

	return ( 1. - smoothstep( -p_thickness, p_thickness, length( d ) ) ) * p_fg;
}

vec4
rect( vec4 p_rect, vec4 p_fg ) {

	return
		p_rect.x <= oipos.x && p_rect.y <= oipos.y && oipos.x < p_rect.z && oipos.y < p_rect.w ? p_fg	: vec4( 0. );
}

void
main( void ) {

	float
	t = time;

	oCol = vec4( .1,0,0,1);

	oCol += vec4( vec3( 1., .5, .25 ) * smoothstep( 0, .25 * min( oidims.x, oidims.y ), length( oimouse - oipos ) ), 1. );

	oCol += line( vec4( 0,0, oimouse ), vec4( 1,0,0,1 ), 1. ) + line( vec4( 0, oidims.y, oimouse ), vec4( 1,1,0,1 ), 2. )+line( vec4( oidims.x,0, oimouse ), vec4( 0,1,0,1 ), 4. ) + line( vec4( oidims, oimouse ), vec4( 0,0,1,1 ), 8. );
}

