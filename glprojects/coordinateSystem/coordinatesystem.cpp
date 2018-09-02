#include "coordinatesystem.hpp"
#include "../../code/glmprinter.hpp"

CoordinateSystem::CoordinateSystem( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
CoordinateSystem::init( ) {

	glClearColor( .11f, .13f, .12f, 1. );

	projection = view = model = glm::mat4( 1. );

	// frame buffer
	{
	}

	// textures
	{
	}

	// vertex arrays
	{
		// V-COORDINATE-SYSTEM
		{
			glr.vertices( "V-COORDINATE-SYSTEM" ).
				setUsage( GL_STATIC_DRAW ).
				attrib( "vertex", 0, 3 ) <<
				+0.f << +0.f << +0.f <<
				+1.f << +0.f << +0.f <<
				+0.f << +0.f << +0.f <<
				+0.f << +1.f << +0.f <<
				+0.f << +0.f << +0.f <<
				+0.f << +0.f << +1.f <<
				GLR::VertexArray::Object( 0, 6, GL_LINES );
		}
	}

	// index arrays
	{
	}

	// shaders
	{
		// S-COORDINATE-SYSTEM
		{
			glr.shader(
				"S-COORDINATE-SYSTEM",

				//Vertex Shader
				"#version 330 core\n"
				"layout( location = 0 ) in vec3 vertex;\n"
				"uniform mat4 mv;\n"
				"uniform mat4 p;\n"
				"out VS2GS { vec4 vertex, mvv; } vs2gs;\n"
				"void main( void ) {\n"
					"vec4 v = mv * vec4( vertex, 1. );"
					"vs2gs.mvv = v;\n"
					"vs2gs.vertex = p * v;\n"
					"gl_Position = p * v;\n"
				"}\n",

				//Geometry Shader
				"#version 330 core\n"
				"layout ( lines ) in;\n"
				"layout ( line_strip, max_vertices = 2 ) out;\n"
				"in VS2GS { vec4 vertex, mvv; } vs2gs[ ];\n"
				"out GS2FS { vec4 color; } gs2fs;\n"
				"void main( void ) {\n"
					"gs2fs.color  = vec4( max( vec3( 0. ), vs2gs[ 0 ].mvv.xyz - vs2gs[ 1 ].mvv.xyz ), 1. );\n"
					"gl_Position  = gl_in[ 0 ].gl_Position;\n"
					"EmitVertex( );\n"
					"gs2fs.color  = vec4( max( vec3( 0. ), vs2gs[ 1 ].mvv.xyz - vs2gs[ 0 ].mvv.xyz ), 1. );\n"
					"gl_Position = gl_in[ 1 ].gl_Position;\n"
					"EmitVertex( );\n"
					"EndPrimitive( );\n"
				"}\n",

				//Fragment Shader
				"#version 330 core\n"
				"in GS2FS { vec4 color; } gs2fs;\n"
				"out vec4 fColor;\n"
				"void main( void ) {\n"
					"fColor = gs2fs.color;\n"
				"}\n",

				GLR::ShaderCode::FROM_CODE ).
					addUniform( "mv", GLR::Shader::MAT4, GLR::Shader::SCALAR, & modelView ).
					addUniform( "p",  GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection );
		}
	}

	// container
	{
		// C-COORDINATE-SYSTEM
		{
			glr.container( "C-COORDINATE-SYSTEM" ).
					setVertexArray( "V-COORDINATE-SYSTEM" ).
					setShader( "S-COORDINATE-SYSTEM" ).
					build( );
		}
	}

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	projection = view = model = glm::mat4( 1. );
}

void
CoordinateSystem::paint( ) {

	float
	angle = .31f * vcd->time;

	view = glm::translate( glm::mat4( 1. ), glm::vec3( 0.f, 0.f, -2.f ) );
	view = glm::rotate( view, angle, glm::vec3( .5f * sinf( .2f * angle ), 1.f, 0.f ) );

	modelView = view * model;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glr.run( { "C-COORDINATE-SYSTEM" } );

	print( modelView * glm::vec4( 0., 0., 1., 1. ) );

	std::cout << std::endl;
}

void
CoordinateSystem::resize( int p_width, int p_height ) {

	float
	ratio = ( 1.f * p_width / p_height );

	projection = glm::perspective( 45.0f, ratio, 1.0f, 100.f );
}
