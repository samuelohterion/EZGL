#include "zbuffertexture.hpp"
#include "../../code/glmprinter.hpp"

ZBufferTexture::ZBufferTexture( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
ZBufferTexture::init( ) {

	// frame buffers
	{
		// F-Z-BUFFER-TEXTURE
		{
			glr.frameBuffer( "F-Z-BUFFER-TEXTURE" );
		}
	}

	// textures
	{
		// T-Z-BUFFER-TEXTURE-Z
		{
			glr.texture(
				"T-Z-BUFFER-TEXTURE-Z",
				new GLR::Texture(
				"txZ", GL_TEXTURE_2D, 0,
				GL_DEPTH_COMPONENT32, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 32, 32 ) );
		}
	}

	// vertex arrays
	{
		// V-Z-BUFFER-TEXTURE-QUAD-2D
		{
			glr.vertices( "V-Z-BUFFER-TEXTURE-QUAD-2D" ).
				setUsage( GL_STATIC_DRAW ).
				attrib( "vertex", 0, 2 ) <<
				-1.f << -1.f <<
				+1.f << -1.f <<
				+1.f << +1.f <<
				-1.f << +1.f <<
				GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
		// V-Z-BUFFER-TEXTURE-QUAD-3D
		{
			glr.vertices( "V-Z-BUFFER-TEXTURE-QUAD-3D" ).
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
		//S-Z-BUFFER-TEXTURE-QUAD-2D
		{
			glr.shader(
				"S-Z-BUFFER-TEXTURE-QUAD-2D",

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
		//S-Z-BUFFER-TEXTURE-QUAD-3D
		{
			glr.shader(
				"S-Z-BUFFER-TEXTURE-QUAD-3D",

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
		// C-Z-BUFFER-TEXTURE-QUAD-3D
		{
			glr.container( "C-Z-BUFFER-TEXTURE-QUAD-3D" ).
				setFrameBuffer( "F-Z-BUFFER-TEXTURE" ).
				addOutTexture( "T-Z-BUFFER-TEXTURE-Z" ).
				setVertexArray( "V-Z-BUFFER-TEXTURE-QUAD-3D" ).
				setShader( "S-Z-BUFFER-TEXTURE-QUAD-3D" ).
				addClearBits( GL_DEPTH_BUFFER_BIT ).
				build( );
		}
		// C-Z-BUFFER-TEXTURE-QUAD-2D
		{
			glr.container( "C-Z-BUFFER-TEXTURE-QUAD-2D" ).
				addInTexture( "T-Z-BUFFER-TEXTURE-Z" ).
				setVertexArray( "V-Z-BUFFER-TEXTURE-QUAD-2D" ).
				setShader( "S-Z-BUFFER-TEXTURE-QUAD-2D" ).
				build( );
		}
	}

	p = v = m = glm::mat4( 1. );
}

void
ZBufferTexture::paint( ) {

	float
	angle = 2.15f * vcd->time;

	v = glm::mat4( 1. );
	v = glm::translate( glm::mat4( 1. ), glm::vec3( 0.f, 0.f, -5.f + 4.f * sinf( .32f * vcd->time ) ) );
	v = glm::rotate( v, angle, glm::vec3( 0.1f, 1.f, 0.2f ) );

	mvp = p * v * m;

	glClearColor( .11f, .12f, .13f, 1.f );
	glDisable( GL_CULL_FACE );

	glEnable( GL_DEPTH_TEST );
	glr.run( { "C-Z-BUFFER-TEXTURE-QUAD-3D" } );

	glDisable( GL_DEPTH_TEST );
	glr.run( { "C-Z-BUFFER-TEXTURE-QUAD-2D" } );
}

void
ZBufferTexture::resize( int p_width, int p_height ) {

	float
	w = p_width,
	h = p_height,
	ratio = w / h;

	p = glm::perspective(  45.0f, ratio, 1.0f, 100.f );

	std::cout << std::endl;
}
