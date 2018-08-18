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

	lightColors[ 0 ] = px;
	lightColors[ 1 ] = py;
	lightColors[ 2 ] = pz;
	lightColors[ 3 ] = px + py;
	lightColors[ 4 ] = py + pz;
	lightColors[ 5 ] = pz + px;
	lightColors[ 6 ] = px + py + pz;

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
			"vec3 vertex, color, frame;\n"
		"} vs2gs;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main( ) {\n"
			"vs2gs.frame = vertex;\n"
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
			"vec3 vertex, color, frame;\n"
		"} vs2gs[ ];\n"
		"out GS2FS {\n"
			"vec3 vertex, color, normal, frame;\n"
		"} gs2fs;\n"
		"void main( ) {\n"
			"vec3 n = normalize( cross( vs2gs[ 1 ].vertex - vs2gs[ 0 ].vertex, vs2gs[ 2 ].vertex - vs2gs[ 0 ].vertex ) );\n"

			"gl_Position = gl_in[ 0 ].gl_Position;\n"
			"gs2fs.frame = vs2gs[ 0 ].frame;\n"
			"gs2fs.vertex = vs2gs[ 0 ].vertex;\n"
			"gs2fs.normal = n;\n"
			"gs2fs.color  = vs2gs[ 2 ].color;\n"
			"EmitVertex( );\n"

			"gl_Position = gl_in[ 1 ].gl_Position;\n"
			"gs2fs.frame = vs2gs[ 1 ].frame;\n"
			"gs2fs.vertex = vs2gs[ 1 ].vertex;\n"
			"gs2fs.normal = n;\n"
			"gs2fs.color  = vs2gs[ 2 ].color;\n"
			"EmitVertex( );\n"

			"gl_Position = gl_in[ 2 ].gl_Position;\n"
			"gs2fs.frame = vs2gs[ 2 ].frame;\n"
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
			"normal,\n"
			"frame;\n"
		"} gs2fs;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"uniform vec3 lightPos[ 7 ];\n"
		"uniform vec3 lightCol[ 7 ];\n"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
			"vec3  d = ( vec4( 0., 0., +1., 0. ) ).xyz;\n"
			"float a = dot( gs2fs.normal, d );\n"
			"vec4 f = .25 * vec4( clamp( a, 0, 1 ) * ( 3. + gs2fs.color ), 1.f );\n"
			"int i = 0;\n"
			"for( i = i; i < 7; ++ i ) {"

				"d = lightPos[ i ] - gs2fs.vertex;\n"
				"a = 10. * normalize( dot( gs2fs.normal, d ) );\n"
				"f += vec4( ( clamp( a / dot( d, d ), 0, 1 ) ) * ( ( 1. + lightCol[ i ] ) * ( 3. + gs2fs.color ) * .125 ), 1.f );\n"
			"}\n"
			"float b =\n"
				"( .9 > abs( gs2fs.frame.x ) && ( .9 > abs( gs2fs.frame.y ) ) ) ||\n"
				"( .9 > abs( gs2fs.frame.y ) && ( .9 > abs( gs2fs.frame.z ) ) ) ||\n"
				"( .9 > abs( gs2fs.frame.z ) && ( .9 > abs( gs2fs.frame.x ) ) )\n"
					"? 1.f\n"
					": 0.5f;\n"

			"fColor = vec4( b * clamp( f.xyz / f.a, vec3( 0 ), vec3( 1 ) ), 1. );\n"
//			"fColor.rgb /= fColor.a;\n"
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
		addUniform( "lightCol[6]", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & lightColors[ 6 ] );

	glr.program( "PROGRAM-CUBE" ).
		setShader( "SHADER-CUBE" ).
		setVertexArray( "VERTICES-CUBE" ).
		build( );

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
			"gl_PointSize = 20.- .25 * v.z;\n"
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
}

void
Cube::resize( int p_width, int p_height ) {

	float
	ratio = 1.f * p_width / p_height;

	projection = glm::perspective( 30.0f, ratio, 1.0f, 200.f );
}

#ifndef GL_VERTEX_PROGRAM_POINT_SIZE
#define GL_VERTEX_PROGRAM_POINT_SIZE      0x8642
#endif
//	#ifndef GL_VERTEX_ATTRIB_ARRAY_NORMALIZED
//	#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
//	#endif
void
Cube::paint( ) {

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	angle = vcd->time;

	model = glm::mat4( 1. );
	view = glm::translate( glm::mat4( 1. ), glm::vec3( 0.f, 0.f, -40.f ) );
	view = glm::rotate( view, .123f * angle, glm::vec3( sinf( .1f * angle ), sinf( .21f * angle ), sinf( .31f * angle ) ) );

	GLRenderer::VertexArray
	& va = glr.vertices( "VERTICES-LIGHTS" );

	glEnable( GL_VERTEX_ATTRIB_ARRAY_NORMALIZED );
	glEnable( GL_VERTEX_PROGRAM_POINT_SIZE );

	float
	phase = 3.14159f * sinf( .023f * angle );

	for( std::size_t i = 0; i < 7; ++ i ) {

		lightModelSpace[ i ] = glm::vec4( 5.f * cosf( phase * i + angle ), 11.f * sinf( phase * i + 1.2f * angle ), 21.f * sinf( phase * i + angle ), 1.f );

		lightCameraSpace[ i ] = view * model * lightModelSpace[ i ];

		va.arr[ 6 * i + 0 ] = lightModelSpace[ i ].x;
		va.arr[ 6 * i + 1 ] = lightModelSpace[ i ].y;
		va.arr[ 6 * i + 2 ] = lightModelSpace[ i ].z;
	}

	glr.run( { "PROGRAM-LIGHTS" } );

	glDisable( GL_VERTEX_PROGRAM_POINT_SIZE );
	glDisable( GL_VERTEX_ATTRIB_ARRAY_NORMALIZED );


	for( int z = -9; z <= 9; ++ z ) {

		for( int y = -4; y <= 4; ++ y ) {

			for( int x = -1; x <= 1; ++ x ) {

				float
				f = cosf( 3.14f * sinf( .1f * vcd->time ) );

				f = f * f;
				f = f * f;
				f = f * f;
				f = 1.f - f;
				model = glm::translate( glm::mat4( 1. ), ( 6.f - 4.f * f ) * glm::vec3( x, y, z ) );

				glr.run( { "PROGRAM-CUBE" } );
			}
		}
	}
}
