#include "funIn2D.hpp"
#include <string>
#include <dirent.h>

FunIn2D::FunIn2D( CStr const & p_name, ViewControlData * p_vcd ) :
GLProject( p_name, p_vcd ) {

}

void
FunIn2D::init( ) {

	glr.shader( "Fun1", "../EZGL/glprojects/funIn2D/fun1.vsh", "../EZGL/glprojects/funIn2D/fun1.fsh", GLR::ShaderCode::FROM_FILE ).
		addUniform( "width", GLR::Shader::INT,  GLR::Shader::SCALAR, & vcd->width ).
		addUniform( "height", GLR::Shader::INT, GLR::Shader::SCALAR, & vcd->height ).
		addUniform( "time", GLR::Shader::FLOAT, GLR::Shader::SCALAR, & vcd->time );

	glr.vertices( "VA" )
	//	    x      y
		<< -1. << -1.
		<< +1. << -1.
		<< +1. << +1.
		<< -1. << +1.

		<< GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );

	glr.vertices( "VA" ).attrib( "verts", 2, 0 );

	glr.container( "FUN" ).
		setShader( "Fun1" ).
		setVertexArray( "VA" ).
		build( );
}

void
FunIn2D::resize( int p_width, int p_height ) {

}

void
FunIn2D::paint( ) {

	glDisable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT );

	glr.run( { "FUN" } );
}

