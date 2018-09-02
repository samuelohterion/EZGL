#include "robertslighttest2D.hpp"
#include <string>
#include <dirent.h>

RobertsLightTest2D::RobertsLightTest2D( CStr const & p_name, ViewControlData * p_vcd ) :
GLProject( p_name, p_vcd ) {

}

void
RobertsLightTest2D::init( ) {

	glr.shader( "Fun3", "../EZGL/glprojects/robertslighttest2D/robertslighttest2D.vsh", "../EZGL/glprojects/robertslighttest2D/robertslighttest2D.fsh", GLR::ShaderCode::FROM_FILE ).
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

	glr.vertices( "VA" ).attrib( "vertex", 2, 0 );

	glr.container( "FUN" ).
		setShader( "Fun3" ).
		setVertexArray( "VA" ).
	//	setFixSize( 64, 64 ).
		build( );
}

void
RobertsLightTest2D::resize( int p_width, int p_height ) {

}

void
RobertsLightTest2D::paint( ) {

	glDisable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT );

	glr.run( { "FUN" } );
}

