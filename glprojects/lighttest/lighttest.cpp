#include "lighttest.hpp"

LightTest::LightTest( const CStr &p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
LightTest::init( ) {

	glClearColor( .1f, .02f, .03f, 1. );

	m = glm::mat4( 1. );

	glr.vertices( "QUAD-VERTICES" ).
		setUsage( GL_DYNAMIC_DRAW ).
		addAttrib( "vertices", 3, 0 ).	addAttrib( "normals", 3, 3 ).	addAttrib( "colors", 3, 6 ) <<
		glm::vec3( -1.f, -1.f, 0.f ) <<	glm::vec3( 0.f, 0.f, 1.f ) <<	glm::vec3( 1.f, 0.f, 0.f ) <<
		glm::vec3( +1.f, -1.f, 0.f ) <<	glm::vec3( 0.f, 0.f, 1.f ) <<	glm::vec3( 0.f, 1.f, 0.f ) <<
		glm::vec3( -1.f, +1.f, 0.f ) <<	glm::vec3( 0.f, 0.f, 1.f ) <<	glm::vec3( 0.f, 0.f, 1.f ) <<
		glm::vec3( +1.f, +1.f, 0.f ) <<	glm::vec3( 0.f, 0.f, 1.f ) <<	glm::vec3( 1.f, 1.f, 0.f ) <<
		GLRenderer::VertexArray::Object( 0, 4, GL_TRIANGLE_STRIP );

	glr.shader(
		"QUAD-SHADER",

		"#version 330 core\n\
		precision highp float;\n\
		precision highp int;\n\
		in vec3 vertices;\n\
		in vec3 normals;\n\
		in vec3 colors;\n\
		uniform mat4 p;\n\
		uniform mat4 mv;\n\
		uniform vec3 light;\n\
		out vec4 vVerts, vNorms, vColors, vLight;\n\
		void main( void ) {\n\
			vVerts = p * mv * vec4( vertices, 1. );\n\
			mat3 norm = mat3( transpose( inverse( mv ) ) );\n\
			vNorms  = vec4( normalize( vec3( p * vec4( norm * normals, 0. ) ) ), 1. );\n\
			vColors = vec4( colors, 1. );\n\
			vLight = p * mv * vec4( light, 1. );\n\
			gl_Position = vVerts;\n\
		}\n",

		"#version 330 core\n\
		precision highp float;\n\
		precision highp int;\n\
		//uniform float time;\n\
		in vec4 vVerts, vNorms, vColors, vLight;\n\
		out vec4 fCol;\n\
		void main( void ) {\n\
			vec3 diff = ( vLight - vVerts ).xyz;\n\
			float a = clamp( 1. * max( 0., dot( vNorms.xyz, normalize( diff ) ) ) / pow( dot( diff, diff ), 1. ), 0. ,1. );\n\
			fCol = vec4( ( .8 * a + .2 ) * vColors.xyz, 1. );\n\
//			fCol = vec4( ( .8 * a + .2 ) * vec3( 1. ), 1. );\n\
		}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "p",  GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p ).
			addUniform( "mv",  GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv ).
			addUniform( "light", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light );

	glr.program( "QUAD-PROGRAM" ).
		setVertexArray( "QUAD-VERTICES" ).
		setShader( "QUAD-SHADER" ).
		build( );

	glr.shader(
		"NORMALS-SHADER",

		"#version 330 core\n"
		"precision highp float;\n"
		"precision highp int;\n"
		"in vec3 vertices;\n"
		"in vec3 normals;\n"
		"in vec3 colors;\n"
		"out VS2GS {\n"
			"vec3 vertices, normals, colors;\n"
		"} vs2gsOut;\n"
		"uniform mat4 p;\n"
		"uniform mat4 mv;\n"
		"void main( ) {\n"
			"vs2gsOut.vertices   = ( p * mv * vec4( vertices, 1. ) ).xyz;\n"
			"vs2gsOut.normals = normalize( vec3( p * vec4( mat3( transpose( inverse( mv ) ) ) * normals, 0. ) ) );\n"
			"vs2gsOut.colors  = colors;\n"
			"gl_Position = p * mv * vec4( vertices, 1. );\n"
		"}\n",

		"#version 330 core\n"
		"precision highp float;\n"
		"precision highp int;\n"
		"layout ( triangles ) in;\n"
		"layout ( line_strip, max_vertices = 6 ) out;\n"
		"in VS2GS {\n"
			"vec3 vertices;\n"
			"vec3 normals;\n"
			"vec3 colors;\n"
		"} vs2gsIn[ ];\n"
		"out GS2FS {\n"
			"vec3 colors;\n"
		"} gs2fsOut;\n"
		"const float MAGNITUDE = 1.;\n"
		"void generateLine( int index ) {\n"
			"gl_Position = gl_in[ index ].gl_Position;\n"
			"gs2fsOut.colors = max( vec3( 0. ), vs2gsIn[ index ].normals );\n"
			"EmitVertex( );\n"
			"gl_Position = gl_in[ index ].gl_Position + vec4( vs2gsIn[ index ].normals, 0. ) * MAGNITUDE;\n"
			"gs2fsOut.colors = max( vec3( 0. ), vs2gsIn[ index ].normals );\n"
			"EmitVertex( );\n"
			"EndPrimitive( );\n"
		"}\n"
		"void main( ) {\n"
			"generateLine( 0 ); // first vertex normal\n"
			"generateLine( 1 ); // second vertex normal\n"
			"generateLine( 2 ); // third vertex normal\n"
		"}\n",

		"#version 330 core\n"
		"precision highp float;\n"
		"precision highp int;\n"
		"in GS2FS {\n"
			"vec3 colors;\n"
		"} gs2fsIn;\n"
		"out vec4 fCol;\n"
		"void main( void ) {\n"
			"fCol = vec4( gs2fsIn.colors, 1.f );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "p",  GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p ).
			addUniform( "mv",  GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv );

	glr.program( "NORMALS-PROGRAM" ).
		setVertexArray(	"QUAD-VERTICES" ).
		setShader( "NORMALS-SHADER" ).
		build( );
	}

void
LightTest::paint( ) {

	float
	angle = .31f * vcd->time;

	//light = glm::vec3( cos( 10. * angle ), 0., 1.5 + sin( 10. * angle ) );
	light = glm::vec3( 0.f, 0.f, -2.f + sinf( 10.f * angle ) );

	v = glm::rotate(
			glm::translate(
				glm::mat4( 1. ),
				glm::vec3( 0.f, 0.f, -4.f + sinf( 2.2f * angle ) ) ),
			angle,
			glm::vec3( sin( .3 * angle ), 1.f, 0.f ) );

	mv = v * m;

	light = glm::vec3( inverse( mv ) * glm::vec4( light, 1. ) );

	glr.vertices( "QUAD-VERTICES" ).arr[  0 ] = -1.f + powf( sinf( 5.f * angle ), 4.f );
	glr.vertices( "QUAD-VERTICES" ).arr[  9 ] = +1.f + powf( sinf( 1.6f + 5.f * angle ), 4.f );
	glr.vertices( "QUAD-VERTICES" ).arr[ 18 ] = -1.f + powf( sinf( 4.8f + 5.f * angle ), 4.f );
	glr.vertices( "QUAD-VERTICES" ).arr[ 27 ] = +1.f + powf( sinf( 3.2f + 5.f * angle ), 4.f );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

//	glr.run( { "QUAD-PROGRAM" } );

//	glr.run( { "NORMALS-PROGRAM" } );
	glr.run( { "QUAD-PROGRAM", "NORMALS-PROGRAM" } );
}

void
LightTest::resize( int p_width, int p_height ) {

	float
	ratio = ( 1.f * p_width / p_height );

	p = glm::perspective( 45.0f, ratio, 1.0f, 100.f );
}
