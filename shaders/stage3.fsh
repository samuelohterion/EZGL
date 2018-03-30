#version 330 core

precision highp float;
precision highp int;

uniform highp sampler2D
TX1;

uniform vec2
dimsRec;

in vec2
vCoords;

out vec4
fCol;

const float
M = 1. / 256.;

void
main( void ) {

vec2
	dx = vec2( dimsRec.x, 0 ),
	dy = vec2( 0, dimsRec.y );

float
	dxm = texture( TX1, vCoords - dx ).r,
	dxp = texture( TX1, vCoords + dx ).r,
	dym = texture( TX1, vCoords - dy ).r,
	dyp = texture( TX1, vCoords + dy ).r;

vec2
	g = vec2(
	dxm < dxp ? 1 : dxm == dxp ? 0 : -1,
		dym < dyp ? 1 : dym == dyp ? 0 : -1 );

vec2
	h = .5 * - .5 * ( g * ( 1 - g ) );

vec3
	col = vec3( h, ( g.r * g.g == 0 ) ? 0 : 1 );

//col *= texture( TX1, vCoords ).b;

//	fCol = vec4( 1 - col, 1 );
fCol = vec4( col, 1 );
}
