#include "zbuffertexture.hpp"
#include "../../code/glmprinter.hpp"

ZBufferTexture::ZBufferTexture( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
ZBufferTexture::init( ) {

	glClearColor( .0f, .0f, .0f, 1.f );

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
				GL_DEPTH_COMPONENT24, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 32, 32 ) );
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
					"vs2fs.coord = .5 + .5 * vertex.xy;\n"
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
				"	z = ( z == 0 ) ? 1 : 0;\n"
					"fColor = vec4( z, z, z, 1. );\n"
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
		//S-Z-BUFFER-TEXTURE-QUAD-3D-Z
		{
			glr.shader(
				"S-Z-BUFFER-TEXTURE-QUAD-3D-Z",

				//Vertex Shader
				"#version 330 core\n"
				"layout( location = 0 ) in vec3 vertex;\n"
				"layout( location = 1 ) in vec3 color;\n"
				"uniform mat4 mvp;\n"
				"out VS2FS {\n"
				"	vec4 vertex;\n"
				"} vs2fs;\n"
				"void main( void ) {\n"
					"vs2fs.vertex = mvp * vec4( vertex, 1 );\n"
					"gl_Position = vs2fs.vertex;"
				"}\n",

				//Fragment Shader
				"#version 330 core\n"
				"in VS2FS {\n"
				"	vec4 vertex;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( void ) {\n"
					"fColor = vs2fs.vertex.zzzz;\n"
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
				setVertexArray( "V-Z-BUFFER-TEXTURE-QUAD-3D" ).
				setShader( "S-Z-BUFFER-TEXTURE-QUAD-3D" ).
				build( );
		}
		// C-Z-BUFFER-TEXTURE-QUAD-3D-Z
		{
			glr.container( "C-Z-BUFFER-TEXTURE-QUAD-3D-Z" ).
				setFrameBuffer( "F-Z-BUFFER-TEXTURE" ).
				addOutTexture( "T-Z-BUFFER-TEXTURE-Z" ).
				setVertexArray( "V-Z-BUFFER-TEXTURE-QUAD-3D" ).
				setShader( "S-Z-BUFFER-TEXTURE-QUAD-3D-Z" ).
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
	v = glm::translate( glm::mat4( 1. ), glm::vec3( 0.f, 0.f, -4.f ) );
	v = glm::rotate( v, angle, glm::vec3( 0.f, 1.f, 0.f ) );

	mvp = p * v * m;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glr.run( { "C-Z-BUFFER-TEXTURE-QUAD-3D-Z" } );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
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
