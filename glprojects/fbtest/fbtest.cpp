#include "fbtest.hpp"
#include "../../code/glmprinter.hpp"

FBTest::FBTest( CStr const & p_name ) :
GLProject ( p_name  ) {

}

void
FBTest::init( ) {

	// textures
	{
		// T-MS-TEST-COL
		{
			glr.texture(
				"T-FB-TEST-CANVAS-COL",
				new GLR::Texture(
					"txCOL", GL_TEXTURE_2D, 0, GL_RGBA32F,
					GL_NEAREST, GL_NEAREST,
					GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
					GL_RGBA, GL_FLOAT, 320, 200 ) );
		}
		// T-FB-TEST-CANVAS-Z
		{
			glr.texture(
				"T-FB-TEST-CANVAS-Z",
				new GLR::Texture(
					"txZ", GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
					GL_NEAREST, GL_NEAREST,
					GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
					GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 320, 200 ) );
		}
	}

	// vertex arrays
	{
		// V-FB-TEST-CANVAS
		{
			glr.vertices( "V-FB-TEST-CANVAS" ).
			attrib( "vertex", 0, 2 ) <<
			-1.f << -1.f <<
			+1.f << -1.f <<
			+1.f << +1.f <<
			-1.f << +1.f <<
			GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
		// V-FB-TEST-QUAD-3D
		{
			glr.vertices( "V-FB-TEST-QUAD-3D" ).
			attrib( "vertex", 0, 3 ) <<
			-1.f << -1.f << +0.f <<
			+1.f << -1.f << +0.f <<
			+1.f << +1.f << +0.f <<
			-1.f << +1.f << +0.f <<
			GLR::VertexArray::Object( 0, 4, GL_TRIANGLE_FAN );
		}
		// V-FB-TEST-TETRAHEDRON
		{
			glr.vertices( "V-FB-TEST-TETRAHEDRON" ).
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
		//S-FB-TEST-CANVAS
		{
			glr.shader(
			"S-FB-TEST-CANVAS",

			//Vertex Shader
			"#version 330 core\n"
			"layout( location = 0 ) in vec2 vertex;\n"
			"out VS2FS {\n"
			"   vec2 texCoords;\n"
			"} vs2fs;\n"
			"void main( void ) {\n"
			"   vs2fs.texCoords = vertex * .5 + .5;\n"
			"	gl_Position = vec4( vertex, 0, 1 );\n"
			"}\n",

			//Fragment Shader
			"#version 330 core\n"
			"in VS2FS {\n"
			"   vec2 texCoords;\n"
			"} vs2fs;\n"
			"uniform sampler2D txCOL;\n"
			"uniform sampler2D txZ;\n"
			"uniform float time;\n"
			"out vec4 fColor;\n"
			"void main( void ) {\n"
			"	vec4"
			"	s1 = texture( txCOL, vs2fs.texCoords );\n"
			"	float s2 = texture( txZ, vs2fs.texCoords ).r;\n"
			"	fColor = vec4( s2, s2, s2, 1 );\n"
			"	fColor = mix( s1, fColor, vec4( vs2fs.texCoords.x > 0.5 ? 1 : 0 ) );\n"
			"}\n",
			GLR::ShaderCode::FROM_CODE ).
			addUniform( "time", GLR::Shader::FLOAT, GLR::Shader::SCALAR, & vcd->time );
		}
		//S-FB-TEST-QUAD-3D
		{
			glr.shader(
			"S-FB-TEST-QUAD-3D",

			//Vertex Shader
			"#version 330 core\n"
			"layout( location = 0 ) in vec3 vertex;\n"
			"uniform mat4 model, view, projection;\n"
			"void main( void ) {\n"
			"	gl_Position = projection * view * model * vec4( vertex, 1 );\n"
			"}\n",

			//Fragment Shader
			"#version 330 core\n"
			"uniform vec4 color;\n"
			"out vec4 fColor;\n"
			"void main( void ) {\n"
			"	fColor = color;\n"
			"}\n",
			GLR::ShaderCode::FROM_CODE ).
			addUniform( "model",      GLR::Shader::MAT4, GLR::Shader::SCALAR, & m ).
			addUniform( "view",       GLR::Shader::MAT4, GLR::Shader::SCALAR, & v ).
			addUniform( "projection", GLR::Shader::MAT4, GLR::Shader::SCALAR, & p ).
			addUniform( "color",      GLR::Shader::VEC4, GLR::Shader::SCALAR, & color );
		}
		//S-FB-TEST-TETRAHEDRON
		{
			glr.shader(
			"S-FB-TEST-TETRAHEDRON",

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
			"	vec3 d = normalize( vec3( view * model * vec4( light, 1 ) - gs2fs.v ) );\n"
			"	float amp = max( 0, - dot( gs2fs.n.xyz, d ) );\n"
			"	fColor = vec4( amp * color.rgb, 1 );\n"
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
		// C-FB-TEST-CANVAS
		{
			glr.container ( "C-FB-TEST-CANVAS" ).
				addInTexture( "T-FB-TEST-CANVAS-COL" ).
				addInTexture( "T-FB-TEST-CANVAS-Z" ).
				setVertexArray ( "V-FB-TEST-CANVAS" ).
				setShader ( "S-FB-TEST-CANVAS" ).
				build ( );
		}
		// C-FB-TEST-QUAD-3D
		{
			glr.container ( "C-FB-TEST-QUAD-3D" ).
				setVertexArray ( "V-FB-TEST-QUAD-3D" ).
				setShader ( "S-FB-TEST-QUAD-3D" ).
				build ( );
		}
		// C-FB-TEST-TETRAHEDRON
		{
			glr.container ( "C-FB-TEST-TETRAHEDRON" ).
				setVertexArray ( "V-FB-TEST-TETRAHEDRON" ).
				setShader ( "S-FB-TEST-TETRAHEDRON" ).
				build ( );
		}
	}

	p = v = m = glm::mat4( 1. );

	v = glm::translate( v, glm::vec3( 0.f, 0.f, -2.f ) );

	ccv.fixVCD ( vcd );

	glFrontFace( GL_CCW );
	glClearColor( 1.f, 1.f, 1.f, 1.0f );

	glEnable ( GL_DEPTH_TEST );
	glDisable ( GL_CULL_FACE );
	glCullFace( GL_BACK );

	glr.createOffScreen ( );
	glr.fb->addOutTexture( "T-FB-TEST-CANVAS-Z" );
	glr.fb->addOutTexture( "T-FB-TEST-CANVAS-COL" );
	glr.fb->fixedSize = true;
}

void
FBTest::paint( ) {


	ccv.fixView ( v );
	ccv.fixModel( m );

	ccv.reactOnMouse( );

	m = ccv.model( );

	v  = ccv.view( );

	M4
	t = m;

	glr.screenoff ( );
	glViewport ( 0, 0, 320, 200 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	color = V4 ( .3 ,.2 ,.1 , 1. );
	glr.run( { "C-FB-TEST-QUAD-3D" } );

	color = V4 ( 1, .5, .25, 1 );
	glr.run( { "C-FB-TEST-TETRAHEDRON" } );

	glr.screenon ( );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glViewport ( 0, 0, vcd->width, vcd->height );

	glr.run ( { "C-FB-TEST-CANVAS" } );

	m = t;
}

void
FBTest::resize( int p_width, int p_height ) {

	float
	w = p_width,
	h = p_height,
	ratio = w / h;

	p = glm::perspective(  45.0f, ratio, 1.0f, 100.f );

	std::cout << std::endl;

	if ( glr.fb ) {

		glr.fb->resize( p_width, p_height );
	}

	glViewport ( 0, 0, p_width, p_height );
}
