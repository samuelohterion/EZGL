#include "spherewithtexture.hpp"
#include "../../code/glmprinter.hpp"

SphereWithTexture::SphereWithTexture( const CStr &p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
SphereWithTexture::init( ) {

	GLsizei
	xSize = 32,
	ySize = 64;

	// frame buffer
	{
	}

	// vertex arrays
	{
		// VA-SPHERE-WITH-TEXTURE-SPHERE
		{
			GLRenderer::VertexArray
			& va =
				glr.vertices( "VA-SPHERE-WITH-TEXTURE-SPHERE" ).
					setUsage( GL_STATIC_DRAW ).
					addAttrib( "vertex", 3, 0 ). addAttrib( "coord", 2, 3 );

			va << 0.f << 1.f << 0.f << .0f << 1.f;

			GLfloat
			dx = 1.f / xSize,
			dy = 1.f / ySize;

			for( GLsizei y = 1; y < ySize; ++ y ) {

				GLfloat
				cy = cosf( 3.14159f * y / ySize ),
				sy = sinf( 3.14159f * y / ySize );

				for( GLsizei x = 0; x <= xSize; ++ x ) {

					GLfloat
					cx = cosf( 6.2838f * x / xSize ),
					sx = sinf( 6.2838f * x / xSize );

					V3
					v = V3( sy * sx , cy, sy * cx );

					V2
					c = V2( dx * x, 1.f - dy * y );

					va << v << c;
				}
			}

			va << 0.f << -1.f << 0.f << 1.f << 0.f;
		}
	}

	// index arrays
	{
		// IA-SPHERE-WITH-TEXTURE-SPHERE
		{
			GLRenderer::IndexArray
			& ia =
				glr.indices( "IA-SPHERE-WITH-TEXTURE-SPHERE" ).
					setUsage( GL_STATIC_DRAW );

			GLsizei
			y = 0;

			for( GLsizei x = 0; x < xSize; ++x ) {

				GLushort
				a = 0,
				b = 1 + x,
				c = 2 + x;

				ia << a << b << c;
			}

			for( y = 0; y < ySize - 1; ++ y ) {

				for( GLsizei x = 0; x < xSize; ++ x ) {

					GLushort
					a = 1 + ( xSize + 1 ) * ( y + 0 ) + x,
					b = 1 + ( xSize + 1 ) * ( y + 1 ) + x,
					c = 2 + ( xSize + 1 ) * ( y + 1 ) + x;

					ia << a << b << c;

					a = 2 + ( xSize + 1 ) * ( y + 1 ) + x;
					b = 2 + ( xSize + 1 ) * ( y + 0 ) + x;
					c = 1 + ( xSize + 1 ) * ( y + 0 ) + x;

					ia << a << b << c;
				}
			}

			for( GLsizei x = 0; x < xSize; ++x ) {

				GLushort
				a = 2 + ( xSize + 1 ) * ( y - 1 ) + x,
				b = 1 + ( xSize + 1 ) * ( y - 1 ) + x,
				c = 1 + ( xSize + 1 ) * ( y + 0 );

				ia << a << b << c;
			}

			ia << GLRenderer::IndexArray::Object( 0, ia.indexCount( ), GL_TRIANGLES );
		}
	}

	// shaders
	{
		// SH-SPHERE-WITH-TEXTURE-SPHERE
		{
			glr.shader(
				"SH-SPHERE-WITH-TEXTURE-SPHERE",

				// vertex shader
				"#version 330 core\n"
				"in vec3 vertex;\n"
				"in vec2 coord;\n"
				"out VS2FS {\n"
				"	vec4 vertex;\n"
				"	vec4 normal;\n"
				"	vec2 coord;\n"
				"} vs2fs;\n"
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 proj;\n"

				"void main( ) {\n"
				"	vs2fs.vertex = model * vec4( vertex, 1 );\n"
				"	vs2fs.normal = model * vec4( vertex, 0 );\n"
				"	vs2fs.coord  = coord;\n"
				"	gl_Position = proj * view * model * vec4( vertex, 1 );\n"
				"}\n",

				// fragment shader
				"#version 330 core\n"

				//	light in every space that makes sense to show their behavior
				"uniform vec3 lightP;"
				"uniform vec3 lightC;"
				"uniform sampler2D txEarth;"
				"in VS2FS {\n"
				"	vec4 vertex;\n"
				"	vec4 normal;\n"
				"	vec2 coord;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( ) {\n"
				"	vec3  v = lightP - vs2fs.vertex.xyz;\n"
				"	float a = dot( vs2fs.normal.xyz, v );\n"
				"	fColor = texture( txEarth, vs2fs.coord );\n"
				"	fColor.xyz *= min( max( .1, ( a + dot( vs2fs.normal.xyz, vec3( 0,0,1 ) ) ) ), 1 );\n"
				"}\n",
				GLRenderer::ShaderCode::FROM_CODE ).
				addUniform( "model",  GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model ).
				addUniform( "view",   GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view ).
				addUniform( "proj",   GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection ).
				addUniform( "lightP", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightP ).
				addUniform( "lightC", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightC );
		}
	}

	// textures
	{
		// TX-SPHERE-WITH-TEXTURE-SPHERE
		{
			glr.texture(
				"TX-SPHERE-WITH-TEXTURE-SPHERE",
				new GLRenderer::Texture( "txEarth", "../EZGL/glprojects/spherewithtexture/pix/earth.png" ) );
		}
	}

	// programs
	{
		// PR-SPHERE-WITH-TEXTURE-SPHERE
		{
			glr.program( "PR-SPHERE-WITH-TEXTURE-SPHERE" ).
				setVertexArray( "VA-SPHERE-WITH-TEXTURE-SPHERE" ).
				setIndexArray( "IA-SPHERE-WITH-TEXTURE-SPHERE" ).
				setShader( "SH-SPHERE-WITH-TEXTURE-SPHERE" ).
				addInTexture( "TX-SPHERE-WITH-TEXTURE-SPHERE" ).
				build( );
		}
	}

	glClearColor( .01f, .02f, .03f, 1. );

	projection = view = model = glm::mat4( 1. );

//	view = glm::lookAt( glm::vec3( 0., 0., 4. ), glm::vec3( 0., 0., 0. ), glm::vec3( 0., 1., 0. ) );
}

void
SphereWithTexture::paint( ) {

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	model = glm::mat4( 1. );

	model = glm::translate( model, glm::vec3( 0., 0., -4. ) );
	model = glm::rotate( model, 1.1f * vcd->time, glm::vec3( 0., 1., 0. ) );

	lightP = V3( +2 * cosf( vcd->time ), 0., -4. + 2. * sinf( vcd->time ) );
	lightC = V3( 1., 1., 1. );

	glr.run( { "PR-SPHERE-WITH-TEXTURE-SPHERE" } );
}

void
SphereWithTexture::resize( int p_width, int p_height ) {

	// get aspect ratio
	float
	ratio = ( 1.f * p_width / p_height );

	// create a projection matrix
	projection = glm::perspective( 45.0f, ratio, 1.0f, 100.f );
}
