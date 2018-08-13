#version 330 core

precision highp float;
precision highp int;

in vec3
vertices,
colors,
norms;

uniform mat4
model,
view,
projection;

out vec4
vVerts,
vNorms,
vColors;

void
main( void ) {

	mat4
	mv  = view * model,
	mvp = projection * mv,
	norm = inverse( mvp ) ;

	vVerts  = mvp * vec4( vertices, 1. );
	vNorms  = vec4( norms, 1. ) * norm;
	vColors = vec4( colors, 1. );

	gl_Position = vVerts;
}
