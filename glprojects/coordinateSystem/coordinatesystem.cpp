#include "coordinatesystem.hpp"
#include "../../code/glmprinter.hpp"

CoordinateSystem::CoordinateSystem ( CStr const & p_name ) :
GLProject ( p_name ) {

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
			"gs2fs.color  = vec4( max( vec3( .5 ), vec3( 1. ) ), 1. );\n"
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

	view = glm::lookAt( glm::vec3( +0.f, +0.5f, +4.f ), glm::vec3( +0.f, +0.f, +0.f ), normalize( glm::vec3( +0.f, +1.f, +0.f ) ) );
}

void
CoordinateSystem::paint( ) {

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glm::mat4
	tmpV = view,
	tmpM = model;

	model = glm::mat4( 1 );

	view = glm::lookAt( glm::vec3( +0.f, +0.5f, +4.f ), glm::vec3( +0.f, +0.f, +0.f ), normalize( glm::vec3( +0.f, +1.f, +0.f ) ) );

	modelView = view * model;

	glr.run( { "C-COORDINATE-SYSTEM" } );

	model = tmpM;

	view = tmpV;

	// ----------------------------------------------------------
	// use a camera center view
	GLR::CameraCenterView
	ccv( model, view, vcd );

	// set parameters:
	// x,y : delta angle for rotation around y respective x axis
	// z   : delta s for moving in z-direction
	// with respect to either vcd->time or vcd->dMouse
	// default: dx: .01f  dy: .01f  dz: .1f
	// ccv.setParam( glm::vec3( .01f, .01f, .1f ) );

	// now react on mouse input
	ccv.reactOnMouse( );

	// add additional rotation around z
	// ccv.rotate_around_z( .01f );

	// get new model matrix
	model = ccv.model( );

	// get new view matrix
	view  = ccv.view( );
	// ----------------------------------------------------------

	modelView = view * model;

	glr.run( { "C-COORDINATE-SYSTEM" } );

	std::cout << std::endl;
}

void
CoordinateSystem::resize( int p_width, int p_height ) {

	float
	ratio = ( 1.f * p_width / p_height );

	projection = glm::perspective( 45.0f, ratio, 1.0f, 100.f );

	glr.screenon ( );
}
