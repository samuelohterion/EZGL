#include "cube.hpp"

Cube::Cube( CStr const & p_name, ViewControlData * p_vcd ) :
GLProject( p_name, p_vcd ) {

}

void
Cube::init( ) {

	glClearColor( 0., 0., 0., 1. );

	angle = vcd->time;

	light = glm::vec3( 2.f, 0.f, 0.f );

	// Create perspective projection matrix
	glm::mat4
	id( 1. );

	float
	ratio = vcd->width < vcd->height ? ( 1.f * vcd->height / vcd->width ) : ( 1.f * vcd->width / vcd->height );

	projection = glm::perspective( 45.0f, ratio, 1.0f, 100.f );

	// Apply model view transformations
	model = id;
	view = glm::translate( id, glm::vec3( 0.f, 0.f, -4.f ) );
	view = glm::rotate( view, angle, glm::vec3( 0.f, 1.f, 0.f ) );

	mvp = projection * view * model;  // Calculate final MVP matrix
	norm = glm::transpose( glm::inverse( view * model ) );

/*
* 			glr.vertices( "TRIANGULAR-VERTICES" )
	//      vertices            norms             colors
	//      x      y      z     nx    ny    nz    r     g     b
		<< -1. << +0. << +1. << 0. << 2. << 1. << 1. << 0. << 0.
		<< +1. << +0. << +1. << 0. << 2. << 1. << 0. << 1. << 0.
		<< +0. << +1. << +0. << 0. << 2. << 1. << 1. << 1. << 1.

		<< +1. << +0. << +1. << 1. << 2. << 0. << 0. << 1. << 0.
		<< +1. << +0. << -1. << 1. << 2. << 0. << 0. << 0. << 1.
		<< +0. << +1. << +0. << 1. << 2. << 0. << 1. << 1. << 1.

		<< +1. << +0. << -1. << 0. << 2. << -1. << 0. << 0. << 1.
		<< -1. << +0. << -1. << 0. << 2. << -1. << 1. << 1. << 0.
		<< +0. << +1. << +0. << 0. << 2. << -1. << 1. << 1. << 1.

		<< -1. << +0. << -1. << -1. << 2. << 0. << 1. << 1. << 0.
		<< -1. << +0. << +1. << -1. << 2. << 0. << 1. << 0. << 0.
		<< +0. << +1. << +0. << -1. << 2. << 0. << 1. << 1. << 1.;
	glr.vertices( "TRIANGULAR-VERTICES" ).addAttrib( "vertices", 3, 0 );
	glr.vertices( "TRIANGULAR-VERTICES" ).addAttrib( "norms",    3, 3 );
	glr.vertices( "TRIANGULAR-VERTICES" ).addAttrib( "colors",   3, 6 );

	glr.program( "TRIANGULAR-PROGRAM-1" ).setShader( "TRIANGULAR-SHADER" );
	glr.program( "TRIANGULAR-PROGRAM-1" ).setVertexArray( "TRIANGULAR-VERTICES" );
	glr.program( "TRIANGULAR-PROGRAM-1" ).build( );
*/
/*
*    2----3
*   /|   /|
*  6-+--7 |
*  | 0--+-1
*  |/   |/
*  4----5
*/

	GLfloat
	in = 1.f,
	out = 3.f;

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

	glr.vertices( "VERTICES-CUBE" )
	//	P0 v          n      c            P1 v          n      c            P2 v          n      c
		<< in * p0 << -px << ( py + pz )  << in * p4 << -px << ( py + pz )  << in * p6 << -px << ( py + pz )
		<< in * p6 << -px << ( py + pz )  << in * p2 << -px << ( py + pz )  << in * p0 << -px << ( py + pz )

		<< in * p7 << +px << px           << in * p5 << +px << px           << in * p1 << +px << px
		<< in * p1 << +px << px           << in * p3 << +px << px           << in * p7 << +px << px

		<< in * p0 << -py << ( px + pz )  << in * p1 << -py << ( px + pz )  << in * p5 << -py << ( px + pz )
		<< in * p5 << -py << ( px + pz )  << in * p4 << -py << ( px + pz )  << in * p0 << -py << ( px + pz )

		<< in * p7 << +py << py           << in * p3 << +py << py           << in * p2 << +py << py
		<< in * p2 << +py << py           << in * p6 << +py << py           << in * p7 << +py << py

		<< in * p0 << -pz << ( px + py )  << in * p2 << -pz << ( px + py )  << in * p3 << -pz << ( px + py )
		<< in * p3 << -pz << ( px + py )  << in * p1 << -pz << ( px + py )  << in * p0 << -pz << ( px + py )

		<< in * p7 << +pz << pz           << in * p6 << +pz << pz           << in * p4 << +pz << pz
		<< in * p4 << +pz << pz           << in * p5 << +pz << pz           << in * p7 << +pz << pz

		<< GLRenderer::VertexArray::Object( 0, 3 * 12, GL_TRIANGLES );

	glr.vertices( "VERTICES-WALL")

		<< out * p0 << px << ( py + pz )
		<< out * p2 << px << ( py + pz )
		<< out * p4 << px << ( py + pz )
		<< out * p6 << px << ( py + pz )

		<< GLRenderer::VertexArray::Object( 0, 4, GL_TRIANGLE_STRIP )

		<< out * p0 << py << ( px + pz )
		<< out * p4 << py << ( px + pz )
		<< out * p1 << py << ( px + pz )
		<< out * p5 << py << ( px + pz )

		<< GLRenderer::VertexArray::Object( 4, 4, GL_TRIANGLE_STRIP )

		<< out * p0 << pz << ( px + py )
		<< out * p1 << pz << ( px + py )
		<< out * p2 << pz << ( px + py )
		<< out * p3 << pz << ( px + py )

		<< GLRenderer::VertexArray::Object( 8, 4, GL_TRIANGLE_STRIP )

		<< out * p7 << -px << px
		<< out * p3 << -px << px
		<< out * p5 << -px << px
		<< out * p1 << -px << px

		<< GLRenderer::VertexArray::Object( 12, 4, GL_TRIANGLE_STRIP )

		<< out * p7 << -py << py
		<< out * p6 << -py << py
		<< out * p3 << -py << py
		<< out * p2 << -py << py

		<< GLRenderer::VertexArray::Object( 16, 4, GL_TRIANGLE_STRIP )

		<< out * p7 << -pz << pz
		<< out * p5 << -pz << pz
		<< out * p6 << -pz << pz
		<< out * p4 << -pz << pz

		<< GLRenderer::VertexArray::Object( 20, 4, GL_TRIANGLE_STRIP );

	glr.vertices( "VERTICES-CUBE" ).addAttrib( "vertices", 3, 0 );
	glr.vertices( "VERTICES-CUBE" ).addAttrib( "norms",    3, 3 );
	glr.vertices( "VERTICES-CUBE" ).addAttrib( "colors",   3, 6 );

	glr.vertices( "VERTICES-WALL" ).addAttrib( "vertices", 3, 0 );
	glr.vertices( "VERTICES-WALL" ).addAttrib( "norms",    3, 3 );
	glr.vertices( "VERTICES-WALL" ).addAttrib( "colors",   3, 6 );

	glr.shader( "SHADER-CUBE", "../EZGL/glprojects/cube/cube.vsh", "../EZGL/glprojects/cube/cube.fsh", GLRenderer::ShaderCode::FROM_FILE );
	glr.shader( "SHADER-CUBE" ).addUniform( "model", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model );
	glr.shader( "SHADER-CUBE" ).addUniform( "view", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view );
	glr.shader( "SHADER-CUBE" ).addUniform( "projection", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection );
	glr.shader( "SHADER-CUBE" ).addUniform( "time", GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, & vcd->time );
	glr.shader( "SHADER-CUBE" ).addUniform( "light", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light );

	glr.shader( "SHADER-WALL", "../EZGL/glprojects/cube/cube.vsh", "../EZGL/glprojects/cube/cube.fsh", GLRenderer::ShaderCode::FROM_FILE );
	glr.shader( "SHADER-WALL" ).addUniform( "model", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & model );
	glr.shader( "SHADER-WALL" ).addUniform( "view", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & view );
	glr.shader( "SHADER-WALL" ).addUniform( "projection", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & projection );
	glr.shader( "SHADER-WALL" ).addUniform( "time", GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, & vcd->time );
//	glr.shader( "SHADER-CUBE" ).addUniform( "light", GLRenderer::Shader::VEC3, GLRenderer::Shader::SCALAR, & light );

	glr.program( "PROGRAM-1" ).setShader( "SHADER-CUBE" );
	glr.program( "PROGRAM-1" ).setVertexArray( "VERTICES-CUBE" );
	glr.program( "PROGRAM-1" ).build( );

	glr.program( "PROGRAM-2" ).setShader( "SHADER-WALL" );
	glr.program( "PROGRAM-2" ).setVertexArray( "VERTICES-WALL" );
	glr.program( "PROGRAM-2" ).build( );
/*
	glr.shader( "S1", "../shaders/stage1.vsh", "../shaders/stage1.fsh", GLRenderer::ShaderCode::FROM_FILE );
	glr.shader( "S1" ).addUniform( "ratio", GLRenderer::Shader::VEC2,  GLRenderer::Shader::SCALAR, &s1Vars.ratio );
	glr.shader( "S1" ).addUniform( "cntr", GLRenderer::Shader::VEC2,  GLRenderer::Shader::SCALAR, &s1Vars.cntr );
	glr.shader( "S1" ).addUniform( "time", GLRenderer::Shader::FLOAT,  GLRenderer::Shader::SCALAR, &s1Vars.time );
	glr.shader( "S1" ).addUniform( "zoom", GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, &s1Vars.zoom );


	glr.shader( "S2", "../shaders/stage2.vsh", "../shaders/stage2.fsh", GLRenderer::ShaderCode::FROM_FILE );
	glr.shader( "S2" ).addUniform( "dims", GLRenderer::Shader::VEC2,  GLRenderer::Shader::SCALAR, &s2Vars.dims );
	glr.shader( "S2" ).addUniform( "time", GLRenderer::Shader::FLOAT,  GLRenderer::Shader::SCALAR, &s2Vars.time );

	glr.shader( "S3", "../shaders/stage3.vsh", "../shaders/stage3.fsh", GLRenderer::ShaderCode::FROM_FILE );
	glr.shader( "S3" ).addUniform( "dimsRec", GLRenderer::Shader::VEC2,  GLRenderer::Shader::SCALAR, &s3Vars.dimsRec );
//	glr.sh[ "S3" ]->addUniform( "time", GLRenderer::Shader::FLOAT,  GLRenderer::Shader::SCALAR, &s2Vars.time );

	glr.frameBuffer( "FB1" );

	glr.texture(
		"TX1",
		new GLRenderer::Texture(
			"TX1",
			GL_TEXTURE_2D,
			0,
			GL_R32I,
			GL_NEAREST, GL_NEAREST,
			GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
			GL_RED_INTEGER, GL_INT,
			vcd->width, vcd->height
		)
	);

	glr.texture(
		"TX2",
		new GLRenderer::Texture(
			"TX2",
			GL_TEXTURE_2D,
			0,
			GL_RGBA32F,
			GL_NEAREST, GL_NEAREST,
			GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
			GL_RGBA, GL_FLOAT,
			vcd->width, vcd->height
		)
	);

	glr.vertices( "VA1" )
	//	    x      y
		<< -1. << -1.
		<< +1. << -1.
		<< -1. << +1.
		<< +1. << +1.;

	glr.vertices( "VA1" ).addAttrib( "verts", 2, 0 );

	glr.vertices( "VA2" )
	//	    x      y     s     t
		<< -1. << -1. << 0. << 0.
		<< +1. << -1. << 1. << 0.
		<< -1. << +1. << 0. << 1.
		<< +1. << +1. << 1. << 1.;

	glr.vertices( "VA2" ).addAttrib( "verts", 2, 0 );
	glr.vertices( "VA2" ).addAttrib( "coords", 2, 2 );

	glr.program( "PRG1" ).setFrameBuffer( "FB1" );
	glr.program( "PRG1" ).setShader( "S1" );
	glr.program( "PRG1" ).addOutTexture( "TX1" );
	glr.program( "PRG1" ).setVertexArray( "VA1" );
	glr.program( "PRG1" ).build( );

	glr.program( "PRG2" ).setFrameBuffer( "FB1" );
	glr.program( "PRG2" ).setShader( "S2" );
	glr.program( "PRG2" ).addInTexture( "TX1" );
	glr.program( "PRG2" ).addOutTexture( "TX2" );
	glr.program( "PRG2" ).setVertexArray( "VA2" );
	glr.program( "PRG2" ).build( );

	glr.program( "PRG3" ).setShader( "S3" );
	glr.program( "PRG3" ).addInTexture( "TX2" );
	glr.program( "PRG3" ).setVertexArray( "VA2" );
	glr.program( "PRG3" ).build( );
*/
}

