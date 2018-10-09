#include "fbtest.hpp"
#include "../../code/glmprinter.hpp"

FBTest::FBTest( CStr const & p_name ) :
GLProject ( p_name  ) {

}

void
FBTest::init( ) {

	// textures
	{
	}

	// vertex arrays
	{
		// V-FB-TEST-QUAD-3D
		{
			glr.vertices( "V-FB-TEST-QUAD-3D" ).
			setUsage( GL_STATIC_DRAW ).
			attrib( "vertex", 0, 3 ) <<
			-1.f << -1.f << +0.f <<
			+1.f << -1.f << +0.f <<
			+1.f << +1.f << +0.f <<
			-1.f << +1.f << +0.f <<
			GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
	}

	// index arrays
	{
	}

	// shaders
	{
		//S-FB-TEST-QUAD-3D
		{
			glr.shader(
			"S-FB-TEST-QUAD-3D",

			//Vertex Shader
			"#version 330 core\n"
			"layout( location = 0 ) in vec3 vertex;\n"
			"uniform mat4 model, view, projection;\n"
			"void main( void ) {\n"
			"	gl_Position = projection * view * model * vec4( vertex, 1 );\n"
			"}\n",

			//Fragment Shader
			"#version 330 core\n"
			"uniform vec4 color;\n"
			"out vec4 fColor;\n"
			"void main( void ) {\n"
			"	fColor = color;\n"
			"}\n",
			GLR::ShaderCode::FROM_CODE ).
			addUniform( "model",      GLR::Shader::MAT4, GLR::Shader::SCALAR, & m ).
			addUniform( "view",       GLR::Shader::MAT4, GLR::Shader::SCALAR, & v ).
			addUniform( "projection", GLR::Shader::MAT4, GLR::Shader::SCALAR, & p ).
			addUniform( "color",      GLR::Shader::VEC4, GLR::Shader::SCALAR, & color );
		}
	}

	// container
	{
		// C-FB-TEST-QUAD-3D
		{
			glr.container( "C-FB-TEST-QUAD-3D" ).
			setVertexArray( "V-FB-TEST-QUAD-3D" ).
			setShader( "S-FB-TEST-QUAD-3D" ).
			build( );
		}

	}

	p = v = m = glm::mat4( 1. );

	v = glm::translate( v, glm::vec3( 0.f, 0.f, -2.f ) );

	ccv.fixVCD ( vcd );

	glClearColor( 0.2f, 0.3f, 0.4f, 1.0f );

	glFrontFace( GL_CCW );
}

void
FBTest::paint( ) {

	glr.screenon ( );

	ccv.fixView ( v );
	ccv.fixModel( m );

	ccv.reactOnMouse( );

	m = ccv.model( );

	v  = ccv.view( );

	M4
	t = m;

	glClearColor( .0f, .0f, 1.0f, 1.f );
	glEnable ( GL_DEPTH_TEST );
	glDisable ( GL_CULL_FACE );
	glCullFace( GL_BACK );

//	glDisable( GL_CULL_FACE );
//	glEnable( GL_DEPTH_TEST );

	color = V4 ( 1,1,1,.5 );

	glr.container( "C-MS-TEST-QUAD-2D" );

	m = t;
}

void
FBTest::resize( int p_width, int p_height ) {

	float
	w = p_width,
	h = p_height,
	ratio = w / h;

	p = glm::perspective(  45.0f, ratio, 1.0f, 100.f );

	std::cout << std::endl;

	glViewport ( 0, 0, p_width, p_height );
}
