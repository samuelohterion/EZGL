#include "ballandlight.hpp"

typedef glm::vec3 V3;
typedef glm::mat4 M4;
BallAndLight::BallAndLight( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {
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
	glm::vec3
	normal = glm::vec3( 1.f, 0.f, 0.f ),
	color = glm::vec3( .5, .2, .5);
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
	va << normal << color;
	float
	cl = cosf(latd*factLat),
	sl = sinf(latd*factLat);
	// HIER bei 0 anfangen
	for(int longid = 0; longid <= longS; ++longid){
		x = rad * cl;
		y = rad* sl*sinf(factLong*longid);
		z = rad* sl*cosf(factLong*longid);
		va << x << y << z << color;
		++vxCount;
	}
	va << GLRenderer::VertexArray::Object( 0, vxCount, GL_TRIANGLE_FAN );
	int off = vxCount;
	for (latd; latd< latS-1; latd++){

	}
	glr.shader(
		"SHADER-BALLANDLIGHT",
		//Vertex Shader
		"#version 330 core\n"
		"layout( location = 0 ) in vec3 vertex;\n"
		"layout( location = 1 ) in vec3 color;\n"
		"uniform float time;\n"
		"uniform mat4 mv; \n"
		"uniform mat4 p;\n"
		"out vec3 vColor;\n"
		"out float t;\n"
		"void main( void ) {\n"
			   "vColor = color;\n"
			   "t = time;\n"
			   "gl_Position = p*mv * vec4( vertex, 1. );"
		"}\n",
		//Fragment Shader
		"#version 330 core\n"
		"in vec3 vColor;\n"
		"in float t;"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
		"	fColor = vec4(1,1,1,1);"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "mv", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv ).
			addUniform( "p", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p ).
			addUniform( "time", GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, & vcd->time );

	glr.program( "PROGRAM-BALLANDLIGHT" ).
		setVertexArray( "VERTICES-BALLANDLIGHT" ).
		setShader( "SHADER-BALLANDLIGHT" ).
		build( );
	}
void
BallAndLight::paint( ) {
	float
	angle = 2.15f * vcd->time;
//    light = glm::vec3( 5.f * sin( vcd->time ), 0.f, 0.f );
	//light = glm::vec3( 2.f, 2.f, 2.f );
	v = glm::mat4( 1. );
	v = glm::translate( glm::mat4( 1. ), glm::vec3( 0.f, 0.f, -2.f ) );
	//v = glm::rotate( v, angle, glm::vec3( sin( .1 * angle ), sin( .12 * angle ), sin( .13 * angle ) ) );
	v = glm::rotate( v, angle, glm::vec3( 0.f, 0.f, 1.f ) );
	mv = v * m;  // Calculate final MVP matrix
//    norm = glm::transpose( glm::inverse( mvp ) );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glr.run( { "PROGRAM-BALLANDLIGHT" } );
}
void
BallAndLight::resize( int p_width, int p_height ) {
	float
	ratio = ( 1.f * p_width / p_height );
	p = glm::perspective( 45.0f, ratio, 1.0f, 10.f );
}
