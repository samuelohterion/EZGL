#version 330

precision highp float;

uniform float time;
uniform vec2  dims;
uniform vec2  mouse;

in vec2 vertex;
out vec2 pos;

void
main( void ) {

    gl_Position = vec4( vertex.x, vertex.y, 0., 1. );
    pos = .5 * ( vec2( 1. ) + vertex ) * ( dims - 1. );
}

