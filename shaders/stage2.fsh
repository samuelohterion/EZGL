#version 330 core

precision highp float;
precision highp int;

uniform highp isampler2D
TX1;

uniform float
zoom;

in vec2
vCoords;

out vec4
fCol;

void
main( void ) {

vec3
	d;

d.r  = texture( TX1, vCoords ).r;
	d.g  = floor( 32. * log2( d.r ) );
	d.b =  d.r / 256.;
	fCol = vec4( d, 1 );
}
