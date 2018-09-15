#include "quad3d.hpp"
#include "../../code/glmprinter.hpp"

Quad3D::Quad3D( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
Quad3D::init( ) {

	glClearColor( .0f, .0f, .0f, 1.f );

	// frame buffers
	{
	}

	// textures
	{
	}

	// vertex arrays
	{
		// V-QUAD-3D
		{
			glr.vertices( "V-QUAD-3D" ).
				setUsage( GL_STATIC_DRAW ).
				attrib( "vertex", 0, 3 ).	attrib( "color", 3, 3 ) <<
				-1.f << -1.f << +0. <<			1.f << 0.f << 0.f <<
				+1.f << -1.f << +0. <<			0.f << 1.f << 0.f <<
				+1.f << +1.f << +0. <<			0.f << 0.f << 1.f <<
				-1.f << +1.f << +0. <<			1.f << 1.f << 0.f <<
				GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
	}

	// index arrays
	{
	}

	// shaders
	{
		//S-QUAD-3D"
		{
			glr.shader(
				"S-QUAD-3D",

				//Vertex Shader
				"#version 330 core\n"
				"layout( location = 0 ) in vec3 vertex;\n"
				"layout( location = 1 ) in vec3 color;\n"
				"uniform mat4 mvp;\n"
				"out VS2FS {\n"
				"	vec3 color;\n"
				"} vs2fs;\n"
				"void main( void ) {\n"
					"vs2fs.color = color;\n"
					"gl_Position = mvp * vec4( vertex, 1. );"
				"}\n",

				//Fragment Shader
				"#version 330 core\n"
				"in VS2FS {\n"
				"	vec3 color;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( void ) {\n"
					"fColor = vec4( vs2fs.color, 1. );\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE ).
				addUniform( "mvp", GLR::Shader::MAT4, GLR::Shader::SCALAR, & mvp );
		}
	}

	// container
	{
		// C-QUAD-3D
		{
			glr.container( "C-QUAD-3D" ).
				setVertexArray( "V-QUAD-3D" ).
				setShader( "S-QUAD-3D" ).
				build( );
		}
	}

	p = v = m = glm::mat4( 1. );

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );

	v = glm::translate( v, glm::vec3( 0.f, 0.f, -4.f ) );
}

void
Quad3D::paint( ) {

	// ----------------------------------------------------------
	// use a camera center view
	GLR::CameraCenterView
	ccv( m, v, vcd );

	// set parameters:
	// x,y : delta angle for rotation around y respective x axis
	// z   : delta s for moving in z-direction
	// with respect to either vcd->time or vcd->dMouse
	ccv.setParam( glm::vec3( .01f, .01f, .05f ) );

	// now react on mouse input
	ccv.reactOnMouse( );

	// add additional rotation around y
	ccv.rotate_around_y( .01f );

	// get new model matrix
	m = ccv.model( );

	// get new view matrix
	v  = ccv.view( );
	// ----------------------------------------------------------

	mvp = p * v * m;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glr.run( { "C-QUAD-3D" } );
}

void
Quad3D::resize( int p_width, int p_height ) {

	float
	w = p_width,
	h = p_height,
	ratio = w / h;

	p = glm::perspective(  45.0f, ratio, 1.0f, 100.f );

	std::cout << std::endl;
}
