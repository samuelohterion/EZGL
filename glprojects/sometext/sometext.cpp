#include "sometext.hpp"

SomeText::SomeText ( const CStr & p_name ) :
GLProject ( p_name ) {

}

void
SomeText::init ( ) {

	// vertex arrays
	{
		// VERTEX-ARRAY-QUAD-3D
		{
			glr.vertices( "V-QUAD-3D" ).
				setUsage( GL_STATIC_DRAW ).
				attrib( "vertex", 0, 3 ). attrib( "normal", 3, 3 ) <<
				-1.f << -1.f << 0.f <<       +0.f << +0.f << +1.f <<
				+1.f << -1.f << 0.f <<       +0.f << +0.f << +1.f <<
				+1.f << +1.f << 0.f <<       +0.f << +0.f << +1.f <<
				-1.f << +1.f << 0.f <<       +0.f << +0.f << +1.f <<
				GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
	}

	// index arrays
	{

		// INDEX-ARRAY-QUAD-3D
//		{
//			glr.indices( "I-QUAD-3D" ).
//				setUsage( GL_STATIC_DRAW ) <<
//				0 << 1 << 2 << 3 <<
//				GLRenderer::IndexArray::Object( 0, 4, GL_TRIANGLE_FAN );
//		}
	}

	// shaders
	{
		// S-CHARACTER
		{
			glr.shader(
				"S-CHARACTER",

				// vertex shader
				"#version 330 core\n"
				"in vec3 vertex, normal;\n"
				"out VS2FS {\n"
				"	vec2 texCoord;\n"
				"	vec3 normal;\n"
				"} vs2fs;\n"
				"uniform mat4 model;"
				"uniform mat4 view;"
				"uniform mat4 proj;"
				"uniform vec2 letter;"

				"void main( ) {\n"
				"	vs2fs.texCoord  = letter.xy + .0625 * ( .5 + .5 * vertex.xy );\n"
				"	vs2fs.normal    = normalize( vec3( vec4( normal, 0 ) * inverse( view * model ) ) );\n"
				"	gl_Position = proj * view * model * vec4( vertex, 1 );\n"
				"}\n",

				// fragment shader
				"#version 330 core\n"

				//	light in every space that makes sense to show their behavior
				"uniform sampler2D txChars;\n"
				"in VS2FS {\n"
				"	vec2 texCoord;\n"
				"	vec3 normal;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( ) {\n"
				"	fColor = texture( txChars, vs2fs.texCoord );\n"
				"	if( dot( fColor.rgb, fColor.rgb ) < .1 )\n"
				"		discard;\n "
				"	fColor.xyz *= dot( vs2fs.normal, vec3( 0,0,1 ) );\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE ).
				addUniform( "model",  GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
				addUniform( "view",   GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
				addUniform( "proj",   GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection ).
				addUniform( "letter", GLR::Shader::VEC2, GLR::Shader::SCALAR, & letter );
		}
	}
	// textures
	{
		// T-CHARACTERS
		{
			glr.texture(
			"T-CHARACTERS",
			new GLR::Texture( "txChars", "../pix/characters.png" ) );
		}
	}
	// containers
	{
		// C-PRINT-A-CHARACTER
		{
			glr.container( "C-PRINT-A-CHARACTER" ).
				setVertexArray( "V-QUAD-3D" ).
//				setIndexArray( "I-QUAD-3D" ).
				setShader( "S-CHARACTER" ).
				addInTexture( "T-CHARACTERS" ).
				build( );
		}
	}

	glClearColor( .11f, .13f, .12f, 1. );

	projection = view = model = glm::mat4( 1. );

	view = glm::lookAt( glm::vec3( 0., 0., 10. ), glm::vec3( 0., 0., 0. ), glm::vec3( 0., 1., 0. ) );
}

void
SomeText::paint ( ) {

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// use a camera center view
	GLR::CameraCenterView
	ccv( model, view, vcd );

	// set parameters:
	// x,y : delta angle for rotation around y respective x axis
	// z   : delta s for moving in z-direction
	// with respect to either vcd->time or vcd->dMouse
	ccv.setParam( glm::vec3( .01f, .01f, .5f ) );

	// now react on mouse input
	ccv.reactOnMouse( );

	// add additional rotation around z
	ccv.rotate_around_z( .01f );

	// get new model matrix
	model = ccv.model( );

	// get new view matrix
	view  = ccv.view( );

	glm::mat4
	tmp = model;

	model = glm::translate( model, glm::vec3( -7., 0., 0. ) );
	model = glm::rotate( model, 7.1f * vcd->time, glm::vec3( 1., 0., 0. ) );

	for( GLsizei i = -7; i <= 7; ++ i ) {

		letter = glm::vec2( ( 8.f + i ) / 16.f, 11.f / 16.f );
		glr.run( { "C-PRINT-A-CHARACTER" } );

		model = glm::translate( model, glm::vec3( 1., 0., 0. ) );
		model = glm::rotate( model, -.15f / ( 8.f + i ) * vcd->time, glm::vec3( 1., 0., 0. ) );
	}

	model = tmp;
}

void
SomeText::resize ( int p_width, int p_height ) {

	// get aspect ratio
	float
	ratio = ( 1.f * p_width / p_height );

	// create a projection matrix
	projection = glm::perspective( 45.0f, ratio, 1.0f, 100.f );

	glr.screenon ( );
}
