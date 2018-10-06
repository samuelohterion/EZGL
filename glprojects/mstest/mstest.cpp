#include "mstest.hpp"
#include "../../code/glmprinter.hpp"

MSTest::MSTest( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
MSTest::init( ) {

	// frame buffers
	{
		// "F-MS-TEST"
		{
			glr.frameBuffer( "F-MS-TEST" );
		}
	}

	// textures
	{
		// T-MS-TEST-COL
		{
			glr.texture(
			"T-MS-TEST-COL",
			new GLR::Texture(
			"txCOL", GL_TEXTURE_2D, 0,
			GL_RGBA32F, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT, 32, 32 ) );
		}
		// T-MS-TEST-Z
		{
			glr.texture(
			"T-MS-TEST-Z",
			new GLR::Texture(
			"txZ", GL_TEXTURE_2D, 0,
			GL_DEPTH_COMPONENT32, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 32, 32 ) );
		}
	}

	// vertex arrays
	{
		// V-MS-TEST-QUAD-2D
		{
			glr.vertices( "V-MS-TEST-QUAD-2D" ).
			setUsage( GL_STATIC_DRAW ).
			attrib( "vertex", 0, 2 ) <<
			-1.f << -1.f <<
			+1.f << -1.f <<
			+1.f << +1.f <<
			-1.f << +1.f <<
			GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
		// V-MS-TEST-QUAD-3D
		{
			glr.vertices( "V-MS-TEST-QUAD-3D" ).
			setUsage( GL_STATIC_DRAW ).
			attrib( "vertex", 0, 3 ) <<
			+0.f << -1.f << +0.f <<
			+1.f << +0.f << +0.f <<
			+0.f << +0.f << +1.f <<
			-1.f << +0.f << +0.f <<
			+0.f << +0.f << -1.f <<
			+1.f << +0.f << +0.f <<
			GLR::VertexArray::Object( 0, 6, GL_TRIANGLE_FAN ) <<
			+0.f << +1.f << +0.f <<
			-1.f << +0.f << +0.f <<
			+0.f << +0.f << +1.f <<
			+1.f << +0.f << +0.f <<
			+0.f << +0.f << -1.f <<
			-1.f << +0.f << +0.f <<
			GLR::VertexArray::Object( 6, 6, GL_TRIANGLE_FAN );
		}
	}

	// index arrays
	{
	}

	// shaders
	{
		//S-MS-TEST-QUAD-2D
		{
			glr.shader(
			"S-MS-TEST-QUAD-2D",

			//Vertex Shader
			"#version 330 core\n"
			"layout( location = 0 ) in vec2 vertex;\n"
			"out VS2FS {\n"
			"	vec2 coord;\n"
			"} vs2fs;\n"
			"void main( void ) {\n"
			"vs2fs.coord = .5 + .5 * vertex;\n"
			"gl_Position = vec4( vertex, 0, 1. );"
			"}\n",

			//Fragment Shader
			"#version 330 core\n"
			"uniform sampler2D txZ;"
			"in VS2FS {\n"
			"	vec2 coord;\n"
			"} vs2fs;\n"
			"out vec4 fColor;\n"
			"void main( void ) {\n"
			"	float z = texture( txZ, vs2fs.coord ).r;\n"
			"	fColor = vec4( z, z, z, 1 );\n"
			"}\n",
			GLR::ShaderCode::FROM_CODE );
		}
		//S-MS-TEST-QUAD-3D
		{
			glr.shader(
			"S-MS-TEST-QUAD-3D",

			//Vertex Shader
			"#version 330 core\n"
			"layout( location = 0 ) in vec3 vertex;\n"
			"layout( location = 1 ) in vec3 color;\n"
			"uniform mat4 mvp;\n"
			"void main( void ) {\n"
			"gl_Position = mvp * vec4( vertex, 1. );"
			"}\n",

			//Fragment Shader
			"#version 330 core\n"
			"void main( void ) {\n"
			"}\n",
			GLR::ShaderCode::FROM_CODE ).
			addUniform( "mvp", GLR::Shader::MAT4, GLR::Shader::SCALAR, & mvp );
		}
	}

	// container
	{
		// C-MS-TEST-QUAD-3D
		{
			glr.container( "C-MS-TEST-QUAD-3D" ).
			setFrameBuffer( "F-MS-TEST" ).
			addOutTexture( "T-MS-TEST-Z" ).
			setVertexArray( "V-MS-TEST-QUAD-3D" ).
			setShader( "S-MS-TEST-QUAD-3D" ).
			addClearBits( GL_DEPTH_BUFFER_BIT ).
			build( );
		}
		// C-MS-TEST-QUAD-2D
		{
			glr.container( "C-MS-TEST-QUAD-2D" ).
			addInTexture( "T-MS-TEST-Z" ).
			setVertexArray( "V-MS-TEST-QUAD-2D" ).
			setShader( "S-MS-TEST-QUAD-2D" ).
			build( );
		}
	}

	p = v = m = glm::mat4( 1. );

	v = glm::translate( v, glm::vec3( 0.f, 0.f, -2.f ) );
}

void
MSTest::paint( ) {

	GLR::CameraCenterView
	ccv( m, v, vcd, glm::vec3( .02f, .02f, .1f ) );

	ccv.reactOnMouse( );

	m = ccv.model( );

	v  = ccv.view( );

	mvp = p * v * m;

	glClearColor( .11f, .12f, .13f, 1.f );
	glDisable( GL_CULL_FACE );

	glEnable( GL_DEPTH_TEST );
	glr.run( { "C-MS-TEST-QUAD-3D" } );

	glDisable( GL_DEPTH_TEST );
	glr.run( { "C-MS-TEST-QUAD-2D" } );
}

void
MSTest::resize( int p_width, int p_height ) {

	float
	w = p_width,
	h = p_height,
	ratio = w / h;

	p = glm::perspective(  45.0f, ratio, 1.0f, 100.f );

	std::cout << std::endl;
}
