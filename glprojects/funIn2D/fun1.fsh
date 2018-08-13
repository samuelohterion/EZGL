#version 330 core

precision highp float;
precision highp int;

uniform float
time;

in vec2
vVerts,
vVertsNormed;

out vec4
fCol;

void
main( void ) {

	float
	t = sin( 6.28 * sin( 32. * sin( .1 * time ) ) ),
	s = sin( t ),
	c = cos( t );

	mat2
	rot = mat2( c, s, -s, c );

	vec2
	v = rot * vVertsNormed;

	vec2
	col2d = cos( ( 201. + 200 * sin( 0.11 * time ) ) * vVertsNormed / pow( dot( vVertsNormed, vVertsNormed ), sin( .1 * time ) ) );

	fCol = vec4( col2d.x, col2d.x + col2d.y, col2d.x * col2d.y, 1. );
	fCol = abs( v.x ) < .707 && abs( v.y ) < .707 ? fCol : 1. - fCol;

	fCol /= fCol.a;
}
