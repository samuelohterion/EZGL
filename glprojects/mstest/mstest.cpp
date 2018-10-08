#include "mstest.hpp"
#include "../../code/glmprinter.hpp"

MSTest::MSTest( CStr const & p_name ) :
GLProject ( p_name  ) {

}

void
MSTest::init( ) {

	// frame buffers
	{
		// "F-MS-TEST"
		{
			glr.frameBuffer( "F-MS-TEST" );
		}
	}

	// textures
	{
		// T-MS-TEST-COL
		{
			glr.texture(
			"T-MS-TEST-COL",
			new GLR::Texture(
			"txCOL", GL_TEXTURE_2D, 0,
			GL_RGBA32F, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT, 32, 32 ) );
		}
		// T-MS-TEST-Z
		{
			glr.texture(
			"T-MS-TEST-Z",
			new GLR::Texture(
			"txZ", GL_TEXTURE_2D, 0,
			GL_DEPTH_COMPONENT32, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 32, 32 ) );
		}
	}

	// vertex arrays
	{
		// V-MS-TEST-QUAD-2D
		{
			glr.vertices( "V-MS-TEST-QUAD-2D" ).
			setUsage( GL_STATIC_DRAW ).
			attrib( "vertex", 0, 2 ) <<
			-1.f << -1.f <<
			+1.f << -1.f <<
			+1.f << +1.f <<
			-1.f << +1.f <<
			GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
		// V-MS-TEST-QUAD-3D
		{
			glr.vertices( "V-MS-TEST-QUAD-3D" ).
			setUsage( GL_STATIC_DRAW ).
			attrib( "vertex", 0, 3 ) <<
			+0.f << -1.f << +0.f <<
			+1.f << +0.f << +0.f <<
			+0.f << +0.f << +1.f <<
			-1.f << +0.f << +0.f <<
			+0.f << +0.f << -1.f <<
			+1.f << +0.f << +0.f <<
			GLR::VertexArray::Object( 0, 6, GL_TRIANGLE_FAN ) <<
			+0.f << +1.f << +0.f <<
			-1.f << +0.f << +0.f <<
			+0.f << +0.f << +1.f <<
			+1.f << +0.f << +0.f <<
			+0.f << +0.f << -1.f <<
			-1.f << +0.f << +0.f <<
			GLR::VertexArray::Object( 6, 6, GL_TRIANGLE_FAN );
		}
	}

	// index arrays
	{
	}

	// shaders
	{
		//S-MS-TEST-QUAD-2D-Z
		{
			glr.shader(
			"S-MS-TEST-QUAD-2D-Z",

			//Vertex Shader
			"#version 330 core\n"
			"layout( location = 0 ) in vec2 vertex;\n"
			"out VS2FS {\n"
			"	vec2 coord;\n"
			"} vs2fs;\n"
			"void main( void ) {\n"
			"vs2fs.coord = .5 + .5 * vertex;\n"
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
			"	fColor = vec4( z, z, z, 1 );\n"
			"}\n",
			GLR::ShaderCode::FROM_CODE );
		}
		//S-MS-TEST-QUAD-2D-COL
		{
			glr.shader(
			"S-MS-TEST-QUAD-2D-COL",

			//Vertex Shader
			"#version 330 core\n"
			"layout( location = 0 ) in vec2 vertex;\n"
			"out VS2FS {\n"
			"	vec2 coord;\n"
			"} vs2fs;\n"
			"void main( void ) {\n"
			"vs2fs.coord = .5 + .5 * vertex;\n"
			"gl_Position = vec4( vertex, 0, 1. );"
			"}\n",

			//Fragment Shader
			"#version 330 core\n"
			"uniform sampler2D txCOL;"
			"in VS2FS {\n"
			"	vec2 coord;\n"
			"} vs2fs;\n"
			"out vec4 fColor;\n"
			"void main( void ) {\n"
			"	fColor = texture( txCOL, vs2fs.coord );\n"
			"}\n",
			GLR::ShaderCode::FROM_CODE );
		}
		//S-MS-TEST-QUAD-2D-COL-Z
		{
			glr.shader(
			"S-MS-TEST-QUAD-2D-COL-Z",

			//Vertex Shader
			"#version 330 core\n"
			"layout( location = 0 ) in vec2 vertex;\n"
			"out VS2FS {\n"
			"	vec2 coord;\n"
			"} vs2fs;\n"
			"void main( void ) {\n"
			"vs2fs.coord = .5 + .5 * vertex;\n"
			"gl_Position = vec4( vertex, 0, 1. );"
			"}\n",

			//Fragment Shader
			"#version 330 core\n"
			"uniform sampler2D txZ;"
			"uniform sampler2D txCOL;"
			"uniform float time;"
			"in VS2FS {\n"
			"	vec2 coord;\n"
			"} vs2fs;\n"
			"out vec4 fColor;\n"
			"void main( void ) {\n"
			"	float z = texture( txZ, vs2fs.coord ).r;\n"
			"	vec4  c = texture( txCOL, vs2fs.coord );\n"
			"	fColor = vec4( mix( vec3( z ), c.rgb, vec3( .5 + .5 * sin( .1 * time ) ) ), 1 );\n"
			"}\n",
			GLR::ShaderCode::FROM_CODE ).
			addUniform ( "time", GLR::Shader::FLOAT, GLR::Shader::SCALAR, & vcd->time );
		}
		//S-MS-TEST-QUAD-3D-Z
		{
			glr.shader(
			"S-MS-TEST-QUAD-3D-Z",

			//Vertex Shader
			"#version 330 core\n"
			"layout( location = 0 ) in vec3 vertex;\n"
			"uniform mat4 model, view, projection;\n"
			"void main( void ) {\n"
			"	gl_Position = projection * view * model * vec4( vertex, 1. );\n"
			"}\n",

			//Fragment Shader
			"#version 330 core\n"
			"void main( void ) {\n"
			"}\n",
			GLR::ShaderCode::FROM_CODE ).
			addUniform( "model",      GLR::Shader::MAT4, GLR::Shader::SCALAR, & m ).
			addUniform( "view",       GLR::Shader::MAT4, GLR::Shader::SCALAR, & v ).
			addUniform( "projection", GLR::Shader::MAT4, GLR::Shader::SCALAR, & p );
		}
		//S-MS-TEST-QUAD-3D-COL-PASS1
		{
			glr.shader(
			"S-MS-TEST-QUAD-3D-COL-PASS1",

			//Vertex Shader
			"#version 330 core\n"
			"layout( location = 0 ) in vec3 vertex;\n"
			"uniform mat4 model, view, projection;\n"
			"out VS2GS {\n"
			"	vec4 v;\n"
			"} vs2gs;"
			"void main( void ) {\n"
			"	vs2gs.v = model * vec4( vertex, 1 );\n"
			"	gl_Position = projection * view * vs2gs.v;\n"
			"}\n",

			//Geometry Shader
			"#version 330 core\n"
			"layout ( triangles ) in;\n"
			"layout ( triangle_strip, max_vertices = 3 ) out;\n"

			//	light in every space that makes sense to show their behavior
			"in VS2GS {\n"
			"	vec4 v;\n"
			"} vs2gs[ ];\n"
			"out GS2FS {\n"
			"	vec4 v, n;\n"
			"} gs2fs;\n"
			"void main( ) {\n"
			"	vec3 n = normalize( cross( vs2gs[ 1 ].v.xyz - vs2gs[ 0 ].v.xyz, vs2gs[ 2 ].v.xyz - vs2gs[ 0 ].v.xyz ) );\n"
			"	gl_Position = gl_in[ 0 ].gl_Position;\n"
			"	gs2fs.v = vs2gs[ 0 ].v;\n"
			"	gs2fs.n = vec4( n, 1 );\n"
			"	EmitVertex( );\n"
			"	gl_Position = gl_in[ 1 ].gl_Position;\n"
			"	gs2fs.v = vs2gs[ 1 ].v;\n"
			"	gs2fs.n = vec4( n, 1 );\n"
			"	EmitVertex( );\n"
			"	gl_Position = gl_in[ 2 ].gl_Position;\n"
			"	gs2fs.v = vs2gs[ 2 ].v;\n"
			"	gs2fs.n = vec4( n, 1 );\n"
			"	EmitVertex( );\n"
			"	EndPrimitive( );\n"
			"}\n",

			//Fragment Shader
			"#version 330 core\n"
			"in GS2FS {\n"
			"	vec4 v, n;\n"
			"} gs2fs;\n"
			"uniform mat4 model, view;\n"
			"uniform vec4 color;\n"
			"out vec4 fColor;\n"
			"const vec3 light = vec3( 0, 0, 0 );\n"
			"void main( void ) {\n"
			"	if ( color.a < 1. )\n"
			"		discard;\n"
			"	vec3 d = normalize( vec3( view * model * vec4( light, 1 )- gs2fs.v ) );\n"
			"	float amp = max( 0, -dot( gs2fs.n.xyz, d ) );\n"
			"	fColor = vec4( amp * color.rgb, 1 );\n"
			"}\n",
			GLR::ShaderCode::FROM_CODE ).
			addUniform( "model",      GLR::Shader::MAT4, GLR::Shader::SCALAR, & m ).
			addUniform( "view",       GLR::Shader::MAT4, GLR::Shader::SCALAR, & v ).
			addUniform( "projection", GLR::Shader::MAT4, GLR::Shader::SCALAR, & p ).
			addUniform( "color",      GLR::Shader::VEC4, GLR::Shader::SCALAR, & color );
		}
		//S-MS-TEST-QUAD-3D-COL-PASS2
		{
			glr.shader(
			"S-MS-TEST-QUAD-3D-COL-PASS2",

			//Vertex Shader
			"#version 330 core\n"
			"layout( location = 0 ) in vec3 vertex;\n"
			"uniform mat4 model, view, projection;\n"
			"out VS2GS {\n"
			"	vec4 v;\n"
			"	vec2 t;\n"
			"} vs2gs;"
			"void main( void ) {\n"
			"	vs2gs.v = model * vec4( vertex, 1 );\n"
			"	vec4  r = projection * view * vs2gs.v;\n"
			"	vs2gs.t = .5 + .5 * r.xy / r.a;\n"
			"	gl_Position = r;\n"
			"}\n",

			//Geometry Shader
			"#version 330 core\n"
			"layout ( triangles ) in;\n"
			"layout ( triangle_strip, max_vertices = 3 ) out;\n"

			//	light in every space that makes sense to show their behavior
			"in VS2GS {\n"
			"	vec4 v;\n"
			"	vec2 t;\n"
			"} vs2gs[ ];\n"
			"out GS2FS {\n"
			"	vec4 v, n;\n"
			"	vec2 t;\n"
			"} gs2fs;\n"
			"void main( ) {\n"
			"	vec3 n = normalize( cross( vs2gs[ 1 ].v.xyz - vs2gs[ 0 ].v.xyz, vs2gs[ 2 ].v.xyz - vs2gs[ 0 ].v.xyz ) );\n"
			"	gl_Position = gl_in[ 0 ].gl_Position;\n"
			"	gs2fs.t = vs2gs[ 0 ].t;\n"
			"	gs2fs.v = vs2gs[ 0 ].v;\n"
			"	gs2fs.n = vec4( n, 1 );\n"
			"	EmitVertex( );\n"
			"	gl_Position = gl_in[ 1 ].gl_Position;\n"
			"	gs2fs.t = vs2gs[ 1 ].t;\n"
			"	gs2fs.v = vs2gs[ 1 ].v;\n"
			"	gs2fs.n = vec4( n, 1 );\n"
			"	EmitVertex( );\n"
			"	gl_Position = gl_in[ 2 ].gl_Position;\n"
			"	gs2fs.t = vs2gs[ 2 ].t;\n"
			"	gs2fs.v = vs2gs[ 2 ].v;\n"
			"	gs2fs.n = vec4( n, 1 );\n"
			"	EmitVertex( );\n"
			"	EndPrimitive( );\n"
			"}\n",

			//Fragment Shader
			"#version 330 core\n"
			"in GS2FS {\n"
			"	vec4 v, n;\n"
			"	vec2 t;\n"
			"} gs2fs;\n"
			"uniform mat4 model, view;\n"
			"uniform vec4 color;\n"
			"uniform sampler2D txCOL;\n"
			"out vec4 fColor;\n"
			"const vec3 light = vec3( 0, 0, 0 );\n"
			"void main( void ) {\n"
			"	vec3 d = normalize( vec3(  view * model * vec4( light, 1 ) - gs2fs.v ) );\n"
			"	float amp = max( 0, -dot( gs2fs.n.xyz, d ) );\n"
			"	fColor = texture( txCOL, gs2fs.t );\n"
			"	if( 1. <= color.a )\n"
			"		fColor = texture( txCOL, gs2fs.t );\n"
			"	else"
			"		fColor = vec4( amp * mix( texture( txCOL, gs2fs.t ).rgb, color.rgb, color.a ), 1 );\n"
			"}\n",
			GLR::ShaderCode::FROM_CODE ).
			addUniform( "model",      GLR::Shader::MAT4, GLR::Shader::SCALAR, & m ).
			addUniform( "view",       GLR::Shader::MAT4, GLR::Shader::SCALAR, & v ).
			addUniform( "projection", GLR::Shader::MAT4, GLR::Shader::SCALAR, & p ).
			addUniform( "color",      GLR::Shader::VEC4, GLR::Shader::SCALAR, & color );
		}
	}

	// container
	{
		// C-MS-TEST-QUAD-3D-PASS1
		{
			glr.container( "C-MS-TEST-QUAD-3D-PASS1" ).
			setFrameBuffer( "F-MS-TEST" ).
			addOutTexture( "T-MS-TEST-Z" ).
			addOutTexture( "T-MS-TEST-COL" ).
			setVertexArray( "V-MS-TEST-QUAD-3D" ).
			setShader( "S-MS-TEST-QUAD-3D-COL-PASS1" ).
			build( );
		}
		// C-MS-TEST-QUAD-3D-PASS2
		{
			glr.container( "C-MS-TEST-QUAD-3D-PASS2" ).
			addInTexture( "T-MS-TEST-COL" ).
			setVertexArray( "V-MS-TEST-QUAD-3D" ).
			setShader( "S-MS-TEST-QUAD-3D-COL-PASS2" ).
			build( );
		}
		// C-MS-TEST-QUAD-2D-Z-COL
		{
			glr.container( "C-MS-TEST-QUAD-2D-COL-Z" ).
			addInTexture( "T-MS-TEST-Z" ).
			addInTexture( "T-MS-TEST-COL" ).
			setVertexArray( "V-MS-TEST-QUAD-2D" ).
			setShader( "S-MS-TEST-QUAD-2D-COL-Z" ).
			build( );
		}
		// C-MS-TEST-QUAD-2D-COL
		{
			glr.container( "C-MS-TEST-QUAD-2D-COL" ).
			addInTexture( "T-MS-TEST-COL" ).
			setVertexArray( "V-MS-TEST-QUAD-2D" ).
			setShader( "S-MS-TEST-QUAD-2D-COL" ).
			build( );
		}
	}

	p = v = m = glm::mat4( 1. );

	v = glm::translate( v, glm::vec3( 0.f, 0.f, -2.f ) );

	ccv.fixVCD ( vcd );

	glClearColor( 0.2f, 0.3f, 0.4f, 1.0f );

	glFrontFace( GL_CCW );
}

void
MSTest::paint( ) {

	ccv.fixView ( v );
	ccv.fixModel( m );

	ccv.reactOnMouse( );

	m = ccv.model( );

	v  = ccv.view( );

	M4
	t = m;

	glClearColor( .0f, .0f, .0f, 1.f );
	glEnable ( GL_DEPTH_TEST );
	glDisable ( GL_CULL_FACE );
	glCullFace( GL_BACK );

//	glDisable( GL_CULL_FACE );
//	glEnable( GL_DEPTH_TEST );

	glr.container( "C-MS-TEST-QUAD-3D-PASS1" ).
		addClearBits( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	m = glm::translate( m, V3( -5, 0, 0 ) );
	color = glm::vec4( 1, 0, 0, 1 );
	glr.run( { "C-MS-TEST-QUAD-3D-PASS1" } );

	glr.
	container( "C-MS-TEST-QUAD-3D-PASS1" ).
		subClearBits( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	m = glm::translate( m, V3( +2, 0, 0 ) );
	color = glm::vec4( 1, 1, 0, .2 );
	glr.run( { "C-MS-TEST-QUAD-3D-PASS1" } );

	m = glm::translate( m, V3( +2, 0, 0 ) );
	color = glm::vec4( 0, 1, 0, 1 );
	glr.run( { "C-MS-TEST-QUAD-3D-PASS1" } );

	m = glm::translate( m, V3( +2, 0, 0 ) );
	color = glm::vec4( 0, 1, 1, .2 );
	glr.run( { "C-MS-TEST-QUAD-3D-PASS1" } );

	m = glm::translate( m, V3( +2, 0, 0 ) );
	color = glm::vec4( 0, 0, 1, 1. );
	glr.run( { "C-MS-TEST-QUAD-3D-PASS1" } );

	m = glm::translate( m, V3( +2, 0, 0 ) );
	color = glm::vec4( 1, 0, 1, .2 );
	glr.run( { "C-MS-TEST-QUAD-3D-PASS1" } );


	// PASS2
	glEnable ( GL_DEPTH_TEST );
	glDisable ( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glr.container( "C-MS-TEST-QUAD-3D-PASS2" ).
		addClearBits( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	m = glm::translate( t, V3( -5, 0, 0 ) );
	color = glm::vec4( 1, 0, 0, 1 );
	glr.run( { "C-MS-TEST-QUAD-3D-PASS2" } );

	glr.
	container( "C-MS-TEST-QUAD-3D-PASS2" ).
		subClearBits( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	m = glm::translate( m, V3( +2, 0, 0 ) );
	color = glm::vec4( 1, 1, 0, .2 );
	glr.run( { "C-MS-TEST-QUAD-3D-PASS2" } );

	m = glm::translate( m, V3( +2, 0, 0 ) );
	color = glm::vec4( 0, 1, 0, 1 );
	glr.run( { "C-MS-TEST-QUAD-3D-PASS2" } );

	m = glm::translate( m, V3( +2, 0, 0 ) );
	color = glm::vec4( 0, 1, 1, .5 );
	glr.run( { "C-MS-TEST-QUAD-3D-PASS2" } );

	m = glm::translate( m, V3( +2, 0, 0 ) );
	color = glm::vec4( 0, 0, 1, 1. );
	glr.run( { "C-MS-TEST-QUAD-3D-PASS2" } );

	m = glm::translate( m, V3( +2, 0, 0 ) );
	color = glm::vec4( 1, 0, 1, .2 );
	glr.run( { "C-MS-TEST-QUAD-3D-PASS2" } );


//	glDisable( GL_DEPTH_TEST );
//	glr.run( { "C-MS-TEST-QUAD-2D-COL" } );

	M4
	tr = p * v * m;
	V4
	r = V4( 0,0,-1,1);

	r = tr * r;
	r /= r.a;
	print( r );
	std::cout << std::endl;

	m = t;
}

void
MSTest::resize( int p_width, int p_height ) {

	float
	w = p_width,
	h = p_height,
	ratio = w / h;

	p = glm::perspective(  45.0f, ratio, 1.0f, 100.f );

	std::cout << std::endl;
}
