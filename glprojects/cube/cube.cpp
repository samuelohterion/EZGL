#include "cube.hpp"

Cube::Cube( CStr const & p_name, ViewControlData * p_vcd ) :
GLProject( p_name, p_vcd ),
textureBuilt( false ) {

}

void
Cube::init( ) {

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClearColor( 0., 0., 0., 1. );

	angle = vcd->time;

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

	glr.frameBuffer( "FRAMEBUFFER-SOME-NICE-TEXTURE" );

	glr.vertices( "VERTICES-SOME-NICE-TEXTURE" ).
		setUsage( GL_STATIC_DRAW ).
		addAttrib( "vertex", 2, 0 ) <<
		-1.f << -1.f <<
		+1.f << -1.f <<
		+1.f << +1.f <<
		-1.f << +1.f <<
		GLRenderer::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );

	glr.texture(
		"TEXTURE-SOME-NICE-TEXTURE-CHECKERBOARD",
		new GLRenderer::Texture(
			"TX1",
			GL_TEXTURE_2D,
			0,
			GL_RGBA32F,
			GL_NEAREST, GL_NEAREST,
			GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
			GL_RGBA, GL_FLOAT,
			textureWidth, textureHeight ) );

	glr.shader(
		"SHADER-SOME-NICE-TEXTURE-CHECKERBOARD",
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
		GLRenderer::ShaderCode::FROM_CODE );

	glr.program( "PROGRAM-SOME-NICE-TEXTURE-CHECKERBOARD" ).
		setFrameBuffer( "FRAMEBUFFER-SOME-NICE-TEXTURE" ).
		setVertexArray( "VERTICES-SOME-NICE-TEXTURE" ).
		setShader( "SHADER-SOME-NICE-TEXTURE-CHECKERBOARD" ).
		addOutTexture( "TEXTURE-SOME-NICE-TEXTURE-CHECKERBOARD" ).
		setFixSize( textureWidth, textureHeight ).
		build( );


	glr.texture(
		"TEXTURE-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST",
		new GLRenderer::Texture(
			"TX2",
			GL_TEXTURE_2D,
			0,
			GL_RGBA32F,
			GL_NEAREST, GL_NEAREST,
			GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
			GL_RGBA, GL_FLOAT,
			textureWidth, textureHeight ) );

	glr.shader(
		"SHADER-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST",
		// VERTEX SHADER
		"/home/friedrich/Development/c++/2018.06.03/EZGL/glprojects/robertslighttest2D/robertslighttest2D.vsh",
		// FRAGMENT SHADER
		"/home/friedrich/Development/c++/2018.06.03/EZGL/glprojects/robertslighttest2D/robertslighttest2D.fsh",
		GLRenderer::ShaderCode::FROM_FILE ).
		addUniform( "time", GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, & vcd->time ).
		addUniform( "width", GLRenderer::Shader::INT,  GLRenderer::Shader::SCALAR, & textureWidth ).
		addUniform( "height", GLRenderer::Shader::INT, GLRenderer::Shader::SCALAR, & textureHeight );

	glr.program( "PROGRAM-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST" ).
		setVertexArray( "VERTICES-SOME-NICE-TEXTURE" ).
		setShader( "SHADER-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST" ).
			setFrameBuffer( "FRAMEBUFFER-SOME-NICE-TEXTURE" ).
			addOutTexture( "TEXTURE-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST" ).
			setFixSize( textureWidth, textureHeight ).
		build( );

	glr.vertices( "VERTICES-LIGHTS" ).
	setUsage( GL_DYNAMIC_DRAW ).
	addAttrib( "vertex", 3, 0 ).
	addAttrib( "color", 3, 3 ) <<

		.0f << .0f << .0f << lightColors[ 0 ] <<
		.0f << .0f << .0f << lightColors[ 1 ] <<
		.0f << .0f << .0f << lightColors[ 2 ] <<
		.0f << .0f << .0f << lightColors[ 3 ] <<
		.0f << .0f << .0f << lightColors[ 4 ] <<
		.0f << .0f << .0f << lightColors[ 5 ] <<
		.0f << .0f << .0f << lightColors[ 6 ] <<
		GLRenderer::VertexArray::Object( 0, 7, GL_POINTS );

	glr.shader(
		"SHADER-LIGHTS",

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
//			"fColor = vec4( 1., 1., 1., 1. );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
		addUniform( "model", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model ).
		addUniform( "view", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view ).
		addUniform( "projection", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection );

	glr.program( "PROGRAM-LIGHTS" ).
		setShader( "SHADER-LIGHTS" ).
		setVertexArray( "VERTICES-LIGHTS" ).
		build( );


	model = glm::mat4( 1. );
/*
*    2----3
*   /|   /|
*  6-+--7 |
*  | 0--+-1
*  |/   |/
*  4----5
*/

	glr.vertices( "VERTICES-CUBE" ).
	addAttrib( "vertex", 3, 0 ).	addAttrib( "color", 3, 3 ) <<

		p0 << p0 << p1 << p0 << p5 << px << p4 << px <<
		p6 << py << p2 << py <<
		p3 << pz << p1 << pz <<
		GLRenderer::VertexArray::Object( 0, 8, GL_TRIANGLE_FAN ) <<

		p7 << p0 << p3 << p0 << p2 << ( py + pz ) << p6 << ( py + pz ) <<
		p4 << ( px + py ) << p5 << ( px + py ) <<
		p1 << ( px + pz ) << p3 << ( px + pz ) <<
		GLRenderer::VertexArray::Object( 8, 8, GL_TRIANGLE_FAN );

	glr.shader(
		"SHADER-CUBE",
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
		GLRenderer::ShaderCode::FROM_CODE ).
		addUniform( "model", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model ).
		addUniform( "view", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view ).
		addUniform( "projection", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection ).
		addUniform( "lightPos[0]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightCameraSpace[ 0 ] ).
		addUniform( "lightPos[1]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightCameraSpace[ 1 ] ).
		addUniform( "lightPos[2]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightCameraSpace[ 2 ] ).
		addUniform( "lightPos[3]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightCameraSpace[ 3 ] ).
		addUniform( "lightPos[4]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightCameraSpace[ 4 ] ).
		addUniform( "lightPos[5]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightCameraSpace[ 5 ] ).
		addUniform( "lightPos[6]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightCameraSpace[ 6 ] ).
		addUniform( "lightCol[0]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightColors[ 0 ] ).
		addUniform( "lightCol[1]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightColors[ 1 ] ).
		addUniform( "lightCol[2]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightColors[ 2 ] ).
		addUniform( "lightCol[3]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightColors[ 3 ] ).
		addUniform( "lightCol[4]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightColors[ 4 ] ).
		addUniform( "lightCol[5]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightColors[ 5 ] ).
		addUniform( "lightCol[6]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightColors[ 6 ] ).
		addUniform( "time",        GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, & vcd->time );

	glr.program( "PROGRAM-CUBE" ).
		setShader( "SHADER-CUBE" ).
		setVertexArray( "VERTICES-CUBE" ).
		addInTexture( "TEXTURE-SOME-NICE-TEXTURE-CHECKERBOARD" ).
		addInTexture( "TEXTURE-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST" ).
		build( );
}

void
Cube::resize( int p_width, int p_height ) {

	float
	ratio = 1.f * p_width / p_height;

	projection = glm::perspective( 45.0f, ratio, 1.0f, 200.f );

//	glr.tx[ "TX1" ]->resize( p_width, p_height );
}

void
Cube::paint( ) {

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if( ! textureBuilt ) {

		glr.run( { "PROGRAM-SOME-NICE-TEXTURE-CHECKERBOARD" } );

		textureBuilt = true;
	}

	glr.run( { "PROGRAM-SOME-NICE-TEXTURE-ROBERTS-LIGHT-TEST" } );

	angle = vcd->time;

	model = glm::mat4( 1. );
	view  = glm::translate( glm::mat4( 1. ), glm::vec3( 0.f, 0.f, -50.f ) );
	view  = glm::rotate( view, .123f * angle, glm::vec3( sinf( .1f * angle ), sinf( .21f * angle ), sinf( .31f * angle ) ) );

	GLRenderer::VertexArray
	& va = glr.vertices( "VERTICES-LIGHTS" );

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

	glr.run( { "PROGRAM-LIGHTS" } );

	glDisable( GL_VERTEX_PROGRAM_POINT_SIZE );
	glDisable( GL_VERTEX_ATTRIB_ARRAY_NORMALIZED );

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
				model = glm::translate( glm::mat4( 1. ), ( 6.f - 4.f * f ) * glm::vec3( x, y, z ) );
				//model = glm::rotate( model, .221f * vcd->time, glm::vec3( 0.5f, 1.f, 0.25f ) );
				model = glm::rotate( model, ( x + y + z ) * 3.14f * ( 1.f - f ) * ( 1.f - f ) * ( 1.f - f ), glm::vec3( sinf( .1f * angle ), sinf( .21f * angle ), sinf( .31f * angle ) ) );
				glr.run( { "PROGRAM-CUBE" } );
			}
		}
	}
}
