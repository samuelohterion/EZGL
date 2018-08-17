#include "simplecoloredtriangle.hpp"
#include "../coordinateSystem/coordinatesystem.hpp"

typedef glm::vec3 V3;
typedef glm::mat4 M4;

SimpleColoredTriangle::SimpleColoredTriangle( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
SimpleColoredTriangle::init( ) {

	glClearColor( .0f, .0f, .0f, 1.f );

	m = glm::mat4( 1. );

	glr.vertices( "SCT-VERTICES" ).
		setUsage( GL_STATIC_DRAW ).
		addAttrib( "vertex", 3, 0 ).	addAttrib( "color", 3, 3 ) <<
		-1.f << -1.f << +0. <<			1.f << 0.f << 0.f <<
		+1.f << -1.f << +0. <<			0.f << 1.f << 0.f <<
		+1.f << +1.f << +0. <<			0.f << 0.f << 1.f <<
		-1.f << +1.f << +0. <<			0.f << 0.f << 1.f <<
		GLRenderer::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );

	glr.shader(
		"SCT-SHADER",
		//Vertex Shader
		"#version 330 core\n"
		"layout( location = 0 ) in vec3 vertex;\n"
		"layout( location = 1 ) in vec3 color;\n"
		"uniform mat4 mvp;\n"
		"out vec3 vColor;\n"
		"void main( void ) {\n"
			"vColor = color;\n"
			"gl_Position = mvp * vec4( vertex, 1. );"
		"}\n",

		//Fragment Shader
		"#version 330 core\n"
		"in vec3 vColor;\n"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
			"fColor = vec4( vColor, 1. );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
		addUniform( "mvp", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mvp );

	glr.program( "SCT-PROGRAM" ).
		setVertexArray( "SCT-VERTICES" ).
		setShader( "SCT-SHADER" ).
		build( );
	}

void
SimpleColoredTriangle::paint( ) {

	float
	angle = 2.15f * vcd->time;
//	light = glm::vec3( 5.f * sin( vcd->time ), 0.f, 0.f );
	//light = glm::vec3( 2.f, 2.f, 2.f );

	v = glm::mat4( 1. );

	v = glm::translate( glm::mat4( 1. ), glm::vec3( 0.f, 0.f, -4.f ) );
	//v = glm::rotate( v, angle, glm::vec3( sin( .1 * angle ), sin( .12 * angle ), sin( .13 * angle ) ) );
	v = glm::rotate( v, angle, glm::vec3( 0.f, 1.f, 0.f ) );

	mvp = p * v * m;  // Calculate final MVP matrix
//	norm = glm::transpose( glm::inverse( mvp ) );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glr.run( { "SCT-PROGRAM" } );
}

void
SimpleColoredTriangle::resize( int p_width, int p_height ) {

	float
	w = p_width,
	h = p_height,
	ratio = w / h;

	p = glm::perspective(  45.0f, ratio, 1.0f, 100.f );

	print( p );
	std::cout << std::endl;
}
