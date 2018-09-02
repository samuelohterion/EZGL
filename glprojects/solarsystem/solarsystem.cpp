#include "solarsystem.hpp"
#include "../../code/glmprinter.hpp"

SolarsSystem::SolarsSystem( const CStr &p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
SolarsSystem::init( ) {

	GLsizei
	xSize = 64,
	ySize = 32;

	// frame buffer
	{
	}

	// vertex arrays
	{
		// V-SPHERE-WITH-TEXTURE-SPHERE
		{
			GLR::VertexArray
			& va =
				glr.vertices( "V-SPHERE-WITH-TEXTURE-SPHERE" ).
					setUsage( GL_STATIC_DRAW ).
					attrib( "vertex", 0, 3 ). attrib( "coord", 3, 2 );

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
		// V-SPHERE-WITH-TEXTURE-ORBIT-LINE-MERCURY
		{
			GLR::VertexArray
			& va =
				glr.vertices( "V-SPHERE-WITH-TEXTURE-ORBIT-LINE-MERCURY" ).
					setUsage( GL_STATIC_DRAW ).
					attrib( "vertex", 0, 3 ).
					attrib( "color", 3, 3 );

			GLfloat
			mercuryDay = 2.f * 3.14159f / 85.f;

			for( GLuint alpha = 0; alpha <= 85; ++ alpha ) {

				GLfloat
				z = 1.f * cosf( mercuryDay * alpha ),
				x = 1.f * sinf( mercuryDay * alpha ),
				c = ( alpha & 1 ) == 1  ? 0.f : 1.f;

				va << x << 0.f << z << c << c << c;
			}

			va << GLR::VertexArray::Object( 0, 86, GL_LINE_STRIP );
		}
	}

	// index arrays
	{
		// I-SPHERE-WITH-TEXTURE-SPHERE
		{
			GLR::IndexArray
			& ia =
				glr.indices( "I-SPHERE-WITH-TEXTURE-SPHERE" ).
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
		// S-SPHERE-WITH-TEXTURE-ORBIT-LINE-MERCURY
		{
			glr.shader(
				"S-SPHERE-WITH-TEXTURE-ORBIT-LINE-MERCURY",

				// vertex shader
				"#version 330 core\n"
				"in vec3 vertex;\n"
				"in vec3 color;\n"
				"out VS2FS {\n"
				"	vec3 color;\n"
				"} vs2fs;\n"
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 proj;\n"

				"void main( ) {\n"
				"	vs2fs.color  = color;\n"
				"	gl_Position = proj * view * model * vec4( vertex, 1 );\n"
				"}\n",

				// fragment shader
				"#version 330 core\n"

				//	light in every space that makes sense to show their behavior
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 proj;\n"
				"in VS2FS {\n"
				"	vec3 color;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( ) {\n"
				"	fColor = vec4( vs2fs.color, 1 );\n"
				"}\n",

				GLR::ShaderCode::FROM_CODE ).
				addUniform( "model",  GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
				addUniform( "view",   GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
				addUniform( "proj",   GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection );
		}
		// S-SPHERE-WITH-TEXTURE-EARTH
		{
			glr.shader(
				"S-SPHERE-WITH-TEXTURE-EARTH",

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
		// S-SPHERE-WITH-TEXTURE-SPHERE
		{
			glr.shader(
				"S-SPHERE-WITH-TEXTURE-SPHERE",

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
		// S-SPHERE-WITH-TEXTURE-SUN
		{
			glr.shader(
				"S-SPHERE-WITH-TEXTURE-SUN",

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
		// T-SPHERE-WITH-TEXTURE-SPHERE-EARTH-LANDSCAPE
		{
			glr.texture(
				"T-SPHERE-WITH-TEXTURE-SPHERE-EARTH-LANDSCAPE",
				new GLR::Texture( "txSphere", "../EZGL/pix/2k_earth_daymap.jpg" ) );
		}
		// T-SPHERE-WITH-TEXTURE-SPHERE-EARTH-CLOUDS
		{
			glr.texture(
				"T-SPHERE-WITH-TEXTURE-SPHERE-EARTH-CLOUDS",
				new GLR::Texture( "txClouds", "../EZGL/pix/2k_earth_clouds.jpg" ) );
		}
		// T-SPHERE-WITH-TEXTURE-SPHERE-EARTH-NIGHTMAP
		{
			glr.texture(
				"T-SPHERE-WITH-TEXTURE-SPHERE-EARTH-NIGHTMAP",
				new GLR::Texture( "txNightmap", "../EZGL/pix/2k_earth_nightmap.jpg" ) );
		}
		// T-SPHERE-WITH-TEXTURE-SPHERE-MOON
		{
			glr.texture(
				"T-SPHERE-WITH-TEXTURE-SPHERE-MOON",
				new GLR::Texture( "txSphere", "../EZGL/pix/2k_moon.jpg" ) );
		}
		// T-SPHERE-WITH-TEXTURE-SPHERE-MERCURY
		{
			glr.texture(
				"T-SPHERE-WITH-TEXTURE-SPHERE-MERCURY",
				new GLR::Texture( "txSphere", "../EZGL/pix/2k_mercury.jpg" ) );
		}
		// T-SPHERE-WITH-TEXTURE-SPHERE-SUN
		{
			glr.texture(
				"T-SPHERE-WITH-TEXTURE-SPHERE-SUN",
				new GLR::Texture( "txSphere", "../EZGL/pix/2k_sun.jpg" ) );
		}
	}

	// containers
	{
		// C-SPHERE-WITH-TEXTURE-ORBIT-LINE-MERCURY
		{
			glr.container( "C-SPHERE-WITH-TEXTURE-ORBIT-LINE-MERCURY" ).
				setVertexArray( "V-SPHERE-WITH-TEXTURE-ORBIT-LINE-MERCURY" ).
				setShader( "S-SPHERE-WITH-TEXTURE-ORBIT-LINE-MERCURY" ).
				build( );
		}
		// C-SPHERE-WITH-TEXTURE-SPHERE-EARTH
		{
			glr.container( "C-SPHERE-WITH-TEXTURE-SPHERE-EARTH" ).
				setVertexArray( "V-SPHERE-WITH-TEXTURE-SPHERE" ).
				setIndexArray( "I-SPHERE-WITH-TEXTURE-SPHERE" ).
				setShader( "S-SPHERE-WITH-TEXTURE-EARTH" ).
				addInTexture( "T-SPHERE-WITH-TEXTURE-SPHERE-EARTH-LANDSCAPE" ).
				addInTexture( "T-SPHERE-WITH-TEXTURE-SPHERE-EARTH-CLOUDS" ).
				addInTexture( "T-SPHERE-WITH-TEXTURE-SPHERE-EARTH-NIGHTMAP" ).
				build( );
		}
		// C-SPHERE-WITH-TEXTURE-SPHERE-MOON
		{
			glr.container( "C-SPHERE-WITH-TEXTURE-SPHERE-MOON" ).
				setVertexArray( "V-SPHERE-WITH-TEXTURE-SPHERE" ).
				setIndexArray( "I-SPHERE-WITH-TEXTURE-SPHERE" ).
				setShader( "S-SPHERE-WITH-TEXTURE-SPHERE" ).
				addInTexture( "T-SPHERE-WITH-TEXTURE-SPHERE-MOON" ).
				build( );
		}
		// C-SPHERE-WITH-TEXTURE-SPHERE-MERCURY
		{
			glr.container( "C-SPHERE-WITH-TEXTURE-SPHERE-MERCURY" ).
				setVertexArray( "V-SPHERE-WITH-TEXTURE-SPHERE" ).
				setIndexArray( "I-SPHERE-WITH-TEXTURE-SPHERE" ).
				setShader( "S-SPHERE-WITH-TEXTURE-SPHERE" ).
				addInTexture( "T-SPHERE-WITH-TEXTURE-SPHERE-MERCURY" ).
				build( );
		}
		// C-SPHERE-WITH-TEXTURE-SPHERE-SUN
		{
			glr.container( "C-SPHERE-WITH-TEXTURE-SPHERE-SUN" ).
				setVertexArray( "V-SPHERE-WITH-TEXTURE-SPHERE" ).
				setIndexArray( "I-SPHERE-WITH-TEXTURE-SPHERE" ).
				setShader( "S-SPHERE-WITH-TEXTURE-SUN" ).
				addInTexture( "T-SPHERE-WITH-TEXTURE-SPHERE-SUN" ).
				build( );
		}
	}

	glClearColor( .01f, .02f, .03f, 1. );

	projection = view = model = glm::mat4( 1. );

	lightC = V3( 1., 1., 1. );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
}

void
SolarsSystem::paint( ) {

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	view = glm::lookAt( glm::vec3( 0., 0., 15. * vcd->mousey / vcd->height ), glm::vec3( 0., 0., 0. ), glm::vec3( 0., 1., 0. ) );

	GLfloat
	year = .002f * ( vcd->time + 3141.5f * vcd->mousex / vcd->width ),
	day = 365.f * year;

	lightP = glm::vec4( 0.f, 0.f, 0.f, 1.f );

	model  = glm::mat4( 1. );
	model = glm::rotate( model, 30.f / 180.f * 3.1415f, glm::vec3( 1.f, 0.f, 0.f ) );

	M4
	tmp = model;

	model = glm::scale( model, glm::vec3( 8.f ) );
	glr.run( { "C-SPHERE-WITH-TEXTURE-ORBIT-LINE-MERCURY" } );

	model = tmp;
	model = glm::scale( model, glm::vec3( 3.f ) );

	glr.run( { "C-SPHERE-WITH-TEXTURE-SPHERE-SUN" } );

	model = tmp;
	model = glm::rotate( model, 365.25f / 85.f * year, glm::vec3( 0.f, 1.f, 0.f ) );
	model = glm::translate( model, glm::vec3( 8., 0., 0. ) );
	glr.run( { "C-SPHERE-WITH-TEXTURE-SPHERE-MERCURY" } );

	model = tmp;
	model = glm::rotate( model, year, glm::vec3( 0.f, 1.f, 0.f ) );
	model = glm::translate( model, glm::vec3( 15., 0., 0. ) );
	tmp   = model;
	model = glm::rotate( model, 23.f / 180.f * 3.1415f, glm::vec3( 1.f, 0.f, 0.f ) );
	model = glm::rotate( model, day, glm::vec3( 0.f, 1.f, 0.f ) );
	glr.run( { "C-SPHERE-WITH-TEXTURE-SPHERE-EARTH" } );

	model = tmp;
	model = glm::rotate( model, day / 28.5f, glm::vec3( 0, 1, .0 ) );
	model = glm::translate( model, glm::vec3( 4., 0, 0 ) );
	model = glm::scale( model, glm::vec3( 1.f / 3.f ) );
	glr.run( { "C-SPHERE-WITH-TEXTURE-SPHERE-MOON" } );
}

void
SolarsSystem::resize( int p_width, int p_height ) {

	// get aspect ratio
	float
	ratio = ( 1.f * p_width / p_height );

	// create a projection matrix
	projection = glm::perspective( 45.0f, ratio, 1.0f, 300.f );
}
