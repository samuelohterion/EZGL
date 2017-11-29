#version 330

precision highp float;


uniform vec2
dims;

uniform vec2
mouse;

uniform float
time,
zoom;

int
buttons;


in vec2 pos;

layout( location = 0 ) out vec4 col;

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

	return iter > maxiter ? 0 : 1. * iter / maxiter;
}

const int
maxiter = 256;

void
main( ) {

	float
	t = 30. * ( 1. + sin( -.5 * 3.1415 + .03 * time ) );

	vec2
	p = 2. * ( pos / dims.y ),
	m = 2. * ( mouse / dims.y );

	p -= vec2( dims.x / dims.y, 1 );
	m -= vec2( dims.x / dims.y, 1 );

	p *= 2.;
	m *= 2.;

	float
	fc  = pow( 1.2, t ),
	cf  = 1 / fc,
	rd  = cf,
	rd2 = rd * rd;

	p *= cf;
	m *= cf;


	p -= vec2( -1.465, 0 );
	m -= vec2( -1.465, 0 );

	vec2
	d  = zoom * ( p - m );

	float
	dd = dot( d, d );

	if( dd < rd2 ) {

		vec3
		r = vec3( d, sqrt( rd2 - dd ) );

		vec3
//		rf = refract( vec3( 0, 0, -1 ), normalize( r ), zoom ); //diamant 1 / 2.5
		rf = refract( vec3( 0, 0, -1 ), normalize( r ), .8 / zoom ); //diamant 1 / 2.5

		rf *= r.z;

		p = p.xy + rf.xy;
	}

	float
	f = mandel( p.xy, p.xy, maxiter ),
	fcol = log( maxiter * f ) / log( maxiter ),
	fcolmod = 5. * mod( fcol, .2 );

	col = vec4( fcol, fcolmod, fcolmod, 1 );

	col *= dd < rd2 ? .5 + .5 * sqrt( 1 - dd * fc * fc ) : 1.;
}

