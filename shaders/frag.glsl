#version 330

precision highp float;

uniform float time;
uniform vec2 dims;
uniform vec2 rdims;
uniform vec2 mouse;

in vec2 pos;
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

vec2
lens( vec2 center, float rad, float dist, float n ) {

	vec2
	d = pos - center;

	float
	h = sqrt( rad * rad - dot( d, d ) );

	vec3
	s = vec3( d, h );


}

void
main( void ) {

	float
	t = time;

	col = rect( vec4( 20., 20., dims - 20. ), vec4( 1,1,1,0 ) );

	col += vec4( vec3( 1., .5, .25 ) * smoothstep( 0, .25 * min( dims.x, dims.y ), length( mouse - pos ) ), 1. );

	col += line( vec4( 0,0, mouse ), vec4( 1,0,0,1 ), 1. ) + line( vec4( 0, dims.y, mouse ), vec4( 1,1,0,1 ), 2. )+line( vec4( dims.x,0, mouse ), vec4( 0,1,0,1 ), 4. ) + line( vec4( dims, mouse ), vec4( 0,0,1,1 ), 8. );

	col /= col.a;
}

