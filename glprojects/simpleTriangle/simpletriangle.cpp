#include "simpletriangle.hpp"

typedef glm::vec3 V3;
typedef glm::mat4 M4;

SimpleTriangle::SimpleTriangle( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
SimpleTriangle::init( ) {

	glClearColor( .0f, .0f, .0f, 1.f );

	m = glm::mat4( 1. );

	glr.vertices( "ST-VERTICES" ).
		setUsage( GL_STATIC_DRAW ).
		addAttrib( "vertex", 2, 0 ) <<
		-1.f << -1.f <<
		+1.f << -1.f <<
		+0.f << +1.f <<
		GLRenderer::VertexArray::Object( 0, 3, GL_TRIANGLES );

	glr.shader(
		"ST-SHADER",
		//Vertex Shader
		"#version 330 core\n"
		"layout( location = 0 ) in vec2 vertex;\n"
		"void main( void ) {\n"
			"gl_Position = vec4( vertex, 0., 1. );"
		"}\n",

		//Fragment Shader
		"#version 330 core\n"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
			"fColor = vec4( .8, .7, .3, 1. );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE );

	glr.program( "ST-PROGRAM" ).
		setVertexArray( "ST-VERTICES" ).
		setShader( "ST-SHADER" ).
		build( );
	}

void
SimpleTriangle::paint( ) {

	glClear( GL_COLOR_BUFFER_BIT );

	glr.run( { "ST-PROGRAM" } );
}

void
SimpleTriangle::resize( int p_width, int p_height ) {

}
