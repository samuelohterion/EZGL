#include "yetanothersphere.hpp"

YetAnotherSphere::YetAnotherSphere( const CStr &p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
YetAnotherSphere::init( ) {

	glClearColor( .11f, .13f, .12f, 1. );

	projection = view = model = glm::mat4( 1. );

	view = glm::lookAt( glm::vec3( 0., 0., 4. ), glm::vec3( 0., 0., 0. ), glm::vec3( 0., 1., 0. ) );

	// frame buffer
	{
	}

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
				-1.f << +1.f << 0.f <<       +0.f << +0.f << +1.f;// <<
				//GLRenderer::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
		// V-YET-ANOTHER-SPHERE
		{
			GLR::VertexArray
			& va =
				glr.vertices( "V-YET-ANOTHER-SPHERE" ).
					setUsage( GL_STATIC_DRAW ).
					attrib( "vertex", 0, 3 ).
					attrib( "color", 3, 3 );

			float
			a = sqrtf( 1.f / 24.f ),
			b = sqrtf( 2.f / 24.f ),
			c = sqrtf( 1.f / 3.f ),
			d = sqrtf( 2.f / 3.f );

			glm::vec3
			p0 = glm::vec3( +0.f, +0.f, d - a ),
			p1 = glm::vec3( -b,   -.5f,    -a ),
			p2 = glm::vec3( -b,   +.5f,    -a ),
			p3 = glm::vec3( +c,   +0.f,    -a ),
			pX = p1 - p0,
			pY = p2 - p0;

			GLsizei
			divs = 1,
			len  = 1 << divs;

			GLfloat
			dX = 1.f / len,
			dY = 1.f / len;

			for( GLsizei y = 0; y <= len; ++ y ) {

				for( GLsizei x = 0; x <= len - y; ++ x ) {

					glm::vec3 p = x * dX * pX + y * dY * pY;

					va << p << glm::vec3( 1., 0., 0. );
				}
			}

			pX = p2 - p0;
			pY = p3 - p0;

			for( GLsizei y = 0; y <= len; ++ y ) {

				for( GLsizei x = 0; x <= len - y; ++ x ) {

					glm::vec3 p = x * dX * pX + y * dY * pY;

					va << p << glm::vec3( 0., 1., 0. );
				}
			}

			pX = p3 - p0;
			pY = p1 - p0;

			for( GLsizei y = 0; y <= len; ++ y ) {

				for( GLsizei x = 0; x <= len - y; ++ x ) {

					glm::vec3 p = x * dX * pX + y * dY * pY;

					va << p << glm::vec3( 0., 0., 1. );
				}
			}

//			va << glm::vec3( p0 ) << glm::vec3( 1., 0., 0. );
//			va << glm::vec3( p1 ) << glm::vec3( 0., 1., 0. );
//			va << glm::vec3( p2 ) << glm::vec3( 0., 0., 1. );
//			va << glm::vec3( p3 ) << glm::vec3( 1., 1., 1. );
		}
	}

	// index arrays
	{
		// INDEX-ARRAY-QUAD-3D
		{
			glr.indices( "I-QUAD-3D" ).
				setUsage( GL_STATIC_DRAW ) <<
				0 << 1 << 2 << 3 <<
				GLR::IndexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
		// I-YET-ANOTHER-SPHERE

		//     5
		//   3   4
		// 0   1   2
		//     5
		//   4   8
		// 2   6   7
		//     5
		//   8   a
		// 7   9   b

		//                      35
		//                   33    34
		//                30    31    32
		//             26    27    28    29
		//          21    22    23    24    25
		//       15    16    17    18    19    20
		//    08    09    10    11    12    13    14
		// 00    01    02    03    04    05    06    07

		//                      35
		//                   34    62
		//                32    60    61
		//             29    57    58    59
		//          25    53    54    55    56
		//       20    48    49    50    51    52
		//    14    42    43    44    45    46    47
		// 07    35    36    37    38    39    40    41

		{
			glr.indices( "I-YET-ANOTHER-SPHERE" ).
				setUsage( GL_STATIC_DRAW ) <<
				0 << 1 << 3 <<
				1 << 2 << 4 <<
				1 << 4 << 3 <<
				3 << 4 << 5 <<
				GLR::IndexArray::Object( 0, 12, GL_TRIANGLES );
		}
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
		// S-YET-ANOTHER-SPHERE
		{
			glr.shader(
				"S-YET-ANOTHER-SPHERE",

				// vertex shader
				"#version 330 core\n"
				"in vec3 vertex, color;\n"
				"out VS2GS {\n"
				"	vec3 vertex, color;\n"
				"} vs2gs;\n"
				"uniform mat4 model;"
				"uniform mat4 view;"
				"uniform mat4 proj;"
				"void main( ) {\n"
				"	vec4\n"
				"	v = vec4( vertex, 1 );\n"
				"	vs2gs.vertex = vec3( view * model * v );\n"
				"	vs2gs.color  = color;\n"
				"	gl_Position = proj * view * model * v;\n"
				"}\n",

				// geometry shader
				"#version 330 core\n"
				"layout ( triangles ) in;\n"
				"layout ( triangle_strip, max_vertices = 3 ) out;\n"

				//	light in every space that makes sense to show their behavior
				"in VS2GS {\n"
				"	vec3 vertex, color;\n"
				"} vs2gs[ ];\n"
				"out GS2FS {\n"
				"	vec4 color, normal;\n"
				"} gs2fs;\n"
				"void main( ) {\n"
				"	vec3 n = normalize( cross( vs2gs[ 1 ].vertex - vs2gs[ 0 ].vertex, vs2gs[ 2 ].vertex - vs2gs[ 0 ].vertex ) );\n"
				"	vec3 c = vs2gs[ 0 ].color;\n"
				"	gl_Position = gl_in[ 0 ].gl_Position;\n"
				"	gs2fs.color  = vec4( c, 1 );\n"
				"	gs2fs.normal = vec4( n, 1 );\n"
				"	EmitVertex( );\n"
				"	gl_Position = gl_in[ 1 ].gl_Position;\n"
				"	gs2fs.color  = vec4( c, 1 );\n"
				"	gs2fs.normal = vec4( n, 1 );\n"
				"	EmitVertex( );\n"
				"	gl_Position = gl_in[ 2 ].gl_Position;\n"
				"	gs2fs.color  = vec4( c, 1 );\n"
				"	gs2fs.normal = vec4( n, 1 );\n"
				"	EmitVertex( );\n"
				"	EndPrimitive( );\n"
				"}\n",

				// fragment shader
				"#version 330 core\n"

				//	light in every space that makes sense to show their behavior
				"in GS2FS {\n"
				"	vec4 color, normal;\n"
				"} gs2fs;\n"
				"out vec4 fColor;\n"
				"void main( ) {\n"
//				"	fColor = vec4( .9, .7, .1, 1. );\n"
				"	fColor = gs2fs.color;\n"
				"	fColor.xyz *= .1 + .9 * max( 0, dot( gs2fs.normal.xyz, vec3( 0, 0, 1 ) ) );\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE ).
				addUniform( "model",  GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
				addUniform( "view",   GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
				addUniform( "proj",   GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection );
		}
	}

	// textures
	{
		// T-CHARACTERS
		{
			glr.texture(
			"T-CHARACTERS",
			new GLR::Texture( "txChars", "../EZGL/pix/characters.png" ) );
		}
	}

	// containers
	{
		// C-PRINT-A-CHARACTER
		{
			glr.container( "C-PRINT-A-CHARACTER" ).
				setVertexArray( "V-QUAD-3D" ).
				setIndexArray( "I-QUAD-3D" ).
				setShader( "S-CHARACTER" ).
				addInTexture( "T-CHARACTERS" ).
				build( );
		}
		// C-YET-ANOTHER-SPHERE
		{
			glr.container( "C-YET-ANOTHER-SPHERE" ).
				setVertexArray( "V-YET-ANOTHER-SPHERE" ).
				setIndexArray( "I-YET-ANOTHER-SPHERE" ).
				setShader( "S-YET-ANOTHER-SPHERE" ).
				build( );
		}
	}
}

void
YetAnotherSphere::paint( ) {

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	model = glm::mat4( 1. );

	model = glm::translate( model, glm::vec3( -7., 0., -7. ) );
	model = glm::rotate( model, 7.1f * vcd->time, glm::vec3( 1., 0., 0. ) );

	for( GLsizei i = -7; i <= 7; ++ i ) {

		letter = glm::vec2( ( 8.f + i ) / 16.f, 11.f / 16.f );
		glr.run( { "C-PRINT-A-CHARACTER" } );

		model = glm::translate( model, glm::vec3( 1., 0., 0. ) );
		model = glm::rotate( model, -.15f / ( 8.f + i ) * vcd->time, glm::vec3( 1., 0., 0. ) );
	}

	model = glm::mat4( 1 );
	model = glm::translate( model, glm::vec3( 0., 0., -7. ) );
	model = glm::rotate( model, 1.f * vcd->time, glm::vec3( 0, 1, 0 ) );
//	model = glm::rotate( model, 1.f * vcd->time, glm::vec3( sinf( .1 * vcd->time ), cosf( .11 * vcd->time ), sinf( .12 * vcd->time ) ) );
	model = glm::scale( model, glm::vec3( 4. ) );

	//glEnable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );

	glr.run( { "C-YET-ANOTHER-SPHERE" } );
}

void
YetAnotherSphere::resize( int p_width, int p_height ) {

	// get aspect ratio
	float
	ratio = ( 1.f * p_width / p_height );

	// create a projection matrix
	projection = glm::perspective( 45.0f, ratio, 1.0f, 100.f );
}
