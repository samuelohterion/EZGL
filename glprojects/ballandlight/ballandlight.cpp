#include "ballandlight.hpp"

typedef glm::vec3 V3;
typedef glm::mat4 M4;
BallAndLight::BallAndLight( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {
	lastT = 0;
	g = 9.81;
}
void
BallAndLight::init( ) {
	glClearColor( .0f, .0f, .0f, 1.f );
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
	glDepthFunc( GL_LESS );

	m = v = p = glm::mat4( 1. );
	nrm = glm::mat3( 1. );

	glr.vertices( "VERTICES-BALLANDLIGHT" ).
		setUsage( GL_STATIC_DRAW ).
		addAttrib( "vertex", 3, 0 ).addAttrib( "color", 3, 3 );

	GLRenderer::VertexArray
	& va = glr.vertices( "VERTICES-BALLANDLIGHT" );


	int
	longS = 32,
	latS = 16,
	vxCount = 1,
	latd = 1;
	float
	rad = 1,
	factLong = 2.f*3.14159265f/longS,
	factLat = 3.14159265f/latS,
	x,y,z;

	glm::vec3
	normal = glm::vec3( rad*1.f, 0.f, 0.f ),
	color = glm::vec3( .8, .5, .2);
	va << normal << color;

	float
	cl = cosf(latd*factLat),
	sl = sinf(latd*factLat);

	for(int longid = 0; longid <= longS; ++longid){
		x = rad * cl;
		y = rad* sl*sinf(factLong*longid);
		z = rad* sl*cosf(factLong*longid);
		va << x << y << z << color;
		++vxCount;
	}
	va << GLRenderer::VertexArray::Object( 0, vxCount, GL_TRIANGLE_FAN );
	int off = vxCount;

	for (latd; latd < latS; latd++){

		float
		sa = sinf((latd-1)*factLat),
		ca = cosf((latd-1)*factLat),
		sb = sinf((latd-0)*factLat),
		cb = cosf((latd-0)*factLat);

		for(int longid = 0; longid <=longS; ++longid){
			float
			xA = rad*ca,
			yA = rad*sa*sinf(factLong*longid),
			zA = rad*sa*cosf(factLong*longid),
			xB = rad*cb,
			yB = rad*sb*sinf(factLong*longid),
			zB = rad*sb*cosf(factLong*longid);

			va << xA << yA << zA << color << xB << yB << zB << color;
			vxCount +=2;
		}
	 }
	 va << GLRenderer::VertexArray::Object( off, vxCount, GL_TRIANGLE_STRIP );

	 off = vxCount;

	 va << -normal.x << normal.y << normal.z <<color;

	 ++vxCount;
	 ++latd;
	 cl = cosf(latd*factLat);
	 sl = sinf(latd*factLat);

		for(int longid = 0; longid <= longS; ++longid){
			x = rad * cl;
			y = rad* sl*sinf(factLong*longid);
			z = rad* sl*cosf(factLong*longid);
			va << x << y << z << color;
		   ++vxCount;

		}

	va << GLRenderer::VertexArray::Object( off, vxCount, GL_TRIANGLE_FAN );

	glr.shader(
		"SHADER-BALLANDLIGHT",
		//Vertex Shader
		"#version 330 core\n"
		"layout( location = 0 ) in vec3 vertex;\n"
		"layout( location = 1 ) in vec3 color;\n"
		"uniform mat4 mv; \n"
		"uniform mat4 p;\n"
		"out vec3 vColor;\n"
		"out vec4 vVectorInWorldSpace;\n"
		" \n"
		"void main( void ) {\n"
		"	vColor = color;\n"
		"	vVectorInWorldSpace  = vec4( vertex, 1. );\n"
		"	gl_Position          = p * mv * vVectorInWorldSpace;\n"
		"}\n",
		//Fragment Shader
		"#version 330 core\n"
		"in vec3 vColor;\n"
		"in vec4 vVectorInWorldSpace;\n"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
		"	fColor = vec4( vColor.rgb * ( .2 + .8 * max( 0., dot( vVectorInWorldSpace.xyz, vec3( 0,.71,.71 ) ) ) ), 1 );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "mv", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv ).
			addUniform( "p",  GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p );

	glr.program( "PROGRAM-BALLANDLIGHT" ).
		setVertexArray( "VERTICES-BALLANDLIGHT" ).
		setShader( "SHADER-BALLANDLIGHT" ).
		build( );
	}
void
BallAndLight::paint( ) {
	float
	angle = 2.15f * vcd->time;

	v = glm::mat4( 1. );
	v = glm::translate( glm::mat4( 1. ), glm::vec3( 0.f, 0.f, -5.f ) );

	v = glm::rotate( v, angle, glm::vec3( 0.f, 1.f, 0.f ) );
	mv = v * m;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glr.run( { "PROGRAM-BALLANDLIGHT" } );
}
void
BallAndLight::resize( int p_width, int p_height ) {
	float
	ratio = ( 1.f * p_width / p_height );
	p = glm::perspective( 45.0f, ratio, 1.0f, 10.f );
}
