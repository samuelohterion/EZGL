#version 330 core
precision highp float;
precision highp int;
uniform float time;
in vec2 vVertsNormed;
in vec2 vVerts;
out vec4 fColor;
float random ( in vec2 st) {
	return 1000. * fract( .001 * sin( dot( st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}
void
main( void ) {
	fColor = vec4( 0,0,0,1);
	vec2 vScaled = .5*(vVertsNormed+1.);
	if(abs(vVertsNormed.x) >1 || abs(vVertsNormed.y) >1)
		discard;
	vScaled = vScaled;
	float c = 0.5*(cos(time*sin(vVertsNormed.x*cos(random(vVertsNormed))) )+1);
	float d = 0.5*(cos(time*sin(vVertsNormed.y))+1);
	fColor = vec4(c,d,.5,1);
	vec3 norm = vec3(random(vScaled)/cos(time)*0.1,0,1);
	vec3 src =  vec3(sin(sin(time)),1. - cos(sin(time)),1.);
	vec3 dir = src-vec3(vVertsNormed,0.);
	float a = dot(dir, norm)/pow(dot(dir,dir),6)+.2;
	vec3 red =vec3(1,.5,-1);
	fColor = vec4(clamp( a*( red*fColor.xyz ), vec3( 0 ), vec3( 1 )),1);
}
