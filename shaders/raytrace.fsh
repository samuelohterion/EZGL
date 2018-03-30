#version 330 core

precision highp float;

int
mndlbrt( vec2 z, int maxiter, float rad2 ) {

	vec2
	c = z;

	float
	d = dot( c, c );

	int
	iter = -1;

	while( ++iter < maxiter && d < rad2 ) {

		vec2
		p = c * c;

		d = p.x + p.y;

		c = vec2( p.x - p.y, 2. * c.x * c.y ) - z;
	}

	return iter;
}

uniform float
time,
zoom;

in vec2
vPos;

out vec4
fCol;

void
main( void ) {

//	eye + a * ray = ( r, -1, -i )
//	e.x + a * ray.x = r
//	e.y + a * ray.y = -1
//	e.z + a * ray.z = -i

//	      1 + e.y
//	a = - -------
//	       ray.y

//	r = eye + a * ray

//	 r         1 + eye.y
//	-1 = eye - --------- ray
//	-i          ray.y

	vec3
	eye = vec3( 0, 1., 3 ),
	ray = ( vec3( vPos, 1 )- eye );

	vec3
	c = eye - ( 1 + eye.y ) / ray.y * ray;

	float
	cs = cos( .1 * time ),
	sn = sin( .1 * time );

	c.z = 4+c.z;
	c.xz = vec2( cs * c.x + sn * c.z, -sn * c.x + cs * c.z );


	vec3
	b = 1. * fract( c *= zoom );

	vec3
	z = eye + ray,
	dz = ( c - z ) / 32.;

	int
	i = 33;

	while( 0 < --i ) {

		int
		mi = mndlbrt( vec2( z.x, z.z ), 32, 32 );

		vec3
		col = vec3( mi / 32., mi / 64., mi / 128. );

		float
		m = log2( 1 + mi / 32. );

		if( z.y + 1 < 1 * m ) {

			fCol = vec4( m,m,m, 1 );

//			float
//			m = mndlbrt( vec2( c.x, c.z ), 32, 32 ) / 32.;
//			fCol += vec4( b.xz * m, clamp( -ray.y, 0, 1 ) * ( b.x + b.y ), 1 );

//			fCol *= .5;

			return;
		}

		z += dz;
	}
	discard;
/*	float
	m = .5 * log2( 1 + mndlbrt( z.xz + dz.xz * 32, 32, 32 ) / 32. );
	fCol = vec4( b.xz * m, 0, 1 );
//	fCol = vec4( b.xz * m, clamp( -ray.y, 0, 1 ) * ( b.x + b.y ), 1 );
	//fCol = vec4( b, clamp( -ray.y, 0, 1 ) * ( b.x + b.y ), 1 );
//	fCol = vec4( vec2( mndlbrt( vPos + vec2( .68, 0. ), 100, 100 ) ), 0, 1 );
*/
}

