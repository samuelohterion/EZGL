#include "funIn2D.hpp"
#include <string>
#include <dirent.h>

FunIn2D::FunIn2D( CStr const & p_name ) :
GLProject( p_name ) {

}

void
FunIn2D::init( ) {

	// usual initialisation frame work

	// frame buffer
	{
	}

	// textures
	{
	}

	// vertex arrays
	{
		// V-FUN-IN-2D
		{
			glr.vertices( "V-FUN-IN-2D" ).
				attrib( "vertex", 0, 2 ) <<
			//	 x      y
				-1. << -1. <<
				+1. << -1. <<
				+1. << +1. <<
				-1. << +1. <<
				GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
	}

	// index arrays
	{
	}

	// shaders
	{
		// S-FUN-IN-2D
		{
			glr.shader(
				// shader name
				"S-FUN-IN-2D",

				// vertex shader filename
				"../EZGL/glprojects/funIn2D/fun1.vsh",

				// fragment shader filename
				"../EZGL/glprojects/funIn2D/fun1.fsh",

				// load from file
				GLR::ShaderCode::FROM_FILE ).
				addUniform( "width",  GLR::Shader::INT,   GLR::Shader::SCALAR, & vcd->width ).
				addUniform( "height", GLR::Shader::INT,   GLR::Shader::SCALAR, & vcd->height ).
				addUniform( "time",   GLR::Shader::FLOAT, GLR::Shader::SCALAR, & vcd->time );
		}
	}

	// containers
	{
		// C-FUN-IN-2D
		{
			glr.container( "C-FUN-IN-2D" ).
				setShader( "S-FUN-IN-2D" ).
				setVertexArray( "V-FUN-IN-2D" ).
				build( );
		}
	}
}

void
FunIn2D::resize( int p_width, int p_height ) {

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );

	glr.screenon ( );
}

void
FunIn2D::paint( ) {

	// clearing not neccessary
	// glClear( GL_COLOR_BUFFER_BIT );

	// just run fun!
	glr.run( { "C-FUN-IN-2D" } );
}

