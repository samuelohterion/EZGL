#version 330

precision highp float;

in vec2 oidims;
in vec2 oirecidims;
in vec2 oipos;


out vec4
oCol;

uniform float time;

float
mandel( vec2 c, vec2 z, int maxiter	) {

	int
	iter = 0;

	float
	r1 = c.x * c.x,
	r2 = c.y * c.y;

	while( iter < maxiter && r1 + r2 < 64. ) {

		r1 = c.x * c.x,
		r2 = c.y * c.y;
		c = vec2( r1 - r2, 2. * c.x * c.y ) - z;
		++iter;
	}

	return 1. * iter / maxiter;
}

void
main( void ) {

	float
	t = sin( .1 * time );

	vec2
	z = 2.5 * ( ( oipos * oirecidims ) * 2 - 1 );

	//z.x += 2. * t;

	float
	m = log( 1. + 1.72 * mandel( .25 * z, t * z, 255 ) ),
	c = 1. + 4. * ( smoothstep( .99, 1., 8. * abs( t ) ) - smoothstep( 1., 1.01, 8. * abs( t ) ) );

	m = m < 1. ? m : 0.;

	float
	m2 = mandel( z, z, 255 ) == 1. ? .2 : 0.;

	oCol = vec4( vec3( m + c * m2, m + c * m2, m + c * m2 ), 1. );
}

