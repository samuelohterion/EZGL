#version 330

precision highp float;

layout( location = 0 ) in vec2 in_position;

uniform vec2
dims;

uniform vec2
mouse;

uniform float
time,
zoom;

int
buttons;

out vec2 pos;

void
main( ) {

	gl_Position = vec4( in_position, 0., 1. );
//	pos = .5 * ( 1. + in_position ) * ( dims + sin( time ) );
	pos = .5 * ( 1. + in_position ) * ( dims - 1. );
}
