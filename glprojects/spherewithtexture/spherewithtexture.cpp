#include "spherewithtexture.hpp"
#include "../../code/glmprinter.hpp"

SphereWithTexture::SphereWithTexture ( const CStr &p_name ) :
GLProject ( p_name ) {

}

void
SphereWithTexture::init ( ) {

	GLsizei
	xSize = 64,
	ySize = 32;

	// vertex arrays
	{
		// VA-SPHERE-WITH-TEXTURE-SPHERE
		{
			GLR::VertexArray
			& va =
				glr.vertices( "VA-SPHERE-WITH-TEXTURE-SPHERE" ).
					setUsage( GL_STATIC_DRAW ).
					attrib( "vertex", 0, 3 ).
					attrib( "coord", 3, 2 );

			va << 0.f << 1.f << 0.f << .5f << 1.f;

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

			va << 0.f << -1.f << 0.f << .5f << 0.f;
		}
	}

	// index arrays
	{
		// IA-SPHERE-WITH-TEXTURE-SPHERE
		{
			GLR::IndexArray
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

			for( y = 0; y < ySize - 2; ++ y ) {

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
				a = 2 + ( xSize + 1 ) * ( y + 0 ) + x,
				b = 1 + ( xSize + 1 ) * ( y + 0 ) + x,
				c = 1 + ( xSize + 1 ) * ( y + 1 );

				ia << a << b << c;
			}

			ia << GLR::IndexArray::Object( 0, ia.indexCount( ), GL_TRIANGLES );
		}
	}

	// shaders
	{
		// SH-SPHERE-WITH-TEXTURE-EARTH
		{
			glr.shader(
				"SH-SPHERE-WITH-TEXTURE-EARTH",

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
				"	vs2fs.normal = normalize( model * vec4( vertex, 0 ) );\n"
				"	vs2fs.coord  = coord;\n"
				"	gl_Position = proj * view * model * vec4( vertex, 1 );\n"
				"}\n",

				// fragment shader
				"#version 330 core\n"

				//	light in every space that makes sense to show their behavior
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 proj;\n"
				"uniform vec3 lightP;"
				"uniform vec3 lightC;"
				"uniform sampler2D txSphere;"
				"uniform sampler2D txClouds;"
				"uniform sampler2D txNightmap;"
				"in VS2FS {\n"
				"	vec4 vertex;\n"
				"	vec4 normal;\n"
				"	vec2 coord;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( ) {\n"
				"	vec3  v = normalize( lightP - vs2fs.vertex.xyz );\n"
				"	float a = dot( vs2fs.normal.xyz, v );\n"
				"	vec3\n"
				"		day    = texture( txSphere, vs2fs.coord ).xyz,\n"
				"		clouds = texture( txClouds, vs2fs.coord ).xyz,\n"
				"		night  = texture( txNightmap, vs2fs.coord ).xyz;\n"
				"	fColor.a = 1;\n"
				"	fColor.rgb = mix( night, day + clouds, vec3( a ) );\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE ).
				addUniform( "model",  GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
				addUniform( "view",   GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
				addUniform( "proj",   GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection ).
				addUniform( "lightP", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightP ).
				addUniform( "lightC", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightC );
		}
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
				"	vs2fs.normal = normalize( model * vec4( vertex, 0 ) );\n"
				"	vs2fs.coord  = coord;\n"
				"	gl_Position = proj * view * model * vec4( vertex, 1 );\n"
				"}\n",

				// fragment shader
				"#version 330 core\n"

				//	light in every space that makes sense to show their behavior
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 proj;\n"
				"uniform vec3 lightP;"
				"uniform vec3 lightC;"
				"uniform sampler2D txSphere;"
				"in VS2FS {\n"
				"	vec4 vertex;\n"
				"	vec4 normal;\n"
				"	vec2 coord;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( ) {\n"
				"	vec3  v = normalize( lightP - vs2fs.vertex.xyz );\n"
				"	float a = dot( vs2fs.normal.xyz, v );\n"
				"	fColor = texture( txSphere, vs2fs.coord );\n"
				"	fColor.rgb *= ( .9 * a + .1 );\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE ).
				addUniform( "model",  GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
				addUniform( "view",   GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
				addUniform( "proj",   GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection ).
				addUniform( "lightP", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightP ).
				addUniform( "lightC", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightC );
		}
		// SH-SPHERE-WITH-TEXTURE-SUN
		{
			glr.shader(
				"SH-SPHERE-WITH-TEXTURE-SUN",

				// vertex shader
				"#version 330 core\n"
				"in vec3 vertex;\n"
				"in vec2 coord;\n"
				"out VS2FS {\n"
				"	vec4 vertex;\n"
				"	vec2 coord;\n"
				"} vs2fs;\n"
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 proj;\n"

				"void main( ) {\n"
				"	vs2fs.vertex = model * vec4( vertex, 1 );\n"
				"	vs2fs.coord  = coord;\n"
				"	gl_Position = proj * view * model * vec4( vertex, 1 );\n"
				"}\n",

				// fragment shader
				"#version 330 core\n"

				//	light in every space that makes sense to show their behavior
				"uniform float time;\n"
				"uniform sampler2D txSphere;\n"
				"in VS2FS {\n"
				"	vec4 vertex;\n"
				"	vec2 coord;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"float random ( in vec2 st ) {\n"
					"return fract( sin( dot( st.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453123 );\n"
				"}\n"
				"void main( ) {\n"
				"	fColor =\n"
				"		texture( txSphere, fract( vs2fs.coord + vec2( sin( -.021 * time ), sin( +.012 * time ) ) ) ) +\n"
				"		texture( txSphere, fract( vs2fs.coord + vec2( sin( -.016 * time ), sin( +.014 * time ) ) ) ) +\n"
				"		texture( txSphere, fract( vs2fs.coord + vec2( sin( +.011 * time ), sin( -.018 * time ) ) ) ) +\n"
				"		texture( txSphere, fract( vs2fs.coord + vec2( sin( -.012 * time ), sin( -.019 * time ) ) ) );\n"
				"	fColor.xyz /= fColor.a;\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE ).
				addUniform( "model",  GLR::Shader::MAT4,  GLR::Shader::SCALAR, & model ).
				addUniform( "view",   GLR::Shader::MAT4,  GLR::Shader::SCALAR, & view ).
				addUniform( "proj",   GLR::Shader::MAT4,  GLR::Shader::SCALAR, & projection ).
				addUniform( "time",   GLR::Shader::FLOAT, GLR::Shader::SCALAR, & vcd->time );
		}
	}

	// textures
	{
		// https://www.solarsystemscope.com/textures/
		// TX-SPHERE-WITH-TEXTURE-SPHERE-EARTH-LANDSCAPE
		{
			glr.texture(
				"TX-SPHERE-WITH-TEXTURE-SPHERE-EARTH-LANDSCAPE",
				new GLR::Texture( "txSphere", "../pix/2k_earth_daymap.jpg" ) );
		}
		// TX-SPHERE-WITH-TEXTURE-SPHERE-EARTH-CLOUDS
		{
			glr.texture(
				"TX-SPHERE-WITH-TEXTURE-SPHERE-EARTH-CLOUDS",
				new GLR::Texture( "txClouds", "../pix/2k_earth_clouds.jpg" ) );
		}
		// TX-SPHERE-WITH-TEXTURE-SPHERE-EARTH-NIGHTMAP
		{
			glr.texture(
				"TX-SPHERE-WITH-TEXTURE-SPHERE-EARTH-NIGHTMAP",
				new GLR::Texture( "txNightmap", "../pix/2k_earth_nightmap.jpg" ) );
		}
		// TX-SPHERE-WITH-TEXTURE-SPHERE-MOON
		{
			glr.texture(
				"TX-SPHERE-WITH-TEXTURE-SPHERE-MOON",
				new GLR::Texture( "txSphere", "../pix/2k_moon.jpg" ) );
		}
		// TX-SPHERE-WITH-TEXTURE-SPHERE-SUN
		{
			glr.texture(
				"TX-SPHERE-WITH-TEXTURE-SPHERE-SUN",
				new GLR::Texture( "txSphere", "../pix/2k_sun.jpg" ) );
		}
	}

	// programs
	{
		// PR-SPHERE-WITH-TEXTURE-SPHERE-EARTH
		{
			glr.container( "PR-SPHERE-WITH-TEXTURE-SPHERE-EARTH" ).
				setVertexArray( "VA-SPHERE-WITH-TEXTURE-SPHERE" ).
				setIndexArray( "IA-SPHERE-WITH-TEXTURE-SPHERE" ).
				setShader( "SH-SPHERE-WITH-TEXTURE-EARTH" ).
				addInTexture( "TX-SPHERE-WITH-TEXTURE-SPHERE-EARTH-LANDSCAPE" ).
				addInTexture( "TX-SPHERE-WITH-TEXTURE-SPHERE-EARTH-CLOUDS" ).
				addInTexture( "TX-SPHERE-WITH-TEXTURE-SPHERE-EARTH-NIGHTMAP" ).
				build( );
		}
		// PR-SPHERE-WITH-TEXTURE-SPHERE-MOON
		{
			glr.container( "PR-SPHERE-WITH-TEXTURE-SPHERE-MOON" ).
				setVertexArray( "VA-SPHERE-WITH-TEXTURE-SPHERE" ).
				setIndexArray( "IA-SPHERE-WITH-TEXTURE-SPHERE" ).
				setShader( "SH-SPHERE-WITH-TEXTURE-SPHERE" ).
				addInTexture( "TX-SPHERE-WITH-TEXTURE-SPHERE-MOON" ).
				build( );
		}
		// PR-SPHERE-WITH-TEXTURE-SPHERE-SUN
		{
			glr.container( "PR-SPHERE-WITH-TEXTURE-SPHERE-SUN" ).
				setVertexArray( "VA-SPHERE-WITH-TEXTURE-SPHERE" ).
				setIndexArray( "IA-SPHERE-WITH-TEXTURE-SPHERE" ).
				setShader( "SH-SPHERE-WITH-TEXTURE-SUN" ).
				addInTexture( "TX-SPHERE-WITH-TEXTURE-SPHERE-SUN" ).
				build( );
		}
	}

	glClearColor( .01f, .02f, .03f, 1. );

	projection = view = model = glm::mat4( 1. );

	lightC = V3( 1., 1., 1. );
}

void
SphereWithTexture::paint( ) {

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	view = glm::lookAt( glm::vec3( 0., 0., 30. * vcd->mousey / vcd->height ), glm::vec3( 0., 0., 0. ), glm::vec3( 0., 1., 0. ) );

	GLfloat
	year = .002f * ( vcd->time + 3141.5f * vcd->mousex / vcd->width ),
	day = 365.f * year;

	//model = glm::rotate( model, 30.f / 180.f * 3.14159f, normalize( glm::vec3( 1.f, 0.f, 0.5f ) ) );
	lightP = glm::vec4( 0.f, 0.f, 0.f, 1.f );
	model  = glm::mat4( 1. );
	model = glm::scale( model, glm::vec3( 3.f ) );
	glr.run( { "PR-SPHERE-WITH-TEXTURE-SPHERE-SUN" } );
	model = glm::scale( model, glm::vec3( 1.f / 3.f ) );

	model = glm::rotate( model, year, glm::vec3( 0.f, 1.f, 0.f ) );
	model = glm::translate( model, glm::vec3( 15., 0., 0. ) );

	M4
	tmp = model;
	model = glm::rotate( model, +23.f / 180.f * 3.14159f, normalize( glm::vec3( 0.f, 0.f, 1.0f ) ) );
	model = glm::rotate( model, -day, glm::vec3( 0.f, 1.f, 0.f ) );
	//model = glm::rotate( model, day, glm::vec3( 0.f, cosf( 23.f / 180.f * 3.14f ), sinf( 23.f / 180.f * 3.14f ) ) );

	glr.run( { "PR-SPHERE-WITH-TEXTURE-SPHERE-EARTH" } );

	//model = glm::rotate( model, -23.f / 180.f * 3.14159f, normalize( glm::vec3( 0.f, 0.f, 1.0f ) ) );
	model = glm::rotate( tmp, day / 28.5f, glm::vec3( 0, 1, .0 ) );
	model = glm::translate( model, glm::vec3( 5., 0, 0 ) );
	model = glm::scale( model, glm::vec3( 1.f / 3.f ) );
	glr.run( { "PR-SPHERE-WITH-TEXTURE-SPHERE-MOON" } );
}

void
SphereWithTexture::resize( int p_width, int p_height ) {

	// get aspect ratio
	float
	ratio = ( 1.f * p_width / p_height );

	// create a projection matrix
	projection = glm::perspective( 45.0f, ratio, 1.0f, 300.f );

	glr.screenon ( );
}
