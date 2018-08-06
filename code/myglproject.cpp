#include "myglproject.hpp"

MyGLProject::MyGLProject( CStr const & p_name, ViewControlData * p_vcd ) :
GLProject( p_name, p_vcd ) {

}

void
MyGLProject::init( ) {

	glr.shader( "Fun1", "../shaders/fun1.vsh", "../shaders/fun1.fsh", GLRenderer::ShaderCode::FROM_FILE );

	glr.shader( "Fun1" ).addUniform( "width", GLRenderer::Shader::INT,  GLRenderer::Shader::SCALAR, & vcd->width );
	glr.shader( "Fun1" ).addUniform( "height", GLRenderer::Shader::INT,  GLRenderer::Shader::SCALAR, & vcd->height );
	glr.shader( "Fun1" ).addUniform( "time", GLRenderer::Shader::FLOAT,  GLRenderer::Shader::SCALAR, & vcd->time );

	glr.vertices( "VA" )
	//	    x      y
		<< -1. << -1.
		<< +1. << -1.
		<< -1. << +1.
		<< +1. << +1.;

	glr.vertices( "VA" ).addAttrib( "verts", 2, 0 );

	glr.program( "FUN" ).setShader( "Fun1" );
	glr.program( "FUN" ).setVertexArray( "VA" );
	glr.program( "FUN" ).build( );
}

void
MyGLProject::resize( int p_width, int p_height ) {

}

void
MyGLProject::paint( ) {

	glDisable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT );

	glr.run( { "FUN" } );
}

