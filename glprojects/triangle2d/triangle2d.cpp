#include "triangle2d.hpp"
#include "../../code/glmprinter.hpp"

Triangle2D::Triangle2D( CStr const & p_name ) :
GLProject ( p_name ) {

}

void
Triangle2D::init( ) {

	glClearColor( .0f, .0f, .0f, 1.f );

	// frame buffers
	{
	}

	// textures
	{
	}

	// vertex arrays
	{
		// V-TRIANGLE-2D
		{
			glr.vertices( "V-TRIANGLE-2D" ).
				setUsage( GL_STATIC_DRAW ).
				attrib( "vertex", 0, 2 ). attrib( "color", 2, 3 ) <<
				-1.f << -1.f <<           1.f << 0.f << 0.f <<
				+1.f << -1.f <<           0.f << 1.f << 0.f <<
				+0.f << +1.f <<           0.f << 0.f << 1.f <<
				GLR::VertexArray::Object( 0, 3, GL_TRIANGLES );
		}
	}

	// index arrays
	{
	}

	// shaders
	{
		//S-TRIANGLE-2D"
		{
			glr.shader(
				"S-TRIANGLE-2D",

				//Vertex Shader
				"#version 330 core\n"
				"layout( location = 0 ) in vec2 vertex;\n"
				"layout( location = 1 ) in vec3 color;\n"
				"out VS2FS {\n"
				"	vec3 color;\n"
				"} vs2fs;\n"
				"void main( void ) {\n"
					"vs2fs.color = color;\n"
					"gl_Position = vec4( vertex, 0, 1 );\n"
				"}\n",

				//Fragment Shader
				"#version 330 core\n"
				"in VS2FS {\n"
				"	vec3 color;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( void ) {\n"
					"fColor = vec4( vs2fs.color, 1 );\n"
				"}\n",

				GLR::ShaderCode::FROM_CODE );
		}
	}

	// container
	{
		// C-TRIANGLE-2D
		{
			glr.container( "C-TRIANGLE-2D" ).
				setVertexArray( "V-TRIANGLE-2D" ).
				setShader( "S-TRIANGLE-2D" ).
				build( );
		}
	}

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
}

void
Triangle2D::paint( ) {

	glClear( GL_COLOR_BUFFER_BIT );

	glr.run( { "C-TRIANGLE-2D" } );
}

void
Triangle2D::resize( int p_width, int p_height ) {

	glr.screenon ( );
}
