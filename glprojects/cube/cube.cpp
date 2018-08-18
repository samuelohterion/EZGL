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

	glr.vertices( "VERTICES-LIGHTS" ).
	setUsage( GL_DYNAMIC_DRAW ).
	addAttrib( "vertex", 3, 0 ).
	addAttrib( "color", 3, 3 ) <<

		0.f   <<  0.f  << 0.f   <<
		.993f << .992f << .998f <<
		GLRenderer::VertexArray::Object( 0, 1, GL_POINTS );

	glr.vertices( "VERTICES-CUBE" ).
	addAttrib( "vertex", 3, 0 ).	addAttrib( "color", 3, 3 ) <<

		p0 << px << p1 << px << p5 << px << p4 << px <<
		p6 << py << p2 << py <<
		p3 << pz << p1 << pz <<
		GLRenderer::VertexArray::Object( 0, 8, GL_TRIANGLE_FAN ) <<

		p7 << ( py + pz ) << p3 << ( py + pz ) << p2 << ( py + pz ) << p6 << ( py + pz ) <<
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
			"vec3 vertex, color;\n"
		"} vs2gs;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main( ) {\n"
			"vec4\n"
			"v = view * model * vec4( vertex, 1. );\n"
			"vs2gs.vertex = v.xyz;\n"
			"vs2gs.color  = color;\n"
			"gl_Position = projection * v;\n"
		"}\n",
		// geometry shader
		"#version 330 core\n"
		"layout ( triangles ) in;\n"
		"layout ( triangle_strip, max_vertices = 3 ) out;\n"
		"in VS2GS {\n"
			"vec3 vertex, color;\n"
		"} vs2gs[ ];\n"
		"out GS2FS {\n"
			"vec3 vertex, color, normal;\n"
		"} gs2fs;\n"
		"void main( ) {\n"
			"vec3 n = normalize( cross( vs2gs[ 1 ].vertex - vs2gs[ 0 ].vertex, vs2gs[ 2 ].vertex - vs2gs[ 0 ].vertex ) );\n"

			"gl_Position = gl_in[ 0 ].gl_Position;\n"
			"gs2fs.vertex = vs2gs[ 0 ].vertex;\n"
			"gs2fs.normal = n;\n"
			"gs2fs.color  = vs2gs[ 2 ].color;\n"
			"EmitVertex( );\n"

			"gl_Position = gl_in[ 1 ].gl_Position;\n"
			"gs2fs.vertex = vs2gs[ 1 ].vertex;\n"
			"gs2fs.normal = n;\n"
			"gs2fs.color  = vs2gs[ 2 ].color;\n"
			"EmitVertex( );\n"

			"gl_Position = gl_in[ 2 ].gl_Position;\n"
			"gs2fs.vertex = vs2gs[ 2 ].vertex;\n"
			"gs2fs.normal = n;\n"
			"gs2fs.color  = vs2gs[ 2 ].color;\n"
			"EmitVertex( );\n"

			"EndPrimitive( );\n"
		"}\n",
		// fragment shader
		"#version 330 core\n"
		"in GS2FS {\n"
			"vec3\n"
			"vertex,\n"
			"color,\n"
			"normal;\n"
		"} gs2fs;\n"
		"uniform mat4 model;\n"
		"uniform vec3 light;\n"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
			"vec3 d = ( vec4( light, 1. ) ).xyz - gs2fs.vertex;\n"
			"float a = 2. * normalize( dot( gs2fs.normal, d ) );\n"
			"fColor = vec4( ( clamp( 5. * a / dot( d, d ), .1, 1 ) ) * ( .7 + .3 * gs2fs.color ), 1.f );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
		addUniform( "model", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model ).
		addUniform( "view", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view ).
		addUniform( "projection", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection ).
		addUniform( "light", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light );

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
		"uniform vec3 light;\n"

		"void main( ) {\n"
			"vec4 v = projection * view * model * vec4( vertex, 1 );\n"
			"vs2fs.vertex = ( projection * view * model * vec4( light, 1. ) ).xyz;\n"
			"vs2fs.color  = color;\n"
			"gl_PointSize = 90.- .5 * v.z;\n"
			"gl_Position  = v;\n"
		"}\n",

		// fragment shader
		"#version 330 core\n"
		"in VS2FS {\n"
			"vec3 vertex, color;\n"
		"} vs2fs;\n"
		"uniform vec3 light;\n"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
			"vec2 v = -1. + 2. * gl_PointCoord.xy;"
			"float s = dot( v, v );"
			"if( s > .999 ) discard;"
			"fColor = clamp( ( 1. - 1. * sqrt( .01 * s ) ), .1, 1. ) * vec4( vs2fs.color, 0. );\n"
//			"fColor = vec4( 1., 1., 1., 1. );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
		addUniform( "model", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model ).
		addUniform( "view", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view ).
		addUniform( "projection", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection ).
		addUniform( "light", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light );

	glr.program( "PROGRAM-CUBE" ).
		setShader( "SHADER-CUBE" ).
		setVertexArray( "VERTICES-CUBE" ).
		build( );

	glr.program( "PROGRAM-LIGHTS" ).
		setShader( "SHADER-LIGHTS" ).
		setVertexArray( "VERTICES-LIGHTS" ).
		build( );
}

void
Cube::resize( int p_width, int p_height ) {

	float
	ratio = 1.f * p_width / p_height;

	projection = glm::perspective( 45.0f, ratio, 1.0f, 200.f );
}

void
Cube::paint( ) {

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	angle = .65f * vcd->time;

	model = glm::mat4( 1. );
	view = glm::translate( glm::mat4( 1. ), glm::vec3( 0.f, 0.f, -50.f ) );
	view = glm::rotate( view, angle, glm::vec3( sinf( .1f * angle ), sinf( .12f * angle ), sinf( .13f * angle ) ) );

	light2 = glm::vec4( 20.f * cosf( .2 * vcd->time ) * cosf( vcd->time ), 20.f * cosf( .21f * vcd->time ) * sinf( 1.2f * vcd->time ), 25.f * cosf( .24 * vcd->time ) * sinf( vcd->time ), 1.f );

	light = view * model * light2;

	GLRenderer::VertexArray
	& va = glr.vertices( "VERTICES-LIGHTS" );

	#ifndef GL_VERTEX_PROGRAM_POINT_SIZE
	#define GL_VERTEX_PROGRAM_POINT_SIZE      0x8642
	#endif
//	#ifndef GL_VERTEX_ATTRIB_ARRAY_NORMALIZED
//	#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
//	#endif

//	glEnable( GL_VERTEX_ATTRIB_ARRAY_NORMALIZED );
	glEnable( GL_VERTEX_PROGRAM_POINT_SIZE );

	va.arr[ 0 ] = light2.x;
	va.arr[ 1 ] = light2.y;
	va.arr[ 2 ] = light2.z;

//	model = glm::translate( glm::mat4( 1. ), glm::vec3( -2.5f ) );
	glr.run( { "PROGRAM-LIGHTS" } );

	for( int z = -4; z <= 4; ++ z ) {

		for( int y = -6; y <= 6; ++ y ) {

			for( int x = -8; x <= 8; ++ x ) {

				model = glm::translate( glm::mat4( 1. ), ( 4.f - 2.f * cosf( 3.14 * sinf( .1 * vcd->time ) ) ) * glm::vec3( x, y, z ) );

				glr.run( { "PROGRAM-CUBE" } );
			}
		}
	}
}
