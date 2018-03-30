#version 330 core

precision highp float;
precision highp int;

uniform float
time;

in vec2
vPos;

out int
fCol;

int
mb( vec2 c, int mi, float r2 ) {

int
	i = 0;

vec2
	z = c,
	d = c * c;

while( d.x + d.y < r2 && ++i <= mi ) {

c = vec2( d.x - d.y, 2 * c.x * c.y ) - z;
		d = c * c;
	}

return i;
}

void
main( void ) {

//	fCol = int( 32 * log2( float( mb( vPos, 256, 256. ) ) ) );
fCol = mb( vPos, 256, pow( 256., sin( .1 * time ) ) );
}
