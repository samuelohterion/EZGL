#include "cubewithtexture.hpp"
#include "../../code/glmprinter.hpp"

CubeWithTexture::CubeWithTexture( const CStr &p_name ) :
GLProject ( p_name ) {

}

void
CubeWithTexture::init( ) {

	// frame buffer
	{
	}

	// textures
	{
		// T-CUBE-WITH-TEXTURE-BACKGROUND
		{
			glr.texture(
			"T-CUBE-WITH-TEXTURE-BACKGROUND",
			new GLR::Texture( "txBackground", "../pix/cubemapsmall.jpg" ) );
		}
		// T-CUBE-WITH-TEXTURE-CUBE
		{
			glr.texture(
			"T-CUBE-WITH-TEXTURE-CUBE",
			new GLR::Texture( "txCube", "../pix/cubemapsmall.png" ) );
		}
	}

	// vertex arrays
	{
		// V-CUBE-WITH-TEXTURE-BACKGROUND
		{

			glr.vertices( "V-CUBE-WITH-TEXTURE-BACKGROUND" ).
				setUsage( GL_STATIC_DRAW ).
				attrib( "vertex", 0, 2 ) <<

				-1.f << -1.f <<
				+1.f << -1.f <<
				+1.f << +1.f <<
				-1.f << +1.f <<

				GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
		// V-CUBE-WITH-TEXTURE-CUBE
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

			GLR::VertexArray
			& va =
				glr.vertices( "V-CUBE-WITH-TEXTURE-CUBE" ).
					setUsage( GL_STATIC_DRAW ).
					attrib( "vertex", 0, 3 ).
					attrib( "normal", 3, 3 ).
					attrib( "color",  6, 3 ).
					attrib( "coord",  9, 2 );

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

				GLR::VertexArray::Object( 0, 6 * 6, GL_TRIANGLES );
		}
	}

	// index arrays
	{
	}

	// shaders
	{
		// S-CUBE-WITH-TEXTURE-BACKGROUND
		{
			glr.shader(
				"S-CUBE-WITH-TEXTURE-BACKGROUND",

				// vertex shader
				"#version 330 core\n"
				"uniform int width;\n"
				"uniform int height;\n"

				"in vec2 vertex;\n"
				"out VS2FS {\n"
				"	vec2 coord;\n"
				"} vs2fs;\n"

				"void main( ) {\n"
				"float\n"
				"	denom = width < height ? width : height;\n"
				"	vec2 aspect = vec2( width, height ) / denom;"
				"	vs2fs.coord = aspect * ( .5 + .5 * vertex );\n"
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
				"float random ( in vec2 st ) {\n"
					"return fract( sin( dot( st.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453123 );\n"
				"}\n"
				"void main( ) {\n"
				"	fColor = vec4( texture( txBackground, vs2fs.coord ).rgb, 1 );\n"
				"}\n",

				GLR::ShaderCode::FROM_CODE ).
				addUniform( "width",  GLR::Shader::INT, GLR::Shader::SCALAR, & vcd->width ).
				addUniform( "height", GLR::Shader::INT, GLR::Shader::SCALAR, & vcd->height );
		}
		// S-CUBE-WITH-TEXTURE-CUBE
		{
			glr.shader(
				"S-CUBE-WITH-TEXTURE-CUBE",

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
				"	if( dot( fColor.xyz, fColor.xyz ) < .8 )\n"
				"		fColor.xyz *= 1.1f * vs2fs.color;\n"
				"	fColor.xyz *= dot( vs2fs.normalMV, vec3( 0, 0, 1 ) );\n"
				"}\n",

				GLR::ShaderCode::FROM_CODE ).
				addUniform( "model",  GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
				addUniform( "view",   GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
				addUniform( "proj",   GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection );
		}
	}

	// containers
	{
		// C-CUBE-WITH-TEXTURE-BACKGROUND
		{
			glr.container( "C-CUBE-WITH-TEXTURE-BACKGROUND" ).
				setVertexArray( "V-CUBE-WITH-TEXTURE-BACKGROUND" ).
				setShader( "S-CUBE-WITH-TEXTURE-BACKGROUND" ).
				addInTexture( "T-CUBE-WITH-TEXTURE-BACKGROUND" ).
				build( );
		}
		// C-CUBE-WITH-TEXTURE-CUBE
		{
			glr.container( "C-CUBE-WITH-TEXTURE-CUBE" ).
				setVertexArray( "V-CUBE-WITH-TEXTURE-CUBE" ).
				setShader( "S-CUBE-WITH-TEXTURE-CUBE" ).
				addInTexture( "T-CUBE-WITH-TEXTURE-CUBE" ).
				build( );
		}
	}

	glClearColor( .11f, .13f, .12f, 1. );

	projection = view = model = glm::mat4( 1. );

	model = glm::translate( model, glm::vec3( +0.f, +0.f, -2.f ) );

	view = glm::lookAt( glm::vec3( 0., 0., 4. ), glm::vec3( 0., 0., 0. ), glm::vec3( 0., 1., 0. ) );
}

void
CubeWithTexture::paint( ) {

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glr.run( { "C-CUBE-WITH-TEXTURE-BACKGROUND" } );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	// ----------------------------------------------------------
	// use a camera center view
	GLR::CameraCenterView
	ccv( model, view, vcd );

	// set parameters:
	// x,y : delta angle for rotation around y respective x axis
	// z   : delta s for moving in z-direction
	// with respect to either vcd->time or vcd->dMouse
	ccv.setParam( glm::vec3( .01f, .01f, .05f ) );

	// now react on mouse input
	ccv.reactOnMouse( );

	// add additional rotation around z
	ccv.rotate_around_z( .01f );

	// get new model matrix
	model = ccv.model( );

	// get new view matrix
	view  = ccv.view( );
	// ----------------------------------------------------------

	glr.run( { "C-CUBE-WITH-TEXTURE-CUBE" } );
}

void
CubeWithTexture::resize( int p_width, int p_height ) {

	// get aspect ratio
	float
	ratio = ( 1.f * p_width / p_height );

	// create a projection matrix
	projection = glm::perspective( 45.0f, ratio, .1f, 100.f );

	glr.screenon ( );
}
