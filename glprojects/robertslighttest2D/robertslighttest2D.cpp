#include "robertslighttest2D.hpp"
#include <string>
#include <dirent.h>

RobertsLightTest2D::RobertsLightTest2D( CStr const & p_name, ViewControlData * p_vcd ) :
GLProject( p_name, p_vcd ) {

}

void
RobertsLightTest2D::init( ) {

	glr.shader( "Fun3", "../EZGL/glprojects/robertslighttest2D/fun1.vsh", "../EZGL/glprojects/robertslighttest2D/fun1.fsh", GLRenderer::ShaderCode::FROM_FILE ).
		addUniform( "width", GLRenderer::Shader::INT,  GLRenderer::Shader::SCALAR, & vcd->width ).
		addUniform( "height", GLRenderer::Shader::INT, GLRenderer::Shader::SCALAR, & vcd->height ).
		addUniform( "time", GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, & vcd->time );

	glr.vertices( "VA" )
	//	    x      y
		<< -1. << -1.
		<< +1. << -1.
		<< +1. << +1.
		<< -1. << +1.

		<< GLRenderer::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );

	glr.vertices( "VA" ).addAttrib( "verts", 2, 0 );

	glr.program( "FUN" ).
		setShader( "Fun3" ).
		setVertexArray( "VA" ).
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

