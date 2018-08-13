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

uniform
float
time;

uniform
vec3
light;

out vec4
vPos,
vVerts,
vNorms,
vColors,
vLight;

void
main( void ) {

	mat4
	mv  = view * model,
	mvp = projection * mv;

	mat3
	norm = mat3( transpose( inverse( mv ) ) );

//	vVerts  = mvp * vec4( vertices, 1. );
	vPos= vec4( vertices, 1. );
	vVerts = mvp * vPos;
	vNorms  = normalize( projection * vec4( norm * norms, 0. ) );
	vColors = vec4( colors, 1. );
//	vLight = mvp * vec4( 2. * cos( 3. * time ), 2. * cos( .1 * time ), 2. * sin( -3. * time ), 1. );
	vLight = mvp * vec4( light, 1. );

	gl_Position = vVerts;
}
