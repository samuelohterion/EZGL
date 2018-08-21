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
	glEnable( GL_CULL_FACE );
	glDepthFunc( GL_LESS );

	m = v = p = glm::mat4( 1. );
	nrm = glm::mat3( 1. );
	light = glm::vec3(0);

	glr.vertices( "VERTICES-BALLANDLIGHT" ).
		setUsage( GL_STATIC_DRAW ).
		addAttrib( "vertex", 3, 0 ).addAttrib( "color", 3, 3 ).addAttrib("normal", 3, 6);

	GLRenderer::VertexArray
	& va = glr.vertices( "VERTICES-BALLANDLIGHT" );


	int
	longS = 64,
	latS = 32,
	vxCount = 1,
	latd = 1;
	float
	rad = 2,
	factLong = 2.f*3.14159265f/longS,
	factLat = 3.14159265f/latS,
	x,y,z;

	glm::vec3
	normal = glm::vec3( rad*1.f, 0.f, 0.f ),
	color = glm::vec3( .1, .7, .5);
	va << normal << color<< normal;

	float
	cl = cosf(latd*factLat),
	sl = sinf(latd*factLat);

	for(int longid = 0; longid <= longS; ++longid){
		x = rad * cl;
		y = rad* sl*sinf(factLong*longid);
		z = rad* sl*cosf(factLong*longid);
		va << x << y << z << color << x << y << z;
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
			xA = rad*cb,
			yA = rad*sb*cosf(factLong*longid),
			zA = rad*sb*sinf(factLong*longid),
			xB = rad*ca,
			yB = rad*sa*cosf(factLong*(1+longid)),
			zB = rad*sa*sinf(factLong*(1+longid));

			va << xA << yA << zA << color << xA << yA << zA<<
			xB << yB << zB << color << xB << yB << zB ;
			vxCount +=2;
		}

	 }
	 va << GLRenderer::VertexArray::Object( off, vxCount, GL_TRIANGLE_STRIP );

	 off = vxCount;

	 va << -normal.x << normal.y << normal.z << color << -normal.x << normal.y << normal.z;

	 ++vxCount;
	 ++latd;
	 cl = cosf(latd*factLat);
	 sl = sinf(latd*factLat);

		for(int longid = longS; longid >= 0; --longid){
			x = rad * cl;
			y = rad* sl*sinf(factLong*longid);
			z = rad* sl*cosf(factLong*longid);
			va << x << y << z << color << x << y << z;
		   ++vxCount;

		}

	va << GLRenderer::VertexArray::Object( off, vxCount, GL_TRIANGLE_FAN );

	glr.shader(
		"SHADER-BALLANDLIGHT",
		//Vertex Shader
		"#version 330 core\n"
		"layout( location = 0 ) in vec3 vertex;\n"
		"layout( location = 1 ) in vec3 color;\n"
		"layout( location = 2 ) in vec3 normal;\n"

		"uniform float time;\n"
		"uniform mat4 m; \n"
		"uniform mat4 v; \n"
		"uniform mat4 p;\n"
		"uniform vec3 light;\n"
		"out vec3 vColor;\n"
		"out vec4 vNormal;\n"
		"out vec4 vLight;\n"
		"out vec4 vVert;\n"
		" \n"
		"void main( void ) {\n"
		"	vColor = color;\n"
		"	mat4 nrm = transpose( inverse( m * v ) );\n"
		"	vNormal  = normalize( nrm * ( vec4( normal, 0 ) ) ) ;\n"
		"	vLight  = m  * vec4( light, 1 );\n"
		"	vVert   = m* v * vec4( vertex, 1 ); \n"
		"	gl_Position = p * m * v * vec4( vertex, 1. );\n"
		"}\n",
		//Fragment Shader
		"#version 330 core\n"
		"in vec3 vColor;\n"
		"in vec4 vVert;\n"
		"in vec4 vNormal;\n"
		"in vec4 vLight;\n"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
			"vec3  d = vLight.xyz - vVert.xyz; \n"
			"float a = max( 0., 10. * dot( d, vNormal.xyz ) / dot( d, d ) ); \n"
			"fColor = vec4( ( .8 * a + .2 ) * vColor, 1 );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "mv", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv ).
			addUniform( "p", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p ).
			addUniform( "light", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light ).
			addUniform( "time", GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, & vcd->time );

	glr.program( "PROGRAM-BALLANDLIGHT" ).
		setVertexArray( "VERTICES-BALLANDLIGHT" ).
		setShader( "SHADER-BALLANDLIGHT" ).
		build( );
	}
void
BallAndLight::paint( ) {
	float
	angle = 1.2f * vcd->time;

	light = glm::vec3( 0, 0, +2. );


	m = v = glm::mat4( 1. );
	m = glm::translate( glm::mat4( 1. ), glm::vec3( 0.f, 0.f, +2.f ) );
	v = glm::translate( glm::mat4( 1. ), glm::vec3( 0.f, 0.f, -6.f ) );
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
