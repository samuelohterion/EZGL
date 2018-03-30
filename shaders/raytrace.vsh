#version 330 core

in vec2
inV;

out vec2
vPos;

uniform vec2
dims;

void
main( void ) {

	gl_Position = vec4( inV, 0, 1 );
	vPos = inV;

	if( dims.x < dims.y )

		vPos *= vec2( 1., dims.y / dims.x );
	else

		vPos *= vec2( dims.x / dims.y, 1. );
}

