#version 330 core

precision highp float;
precision highp int;

in vec2
vertex;

out vec2
vVerts,
vVertsNormed;

uniform int
width,
height;

void
main( void ) {

	vec2
	ratio = vec2( width, height ) / ( width < height ? width : height );

	vVerts = vertex;
	vVertsNormed = ratio * vertex;

	gl_Position = vec4( vertex, 0, 1 );
}
