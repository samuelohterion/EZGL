#version 330 core

precision highp float;
precision highp int;

uniform float
time;

uniform mat4
model,
view,
projection;

in vec4
vPos,
vVerts,
vNorms,
vColors,
vLight;

out vec4
fCol;

void
main( void ) {

	vec3
	diff  = vLight.xyz - vVerts.xyz;

	float
//	a = pow( clamp( dot( vNorms.xyz, normalize( diff ) ), 0., 1e5f ) / dot( diff, diff ), 1. );
	a = max( 0.f, dot( vNorms.xyz, vec3( 0., 0., +1. ) ) );

//	vec3
//	v3 = 1. - vPos.xyz * vPos.xyz;

//	vec3
//	b = abs( v3.x ) < .1 ||
//		abs( v3.y ) < .1 ||
//		abs( v3.z ) < .1
//			? vec3( 1. ) : vec3( .5 );

//	fCol = vec4( clamp( 1. * ( a + .1 ) * vColors.xyz, 0., 1. ), 1. );
	//fCol = vec4( v3, 1. );
	fCol = vec4( 10. * a * vColors.xyz, 1. );
	//fCol = vec4( ( vNorms.xyz ), 1. );
	//fCol = vec4( ( abs( vNorms.xyz ) ), 1. );
}
