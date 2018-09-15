#version 330 core

precision highp float;
precision highp int;

uniform float
time;

in VS2FS {

	vec2
	vertex,
	vertexNormed;
} vs2fs;

out vec4
fColor;

void
main( void ) {

	float
	t = sin( 6.28 * sin( 32. * sin( .1 * time ) ) ),
	s = sin( t ),
	c = cos( t );

	mat2
	rot = mat2( c, s, -s, c );

	vec2
	v = rot * vs2fs.vertexNormed;

	vec2
	col2d = cos( ( 201. + 200 * sin( 0.11 * time ) ) * vs2fs.vertexNormed / pow( dot( vs2fs.vertexNormed, vs2fs.vertexNormed ), sin( .1 * time ) ) );

	fColor = vec4( col2d.x, col2d.x + col2d.y, col2d.x * col2d.y, 1. );
	fColor = abs( v.x ) < .707 && abs( v.y ) < .707 ? fColor : 1. - fColor;
}
