#version 330 core

precision highp float;
precision highp int;

in vec2
vertex;

out VS2FS {

	vec2
	vertex,
	vertexNormed;
} vs2fs;

uniform int
width,
height;

void
main( void ) {

	vec2
	ratio = vec2( width, height ) / ( width < height ? width : height );

	vs2fs.vertex = vertex;
	vs2fs.vertexNormed = ratio * vertex;

	gl_Position = vec4( vertex, 0, 1 );
}
