#include "cube.hpp"

Cube::Cube( CStr const & p_name, ViewControlData * p_vcd ) :
GLProject( p_name, p_vcd ) {

}

void
Cube::init( ) {

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClearColor( 0., 0., 0., 1. );

	angle = vcd->time;

	light = glm::vec3( 0.f, 0.f, -3.f );

	model = glm::mat4( 1. );
/*
*    2----3
*   /|   /|
*  6-+--7 |
*  | 0--+-1
*  |/   |/
*  4----5
*/

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

	glr.vertices( "VERTICES-CUBE" ).
	addAttrib( "vertex", 3, 0 ) <<
	//
		p0 << p1 << p5 << p4 << p6 << p2 << p3 << p1 <<
		GLRenderer::VertexArray::Object( 0, 8, GL_TRIANGLE_FAN ) <<

		p7 << p3 << p2 << p6 << p4 << p5 << p1 << p3 <<
		GLRenderer::VertexArray::Object( 8, 8, GL_TRIANGLE_FAN );

	glr.shader(
		"SHADER-CUBE",
		// vertex shader
		"#version 330 core\n"
		"in vec3 vertex;\n"
		"out VS2GS {\n"
			"vec3 vertex;\n"
		"} vs2gs;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main( ) {\n"
			"vec4\n"
			"v = view * model * vec4( vertex, 1. );\n"
			"vs2gs.vertex = v.xyz;\n"
			"gl_Position = projection * v;\n"
		"}\n",
		// geometry shader
		"#version 330 core\n"
		"layout ( triangles ) in;\n"
		"layout ( triangle_strip, max_vertices = 3 ) out;\n"
		"in VS2GS {\n"
			"vec3 vertex;\n"
		"} vs2gs[ ];\n"
		"out GS2FS {\n"
			"vec3 vertex, normal;\n"
		"} gs2fs;\n"
		"void main( ) {\n"
			"vec3 n = normalize( cross( vs2gs[ 1 ].vertex - vs2gs[ 0 ].vertex, vs2gs[ 2 ].vertex - vs2gs[ 0 ].vertex ) );\n"

			"gl_Position = gl_in[ 0 ].gl_Position;\n"
			"gs2fs.vertex = vs2gs[ 0 ].vertex;\n"
			"gs2fs.normal = n;\n"
			"EmitVertex( );\n"

			"gl_Position = gl_in[ 1 ].gl_Position;\n"
			"gs2fs.vertex = vs2gs[ 1 ].vertex;\n"
			"gs2fs.normal = n;\n"
			"EmitVertex( );\n"

			"gl_Position = gl_in[ 2 ].gl_Position;\n"
			"gs2fs.vertex = vs2gs[ 2 ].vertex;\n"
			"gs2fs.normal = n;\n"
			"EmitVertex( );\n"

			"EndPrimitive( );\n"
		"}\n",
		// fragment shader
		"#version 330 core\n"
		"in GS2FS {\n"
			"vec3\n"
			"vertex,\n"
			"normal;\n"
		"} gs2fs;\n"
		"uniform mat4 model;\n"
		"uniform vec3 light;\n"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
			"vec3 d = ( model * vec4( light, 1. ) ).xyz - gs2fs.vertex;\n"
			"float a = 1. * normalize( dot( gs2fs.normal, d ) );\n"
			"fColor = vec4( clamp( pow( a / dot( d, d ), 2. ), .1, 1. ) * vec3( .321, .654, .987 ), 1.f );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
		addUniform( "model", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model ).
		addUniform( "view", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view ).
		addUniform( "projection", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection ).
		addUniform( "light", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light );

	glr.program( "PROGRAM-CUBE-1" ).setShader( "SHADER-CUBE" );
	glr.program( "PROGRAM-CUBE-1" ).setVertexArray( "VERTICES-CUBE" );
	glr.program( "PROGRAM-CUBE-1" ).build( );
}

void
Cube::resize( int p_width, int p_height ) {

	float
	ratio = 1.f * p_width / p_height;

	projection = glm::perspective( 45.0f, ratio, 1.0f, 100.f );
}

void
Cube::paint( ) {

	angle = 3.15f * vcd->time;

	view = glm::translate( glm::mat4( 1. ), glm::vec3( 0.f, 0.f, -5.f ) );
	view = glm::rotate( view, angle, glm::vec3( sin( .1 * angle ), sin( .12 * angle ), sin( .13 * angle ) ) );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glr.run( { "PROGRAM-CUBE-1" } );
}
