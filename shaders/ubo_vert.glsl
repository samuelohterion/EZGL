#version 330

precision highp float;

layout( std140 ) uniform VertVars {

	vec2
	dims;
};

layout( location = 0 ) in vec2 in_position;

out vec2 pos;

void
main( ) {

	gl_Position = vec4( in_position, 0., 1. );
//	pos = .5 * ( 1. + in_position ) * ( dims + sin( time ) );
	pos = .5 * ( 1. + in_position ) * ( dims - 1. );
}

