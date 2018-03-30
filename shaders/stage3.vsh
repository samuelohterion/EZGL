#version 330 core

precision highp float;
precision highp int;

in vec2
verts,
coords;

out vec2
vCoords;

void
main( void ) {

vCoords = vec2( coords.x, coords.y );

gl_Position = vec4( verts, 0, 1 );
}
