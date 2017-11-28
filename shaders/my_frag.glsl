#version 330

precision highp float;

uniform float time;
uniform vec2  dims;
uniform vec2  rdims;
uniform vec2  mouse;

in  vec2 pos;

out vec4 col;


mat2
projector( vec2 v ) {

	return outerProduct( v, v ) / dot( v, v );
}

mat2
lot( vec2 v ) {

	return mat2( 1. ) - projector( v );
}

vec4
line( vec4 p_line, vec4 p_fg, float p_thickness) {

	vec2
	l = p_line.zw - p_line.xy,
	r = pos - p_line.xy;

	vec2
	d = lot( l ) * r;

	return ( 1. - smoothstep( -p_thickness, p_thickness, length( d ) ) ) * p_fg;
}

vec4
rect( vec4 p_rect, vec4 p_fg ) {

	return
		p_rect.x <= pos.x && p_rect.y <= pos.y && pos.x < p_rect.z && pos.y < p_rect.w ? p_fg : vec4( 0. );
}

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
	t = 20. * ( 1. + sin( -.5 * 3.1415 + .03 * time ) );

	vec2
	p = 2. * ( pos * rdims.y );
	p -= vec2( rdims.y * dims.x, 1 );


	if( abs( pos.x - mouse.x ) < dims.x / 8 && abs( pos.y - mouse.y ) < dims.y / 8 )

		p *= .95;
	//p.x += time;

	vec2
	c = p / pow( 2, .5 * t );

	c.x += 1.5;

	vec2
	z = c;

	col = vec4( vec3( 1. - mandel( c, z, 128 ) ), 1 );

	col = abs( pos.x - mouse.x ) < dims.x / 4 && abs( pos.y - mouse.y ) < dims.y / 4 && abs( pos.x - mouse.x ) > dims.x / 8 && abs( pos.y - mouse.y ) > dims.y / 8 ? col : 1-col;
}