void
Cube::resize( int p_width, int p_height ) {

//			glr.tx[ "TX1" ]->resize( vcd->width, vcd->height );
//			glr.tx[ "TX2" ]->resize( vcd->width, vcd->height );
	glm::mat4
	id( 1. );

	float
	ratio = p_width < p_height ? ( 1.f * p_width / p_height ) : ( 1.f * p_width / p_height );

	projection = glm::perspective( 45.0f, ratio, 1.0f, 100.f );

	// Apply model view transformations
//			model = id;
//			view = glm::translate( id, glm::vec3( 0.f, 0.f, -4.f ) );
//			view = glm::rotate( view, angle, glm::vec3( 0.f, 1.f, 1.f ) );

//			mvp = projection * view * model;  // Calculate final MVP matrix
//			norm = glm::transpose( glm::inverse( view * model ) );
}

void
Cube::paint( ) {

	angle = .15f * vcd->time;
//	light = glm::vec3( 5.f * sin( vcd->time ), 0.f, 0.f );
	light = glm::vec3( 2.f, 2.f, 2.f );

	glm::mat4
	id( 1. );

//			glm::vec2
//			wh( vcd->width, vcd->height );

//			projection = glm::perspective( 45.0f, wh.x / wh.y, 1.0f, 100.f );

	// Apply model view transformations
//			model = id;
	view = glm::translate( id, glm::vec3( 0.f, 0.f, -6.f ) );
	view = glm::rotate( view, angle, glm::vec3( sin( .1 * angle ), sin( .12 * angle ), sin( .13 * angle ) ) );
//	view = glm::rotate( view, angle, glm::vec3( 0.f, 1.f, 0.f ) );

//	mvp = projection * view * model;  // Calculate final MVP matrix
//	norm = glm::transpose( glm::inverse( mvp ) );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

//		glr.run( { "PRG1", "PRG2", "PRG3" } );
	glr.run( { "PROGRAM-1", "PROGRAM-2" } );
}
