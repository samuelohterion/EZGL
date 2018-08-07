#version 330 core

precision highp float;
precision highp int;

in vec3
vertices,
colors,
norms;

uniform mat4
mvp,
norm;

out vec4
vNorms,
vColors;

void
main( void ) {

//	vec2
//	ratio = vec2( width, height ) / ( width < height ? width : height );


	vNorms =  norm * vec4( norms, 1. );
	vColors = vec4( colors, 1. );//vec4( colors * .5 * ( clamp( dot( vNorms.xyz, vec3( 0., 0., 1. ) ), 0, 1 ) ), 1. );

	gl_Position = mvp * vec4( vertices, 1. );
}
