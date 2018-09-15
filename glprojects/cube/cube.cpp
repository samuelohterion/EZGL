#include "cube.hpp"

Cube::Cube( CStr const & p_name, ViewControlData * p_vcd ) :
GLProject( p_name, p_vcd ),
textureBuilt( false ) {

}

void
Cube::init( ) {

	glm::vec3
	px( 1., 0., 0. ),
	py( 0., 1., 0. ),
	pz( 0., 0., 1. ),
	p0 = -px -py -pz,
	p1 = +px -py -pz,
	p2 = -px +py -pz,
	p3 = +px +py -pz,
	p4 = -px -py +pz,
	p5 = +px -py +pz,
	p6 = -px +py +pz,
	p7 = +px +py +pz;

	lightColors[ 0 ] = px;
	lightColors[ 1 ] = py;
	lightColors[ 2 ] = pz;
	lightColors[ 3 ] = px + py;
	lightColors[ 4 ] = py + pz;
	lightColors[ 5 ] = pz + px;
	lightColors[ 6 ] = px + py + pz;

	textureWidth = textureHeight = 128;

	// frame buffer
	{
		// F-SOME-NICE-TEXTURE
		{
			glr.frameBuffer( "F-SOME-NICE-TEXTURE" );
		}
	}

	// textures
	{
		// T-SOME-NICE-TEXTURE-CHECKERBOARD
		{
			glr.texture(
				"T-SOME-NICE-TEXTURE-CHECKERBOARD",
				new GLR::Texture(
					"TX1",
					GL_TEXTURE_2D,
					0,
					GL_RGBA32F,
					GL_NEAREST, GL_NEAREST,
					GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
					GL_RGBA, GL_FLOAT,
					textureWidth, textureHeight ) );
		}
		// T-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST
		{
			glr.texture(
				"T-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST",
				new GLR::Texture(
					"TX2",
					GL_TEXTURE_2D,
					0,
					GL_RGBA32F,
					GL_NEAREST, GL_NEAREST,
					GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
					GL_RGBA, GL_FLOAT,
					textureWidth, textureHeight ) );
		}
	}

	// vertex arrays
	{
		// V-SOME-NICE-TEXTURE
		{
			glr.vertices( "V-SOME-NICE-TEXTURE" ).
					setUsage( GL_STATIC_DRAW ).
					attrib( "vertex", 0, 2 ) <<
					-1.f << -1.f <<
					+1.f << -1.f <<
					+1.f << +1.f <<
					-1.f << +1.f <<
					GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
		// V-LIGHTS
		{
			glr.vertices( "V-LIGHTS" ).
				setUsage( GL_DYNAMIC_DRAW ).
				attrib( "vertex", 0, 3 ).
				attrib( "color", 3, 3 ) <<

					.0f << .0f << .0f << lightColors[ 0 ] <<
					.0f << .0f << .0f << lightColors[ 1 ] <<
					.0f << .0f << .0f << lightColors[ 2 ] <<
					.0f << .0f << .0f << lightColors[ 3 ] <<
					.0f << .0f << .0f << lightColors[ 4 ] <<
					.0f << .0f << .0f << lightColors[ 5 ] <<
					.0f << .0f << .0f << lightColors[ 6 ] <<
					GLR::VertexArray::Object( 0, 7, GL_POINTS );
		}
		// V-CUBE
		{
			/*
			*    2----3
			*   /|   /|
			*  6-+--7 |
			*  | 0--+-1
			*  |/   |/
			*  4----5
			*/

			glr.vertices( "V-CUBE" ).
			attrib( "vertex", 0, 3 ).	attrib( "color", 3, 3 ) <<

				p0 << p0 << p1 << p0 << p5 << px << p4 << px <<
				p6 << py << p2 << py <<
				p3 << pz << p1 << pz <<
				GLR::VertexArray::Object( 0, 8, GL_TRIANGLE_FAN ) <<

				p7 << p0 << p3 << p0 << p2 << ( py + pz ) << p6 << ( py + pz ) <<
				p4 << ( px + py ) << p5 << ( px + py ) <<
				p1 << ( px + pz ) << p3 << ( px + pz ) <<
				GLR::VertexArray::Object( 8, 8, GL_TRIANGLE_FAN );
		}
	}

	// index arrays
	{
	}

	// shaders
	{
		// S-SOME-NICE-TEXTURE-CHECKERBOARD
		{
			glr.shader(
				"S-SOME-NICE-TEXTURE-CHECKERBOARD",

				// VERTEX SHADER
				"#version 330 core\n"
				"in  vec2 vertex;\n"
				"out vec2 vCoords;\n"
				"void main( ) {\n"
				"	vCoords = .5 + .5 * vertex;\n"
				"	gl_Position = vec4( vertex, 0, 1 );\n"
				"}\n",

				// FRAGMENT SHADER
				"#version 330 core\n"
				"in  vec2 vCoords;\n"
				"out vec4 fColor;\n"
				"float random ( in vec2 st) {\n"
					"return fract( sin( dot( st.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453123 );\n"
				"}\n"
				"void main( ) {\n"
				"	fColor = ( ( ( int( 8. * vCoords.x ) + int( 8. * vCoords.y ) ) % 2 ) < 1 ) ? vec4( .7, .7, .7, 1. ) : vec4( 0,.0,.0, 1. );\n"
				"	fColor.xyz += vec3( .3 * random( vCoords ) );\n"
				"}\n",

				GLR::ShaderCode::FROM_CODE );
		}
		// S-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST
		{
			glr.shader(
				"S-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST",

				// VERTEX SHADER
				"/home/friedrich/Development/c++/2018.06.03/EZGL/glprojects/robertslighttest2D/robertslighttest2D.vsh",

				// FRAGMENT SHADER
				"/home/friedrich/Development/c++/2018.06.03/EZGL/glprojects/robertslighttest2D/robertslighttest2D.fsh",

				GLR::ShaderCode::FROM_FILE ).
				addUniform( "time", GLR::Shader::FLOAT, GLR::Shader::SCALAR, & vcd->time ).
				addUniform( "width", GLR::Shader::INT,  GLR::Shader::SCALAR, & textureWidth ).
				addUniform( "height", GLR::Shader::INT, GLR::Shader::SCALAR, & textureHeight );
		}
		// S-LIGHTS
		{
			glr.shader(
				"S-LIGHTS",

				// vertex shader
				"#version 330 core\n"
				"in vec3 vertex;\n"
				"in vec3 color;\n"
				"out VS2FS {\n"
					"vec3 vertex, color;\n"
				"} vs2fs;\n"
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 projection;\n"

				"void main( ) {\n"
					"vec4 v = projection * view * model * vec4( vertex, 1 );\n"
					"vs2fs.vertex = v.xyz;\n"
					"vs2fs.color  = color;\n"
					"gl_PointSize = 25.- .25 * v.z;\n"
					"gl_Position  = v;\n"
				"}\n",

				// fragment shader
				"#version 330 core\n"
				"in VS2FS {\n"
					"vec3 vertex, color;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( void ) {\n"
					"vec2  v = 2. * gl_PointCoord.xy - 1.;"
					"float s = dot( v, v );"
					"if( s > .999 )"
						"discard;"
					"fColor = clamp( 1. * ( 1. - .5 * s ), .1, 1 ) * vec4( vec3( 1. ) + vs2fs.color, 2. );\n"
				"}\n",

				GLR::ShaderCode::FROM_CODE ).
				addUniform( "model", GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
				addUniform( "view", GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
				addUniform( "projection", GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection );
		}
		// S-CUBE
		{
			glr.shader(
				"S-CUBE",

				// vertex shader
				"#version 330 core\n"
				"in vec3 vertex;\n"
				"in vec3 color;\n"
				"out VS2GS {\n"
				"	vec3 vertex, color, frame;\n"
				"	int vertexID;\n"
				"} vs2gs;\n"
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 projection;\n"
				"void main( ) {\n"
				"	vs2gs.vertexID = gl_VertexID;\n"
				"	vs2gs.frame = vertex;\n"
				"	vec4\n"
				"	v = view * model * vec4( vertex, 1. );\n"
				"	vs2gs.vertex = v.xyz;\n"
				"	vs2gs.color  = color;\n"
				"	gl_Position = projection * v;\n"
				"}\n",

				// geometry shader
				"#version 330 core\n"
				"layout ( triangles ) in;\n"
				"layout ( triangle_strip, max_vertices = 3 ) out;\n"
				"in VS2GS {\n"
				"	vec3 vertex, color, frame;\n"
				"	int vertexID;\n"
				"} vs2gs[ ];\n"
				"out GS2FS {\n"
				"	vec3 vertex, color, frame, normal;\n"
				"	vec2 coord;\n"
				"} gs2fs;\n"
				"void main( ) {\n"
				"	int id = vs2gs[ 0 ].vertexID;\n"
				"	vec3 n = normalize( cross( vs2gs[ 1 ].vertex - vs2gs[ 0 ].vertex, vs2gs[ 2 ].vertex - vs2gs[ 0 ].vertex ) );\n"
				"	gl_Position  = gl_in[ 0 ].gl_Position;\n"
				"	gs2fs.frame  = vs2gs[ 0 ].frame;\n"
				"	gs2fs.vertex = vs2gs[ 0 ].vertex;\n"
				"	gs2fs.normal = n;\n"
				"	gs2fs.color  = vs2gs[ 2 ].color;\n"
				"	gs2fs.coord  = vec2( 0, 0 );\n"
				"	EmitVertex( );\n"

				"	gl_Position = gl_in[ 1 ].gl_Position;\n"
				"	gs2fs.frame = vs2gs[ 1 ].frame;\n"
				"	gs2fs.vertex = vs2gs[ 1 ].vertex;\n"
				"	gs2fs.normal = n;\n"
				"	gs2fs.color  = vs2gs[ 2 ].color;\n"
				"	gs2fs.coord  = ! all( equal( vs2gs[ 1 ].color, vs2gs[ 2 ].color ) ) ? vec2( 1, 0 ) :  vec2( 1, 1 );\n"
				"	EmitVertex( );\n"

				"	gl_Position = gl_in[ 2 ].gl_Position;\n"
				"	gs2fs.frame = vs2gs[ 2 ].frame;\n"
				"	gs2fs.vertex = vs2gs[ 2 ].vertex;\n"
				"	gs2fs.normal = n;\n"
				"	gs2fs.color  = vs2gs[ 2 ].color;\n"
				"	gs2fs.coord  = ! all( equal( vs2gs[ 1 ].color, vs2gs[ 2 ].color ) ) ? vec2( 1, 1 ) :  vec2( 0, 1 );\n"
				"	EmitVertex( );\n"

				"	EndPrimitive( );\n"
				"}\n",

				// fragment shader
				"#version 330 core\n"
				"in GS2FS {\n"
				"	vec3 vertex, color, frame, normal;\n"
				"	vec2 coord;\n"
				"} gs2fs;\n"
				"uniform mat4 model;\n"
				"uniform mat4 view;\n"
				"uniform mat4 projection;\n"
				"uniform vec3 lightPos[ 7 ];\n"
				"uniform vec3 lightCol[ 7 ];\n"
				"uniform float time;\n"
				"uniform sampler2D TX1;\n"
				"uniform sampler2D TX2;\n"
				"out vec4 fColor;\n"
				"void main( void ) {\n"
				"	vec4 col = vec4( .3 * gs2fs.color + pow( .5 + .5 * sin( .05 * time ), 5. ) * texture( TX1, gs2fs.coord ).xyz + pow( .5 - .5 * sin( .05 * time ), 5. ) * texture( TX2, gs2fs.coord ).xyz, 1 );\n"
				"	vec3\n"
				"	d = vec3( 0., 0., +1. ),\n"
				"	n = gs2fs.normal;\n"
				"	float a = dot( n, d );\n"
				"	vec4 f = vec4( clamp( a, 0, 1 ) * col.xyz, 1 );\n"
				"	int i = 0;\n"
				"	for( i = i; i < 7; ++ i ) {"
				"		d = lightPos[ i ] - gs2fs.vertex;\n"
				"		a = 10. * normalize( dot( n, d ) );\n"
				"		f += vec4( clamp( a / dot( d, d ), 0, 1 ) * lightCol[ i ] * col.xyz, 1.f );\n"
				"	}\n"
				"	fColor = vec4( clamp( 2. * f.xyz / f.a, vec3( 0 ), vec3( 1 ) ), 1. );\n"
				"}\n",

				GLR::ShaderCode::FROM_CODE ).
				addUniform( "model", GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
				addUniform( "view", GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
				addUniform( "projection", GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection ).
				addUniform( "lightPos[0]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightCameraSpace[ 0 ] ).
				addUniform( "lightPos[1]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightCameraSpace[ 1 ] ).
				addUniform( "lightPos[2]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightCameraSpace[ 2 ] ).
				addUniform( "lightPos[3]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightCameraSpace[ 3 ] ).
				addUniform( "lightPos[4]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightCameraSpace[ 4 ] ).
				addUniform( "lightPos[5]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightCameraSpace[ 5 ] ).
				addUniform( "lightPos[6]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightCameraSpace[ 6 ] ).
				addUniform( "lightCol[0]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightColors[ 0 ] ).
				addUniform( "lightCol[1]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightColors[ 1 ] ).
				addUniform( "lightCol[2]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightColors[ 2 ] ).
				addUniform( "lightCol[3]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightColors[ 3 ] ).
				addUniform( "lightCol[4]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightColors[ 4 ] ).
				addUniform( "lightCol[5]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightColors[ 5 ] ).
				addUniform( "lightCol[6]", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightColors[ 6 ] ).
				addUniform( "time",        GLR::Shader::FLOAT, GLR::Shader::SCALAR, & vcd->time );
		}
	}

	// container
	{
		// C-COORDINATE-SYSTEM
		{
			glr.container( "C-SOME-NICE-TEXTURE-CHECKERBOARD" ).
				setFrameBuffer( "F-SOME-NICE-TEXTURE" ).
				setVertexArray( "V-SOME-NICE-TEXTURE" ).
				setShader( "S-SOME-NICE-TEXTURE-CHECKERBOARD" ).
				addOutTexture( "T-SOME-NICE-TEXTURE-CHECKERBOARD" ).
				setFixSize( textureWidth, textureHeight ).
				build( );
		}
		// C-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST
		{
			glr.container( "C-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST" ).
				setVertexArray( "V-SOME-NICE-TEXTURE" ).
				setShader( "S-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST" ).
					setFrameBuffer( "F-SOME-NICE-TEXTURE" ).
					addOutTexture( "T-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST" ).
					setFixSize( textureWidth, textureHeight ).
				build( );
		}
		// C-LIGHTS
		{
			glr.container( "C-LIGHTS" ).
				setShader( "S-LIGHTS" ).
				setVertexArray( "V-LIGHTS" ).
				build( );
		}
		// C-CUBE
		{
			glr.container( "C-CUBE" ).
				setShader( "S-CUBE" ).
				setVertexArray( "V-CUBE" ).
				addInTexture( "T-SOME-NICE-TEXTURE-CHECKERBOARD" ).
				addInTexture( "T-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST" ).
				build( );
		}
	}

	glClearColor( .11f, .13f, .12f, 1. );

	projection = view = model = glm::mat4( 1. );

	view  = glm::translate( view, glm::vec3( 0.f, 0.f, -50.f ) );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	angle = vcd->time;
}

void
Cube::paint( ) {

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if( ! textureBuilt ) {

		glr.run( { "C-SOME-NICE-TEXTURE-CHECKERBOARD" } );

		textureBuilt = true;
	}

	glr.run( { "C-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST" } );

	// ----------------------------------------------------------
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
	// ccv.rotate_around_z( .01f );

	// get new model matrix
	model = ccv.model( );

	// get new view matrix
	view  = ccv.view( );
	// ----------------------------------------------------------

	angle = vcd->time;

	model = glm::rotate( model, .0123f, normalize( glm::vec3( sinf( .1f * angle ), sinf( .21f ), sinf( .31f ) ) ) );

	GLR::VertexArray
	& va = glr.vertices( "V-LIGHTS" );

	glEnable( GL_VERTEX_ATTRIB_ARRAY_NORMALIZED );
	glEnable( GL_VERTEX_PROGRAM_POINT_SIZE );

	float
	phase = 3.14159f * sinf( .023f * angle );

	for( std::size_t i = 0; i < 7; ++ i ) {

		lightModelSpace[ i ] = glm::vec4( 7.f * cosf( phase * i + angle ), 13.f * sinf( phase * i + 1.2f * angle ), 23.f * sinf( phase * i + angle ), 1.f );

		lightCameraSpace[ i ] = view * model * lightModelSpace[ i ];

		va.arr[ 6 * i + 0 ] = lightModelSpace[ i ].x;
		va.arr[ 6 * i + 1 ] = lightModelSpace[ i ].y;
		va.arr[ 6 * i + 2 ] = lightModelSpace[ i ].z;
	}

	glr.run( { "C-LIGHTS" } );

	glDisable( GL_VERTEX_PROGRAM_POINT_SIZE );
	glDisable( GL_VERTEX_ATTRIB_ARRAY_NORMALIZED );

	glm::mat4
	tmp = model;

	for( int z = -10; z <= 10; ++ z ) {

		for( int y = -5; y <= 5; ++ y ) {

			for( int x = -2; x <= 2; ++ x ) {

				float
				f = cosf( 3.14f * sinf( .05f * vcd->time ) );

				f = f * f;
				f = f * f;
				f = f * f;
				f = f * f;
				f = f * f;
				f = f * f;
				f = 1.f - f;

				model = glm::translate( tmp, ( 6.f - 4.f * f ) * glm::vec3( x, y, z ) );
				model = glm::rotate( model, ( x + y + z ) * 3.14f * ( 1.f - f ) * ( 1.f - f ) * ( 1.f - f ), glm::vec3( sinf( .1f * angle ), sinf( .21f * angle ), sinf( .31f * angle ) ) );

				glr.run( { "C-CUBE" } );
			}
		}
	}

	model = tmp;
}

void
Cube::resize( int p_width, int p_height ) {

	float
	ratio = 1.f * p_width / p_height;

	projection = glm::perspective( 45.0f, ratio, 1.0f, 200.f );
}
