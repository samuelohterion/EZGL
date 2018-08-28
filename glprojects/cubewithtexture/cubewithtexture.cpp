#include "cubewithtexture.hpp"
#include "../../code/glmprinter.hpp"

CubeWithTexture::CubeWithTexture( const CStr &p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
CubeWithTexture::init( ) {

	glClearColor( .11f, .13f, .12f, 1. );

	projection = view = model = glm::mat4( 1. );

	view = glm::lookAt( glm::vec3( 0., 0., 4. ), glm::vec3( 0., 0., 0. ), glm::vec3( 0., 1., 0. ) );

	// frame buffer
	{
	}

	// vertex arrays
	{
		// VA-CUBE-WITH-TEXTURE-BACKGROUND
		{

			glr.vertices( "VA-CUBE-WITH-TEXTURE-BACKGROUND" ).
				setUsage( GL_STATIC_DRAW ).
				addAttrib( "vertex", 2, 0 ).
				addAttrib( "coord",  2, 2 ) <<

				-1.f << -1.f <<  0.f << 0.f <<
				+1.f << -1.f <<  1.f << 0.f <<
				+1.f << +1.f <<  1.f << 1.f <<
				-1.f << +1.f <<  0.f << 1.f <<

				GLRenderer::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
		// VA-CUBE-WITH-TEXTURE-CUBE
		{
			/*
			 *
			 *
			 *
			 *    2----3
			 *   /|   /|
			 *  6-+--7 |
			 *  | 0--+-1
			 *  |/   |/
			 *  4----5
			 *
			 *
			 *  +-----0-----1-----------+- 4/4
			 *  |     | o o |           |
			 *  |     |  o  |           |
			 *  |     | o o |           |
			 *  0-----2-----3-----1     +- 3/4
			 *  | o o | o o | o   |     |
			 *  |     | o o |  o  |     |
			 *  | o o | o o |   o |     |
			 *  4-----6-----7-----5     +- 2/4
			 *  |     | o   |           |
			 *  |     |     |           |
			 *  |     |   o |           |
			 *  |     4-----5           +- 1/4
			 *  |     |     |           |
			 *  |     |  o  |           |
			 *  |     |     |           |
			 *  +-----0-----1-----+-----+- 0/4
			 *  |     |     |     |     |
			 * 0/4   1/4   2/4   3/4   4/4
			 *
			 *
			*/

			glm::vec3
			p0 = glm::vec3( -1, -1, -1 ),
			p1 = glm::vec3( +1, -1, -1 ),
			p2 = glm::vec3( -1, +1, -1 ),
			p3 = glm::vec3( +1, +1, -1 ),
			p4 = glm::vec3( -1, -1, +1 ),
			p5 = glm::vec3( +1, -1, +1 ),
			p6 = glm::vec3( -1, +1, +1 ),
			p7 = glm::vec3( +1, +1, +1 ),
			nx = glm::vec3( +1, +0, +0 ),
			ny = glm::vec3( +0, +1, +0 ),
			nz = glm::vec3( +0, +0, +1 );

			GLRenderer::VertexArray
			& va =
				glr.vertices( "VA-CUBE-WITH-TEXTURE-CUBE" ).
					setUsage( GL_STATIC_DRAW ).
					addAttrib( "vertex", 3, 0 ).
					addAttrib( "normal", 3, 3 ).
					addAttrib( "color",  3, 6 ).
					addAttrib( "coord",  2, 9 );

			va <<
				// FACE 1
				p0 << -ny << V3( 1. ) - ny << V2( .25, .00 ) <<
				p1 << -ny << V3( 1. ) - ny << V2( .50, .00 ) <<
				p5 << -ny << V3( 1. ) - ny << V2( .50, .25 ) <<

				p5 << -ny << V3( 1. ) - ny << V2( .50, .25 ) <<
				p4 << -ny << V3( 1. ) - ny << V2( .25, .25 ) <<
				p0 << -ny << V3( 1. ) - ny << V2( .25, .00 ) <<

				// FACE 2
				p4 << +nz << +nz << V2( .25, .25 ) <<
				p5 << +nz << +nz << V2( .50, .25 ) <<
				p7 << +nz << +nz << V2( .50, .50 ) <<

				p7 << +nz << +nz << V2( .50, .50 ) <<
				p6 << +nz << +nz << V2( .25, .50 ) <<
				p4 << +nz << +nz << V2( .25, .25 ) <<

				// FACE 3
				p7 << +nx << +nx << V2( .50, .50 ) <<
				p5 << +nx << +nx << V2( .75, .50 ) <<
				p1 << +nx << +nx << V2( .75, .75 ) <<

				p1 << +nx << +nx << V2( .75, .75 ) <<
				p3 << +nx << +nx << V2( .50, .75 ) <<
				p7 << +nx << +nx << V2( .50, .50 ) <<

				// FACE 4
				p4 << -nx << V3( 1 ) - nx << V2( .00, .50 ) <<
				p6 << -nx << V3( 1 ) - nx << V2( .25, .50 ) <<
				p2 << -nx << V3( 1 ) - nx << V2( .25, .75 ) <<

				p2 << -nx << V3( 1 ) - nx << V2( .25, .75 ) <<
				p0 << -nx << V3( 1 ) - nx << V2( .00, .75 ) <<
				p4 << -nx << V3( 1 ) - nx << V2( .00, .50 ) <<

				// FACE 5
				p2 << -nz << V3( 1 ) - nz << V2( .25, .75 ) <<
				p3 << -nz << V3( 1 ) - nz << V2( .50, .75 ) <<
				p1 << -nz << V3( 1 ) - nz << V2( .50, 1.0 ) <<

				p1 << -nz << V3( 1 ) - nz << V2( .50, 1.0 ) <<
				p0 << -nz << V3( 1 ) - nz << V2( .25, 1.0 ) <<
				p2 << -nz << V3( 1 ) - nz << V2( .25, .75 ) <<

				// FACE 6
				p6 << +ny << ny << V2( .25, .50 ) <<
				p7 << +ny << ny << V2( .50, .50 ) <<
				p3 << +ny << ny << V2( .50, .75 ) <<

				p3 << +ny << ny << V2( .50, .75 ) <<
				p2 << +ny << ny << V2( .25, .75 ) <<
				p6 << +ny << ny << V2( .25, .50 ) <<

				GLRenderer::VertexArray::Object( 0, 6 * 6, GL_TRIANGLES );
		}
	}

	// index arrays
	{
	}

	// shaders
	{
		// SH-CUBE-WITH-TEXTURE-BACKGROUND
		{
			glr.shader(
				"SH-CUBE-WITH-TEXTURE-BACKGROUND",

				// vertex shader
				"#version 330 core\n"
				"in vec2 vertex;\n"
				"in vec2 coord;\n"
				"out VS2FS {\n"
				"	vec2 coord;\n"
				"} vs2fs;\n"

				"void main( ) {\n"
				"	vs2fs.coord = coord;\n"
				"	gl_Position = vec4( vertex, 0, 1 );\n"
				"}\n",

				// fragment shader
				"#version 330 core\n"

				//	light in every space that makes sense to show their behavior
				"uniform sampler2D txBackground;\n"
				"in VS2FS {\n"
				"	vec2 coord;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( ) {\n"
				"	fColor = texture( txBackground, vs2fs.coord );\n"
				"}\n",
				GLRenderer::ShaderCode::FROM_CODE ).
				addUniform( "model",  GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model ).
				addUniform( "view",   GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view ).
				addUniform( "proj",   GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection );
		}
		// SH-CUBE-WITH-TEXTURE-CUBE
		{
			glr.shader(
				"SH-CUBE-WITH-TEXTURE-CUBE",

				// vertex shader
				"#version 330 core\n"
				"in vec3 vertex;\n"
				"in vec3 normal;\n"
				"in vec3 color;\n"
				"in vec2 coord;\n"
				"out VS2FS {\n"
				"	vec3 normalMV;\n"
				"	vec3 color;\n"
				"	vec2 texCoord;\n"
				"} vs2fs;\n"
				"uniform mat4 model;"
				"uniform mat4 view;"
				"uniform mat4 proj;"

				"void main( ) {\n"
				"	vs2fs.normalMV = normalize( vec3( vec4( normal, 0 ) * inverse( view * model ) ) );\n"
				"	vs2fs.color    = color;\n"
				"	vs2fs.texCoord = coord;\n"
				"	gl_Position = proj * view * model * vec4( vertex, 1 );\n"
				"}\n",

				// fragment shader
				"#version 330 core\n"

				//	light in every space that makes sense to show their behavior
				"uniform sampler2D txCube;\n"
				"in VS2FS {\n"
				"	vec3 normalMV;\n"
				"	vec3 color;\n"
				"	vec2 texCoord;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( ) {\n"
				"	fColor = texture( txCube, vs2fs.texCoord );\n"
				"	fColor.xyz *= .5 + .5 * vs2fs.color;\n"
				"	fColor.xyz *= dot( vs2fs.normalMV, vec3( 0,0,1 ) );\n"
				"}\n",
				GLRenderer::ShaderCode::FROM_CODE ).
				addUniform( "model",  GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model ).
				addUniform( "view",   GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view ).
				addUniform( "proj",   GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection );
		}
	}

	// textures
	{
		// TX-CUBE-WITH-TEXTURE-BACKGROUND
		{
			glr.texture(
			"TX-CUBE-WITH-TEXTURE-BACKGROUND",
			new GLRenderer::Texture( "txBackground", "../EZGL/glprojects/cubewithtexture/pix/cubemapsmall.jpg" ) );
		}
		// TX-CUBE-WITH-TEXTURE-CUBE
		{
			glr.texture(
			"TX-CUBE-WITH-TEXTURE-CUBE",
			new GLRenderer::Texture( "txCube", "../EZGL/glprojects/cubewithtexture/pix/cubemapsmall.png" ) );
		}
	}

	// programs
	{
		// PR-CUBE-WITH-TEXTURE-BACKGROUND
		{
			glr.program( "PR-CUBE-WITH-TEXTURE-BACKGROUND" ).
				setVertexArray( "VA-CUBE-WITH-TEXTURE-BACKGROUND" ).
				setShader( "SH-CUBE-WITH-TEXTURE-BACKGROUND" ).
				addInTexture( "TX-CUBE-WITH-TEXTURE-BACKGROUND" ).
				build( );
		}
		// PR-CUBE-WITH-TEXTURE-CUBE
		{
			glr.program( "PR-CUBE-WITH-TEXTURE-CUBE" ).
				setVertexArray( "VA-CUBE-WITH-TEXTURE-CUBE" ).
				setShader( "SH-CUBE-WITH-TEXTURE-CUBE" ).
				addInTexture( "TX-CUBE-WITH-TEXTURE-CUBE" ).
				build( );
		}
	}
}

void
CubeWithTexture::paint( ) {

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glr.run( { "PR-CUBE-WITH-TEXTURE-BACKGROUND" } );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	model = glm::mat4( 1. );
	model = glm::translate( model, glm::vec3( 4.f * vcd->mousex / vcd->width - 2.f, 3.f * vcd->mousey / vcd->height - 1.5f, -2.f ) );
	model = glm::rotate( model, 1.f * vcd->time, glm::vec3( sinf( .1f * vcd->time ), cosf( .11f * vcd->time ), sinf( .12f * vcd->time ) ) );

	glr.run( { "PR-CUBE-WITH-TEXTURE-CUBE" } );
}

void
CubeWithTexture::resize( int p_width, int p_height ) {

	// get aspect ratio
	float
	ratio = ( 1.f * p_width / p_height );

	// create a projection matrix
	projection = glm::perspective( 45.0f, ratio, .1f, 100.f );
}
