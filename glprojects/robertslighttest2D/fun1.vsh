#version 330 core

precision highp float;
precision highp int;

in vec2
verts;

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

	vVerts = verts;
	vVertsNormed = ratio * verts;

	gl_Position = vec4( verts, 0, 1 );
}
