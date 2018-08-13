#include "funIn2D.hpp"
#include <string>
#include <dirent.h>

FunIn2D::FunIn2D( CStr const & p_name, ViewControlData * p_vcd ) :
GLProject( p_name, p_vcd ) {

}

void
FunIn2D::init( ) {
/*
	char *path=nullptr;
	size_t size = 255;
	path=getcwd(path,size);
	std::cout<<"\n current Path"<<path;
*/
	glr.shader( "Fun1", "../EZGL/glprojects/funIn2D/fun1.vsh", "../EZGL/glprojects/funIn2D/fun1.fsh", GLRenderer::ShaderCode::FROM_FILE );

	glr.shader( "Fun1" ).addUniform( "width", GLRenderer::Shader::INT,  GLRenderer::Shader::SCALAR, & vcd->width );
	glr.shader( "Fun1" ).addUniform( "height", GLRenderer::Shader::INT,  GLRenderer::Shader::SCALAR, & vcd->height );
	glr.shader( "Fun1" ).addUniform( "time", GLRenderer::Shader::FLOAT,  GLRenderer::Shader::SCALAR, & vcd->time );

	glr.vertices( "VA" )
	//	    x      y
		<< -1. << -1.
		<< +1. << -1.
		<< -1. << +1.
		<< +1. << +1.

		<< GLRenderer::VertexArray::Object( 0, 4, GL_TRIANGLE_STRIP );

	glr.vertices( "VA" ).addAttrib( "verts", 2, 0 );

	glr.program( "FUN" ).setShader( "Fun1" );
	glr.program( "FUN" ).setVertexArray( "VA" );
	glr.program( "FUN" ).build( );
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

