#include "coordinatesystem.hpp"

typedef glm::vec3 V3;
typedef glm::mat4 M4;

void
print( glm::vec2 const & p_vec ) {

	std::cout << p_vec[ 0 ] << " " << p_vec[ 1 ];
}

void
print( glm::vec3 const & p_vec ) {

	std::cout << p_vec[ 0 ] << " " << p_vec[ 1 ] << " " << p_vec[ 2 ];
}

void
print( glm::vec4 const & p_vec ) {

	std::cout << p_vec[ 0 ] << " " << p_vec[ 1 ] << " " << p_vec[ 2 ] << " " << p_vec[ 3 ];
}

void
print( glm::mat2 const & p_mat ) {

	print( p_mat[ 0 ] );
	std::cout <<  std::endl;
	print( p_mat[ 1 ] );
}

void
print( glm::mat3 const & p_mat ) {

	print( p_mat[ 0 ] );
	std::cout <<  std::endl;
	print( p_mat[ 1 ] );
	std::cout <<  std::endl;
	print( p_mat[ 2 ] );
}

void
print( glm::mat4 const & p_mat ) {

	print( p_mat[ 0 ] );
	std::cout <<  std::endl;
	print( p_mat[ 1 ] );
	std::cout <<  std::endl;
	print( p_mat[ 2 ] );
	std::cout <<  std::endl;
	print( p_mat[ 3 ] );
}


CoordinateSystem::CoordinateSystem( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
CoordinateSystem::init( ) {

	glClearColor( .0f, .0f, .0f, 1.f );

	m = glm::mat4( 1. );

	glr.vertices( "CS-VERTICES" ).
		setUsage( GL_STATIC_DRAW ).
		addAttrib( "vertex", 3, 0 ) <<
		+0.f << +0.f << +0.f <<
		+1.f << +0.f << +0.f <<
		+0.f << +0.f << +0.f <<
		+0.f << +2.f << +0.f <<
		+0.f << +0.f << +0.f <<
		+0.f << +0.f << +3.f <<
		GLRenderer::VertexArray::Object( 0, 6, GL_LINES );

	glr.shader(
		"CS-SHADER",
		//Vertex Shader
		"#version 330 core\n"
		"layout( location = 0 ) in vec3 vertex;\n"
		"uniform mat4 mv;\n"
		"uniform mat4 p;\n"
		"out VS2GS { vec4 vertex, mvv; } vs2gs;\n"
		"void main( void ) {\n"
			"vec4 v = mv * vec4( vertex, 1. );"
			"vs2gs.mvv = v;\n"
			"vs2gs.vertex = p * v;\n"
			"gl_Position = p * v;\n"
		"}\n",

		//Geometry Shader
		"#version 330 core\n"
		"layout ( lines ) in;\n"
		"layout ( line_strip, max_vertices = 2 ) out;\n"
		"in VS2GS { vec4 vertex, mvv; } vs2gs[ ];\n"
		"out GS2FS { vec4 color; } gs2fs;\n"
		"void main( void ) {\n"
			"gs2fs.color  = vec4( max( vec3( 0. ), vs2gs[ 0 ].mvv.xyz - vs2gs[ 1 ].mvv.xyz ), 1. );\n"
			"gl_Position  = gl_in[ 0 ].gl_Position;\n"
			"EmitVertex( );\n"
			"gs2fs.color  = vec4( max( vec3( 0. ), vs2gs[ 1 ].mvv.xyz - vs2gs[ 0 ].mvv.xyz ), 1. );\n"
			"gl_Position = gl_in[ 1 ].gl_Position;\n"
			"EmitVertex( );\n"
			"EndPrimitive( );\n"
		"}\n",

		//Fragment Shader
		"#version 330 core\n"
		"in GS2FS { vec4 color; } gs2fs;\n"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
			"fColor = gs2fs.color;\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "mv", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv ).
			addUniform( "p",  GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p );

	glr.program( "CS-PROGRAM" ).
		setVertexArray( "CS-VERTICES" ).
		setShader( "CS-SHADER" ).
		build( );
	}

void
CoordinateSystem::paint( ) {

	float
	angle = .31f * vcd->time;

//	v = glm::lookAt(
//		glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
//		glm::vec3(0,0,0), // and looks at the origin
//		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
//	);

	v = glm::rotate(
			glm::translate(
				glm::mat4( 1. ),
				glm::vec3( 0.f, 0.f, -4.f ) ),
			angle,
			glm::vec3( .5 * sin( .2 * angle ), 1.f, 0.f ) );

	mv = v * m;

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glr.run( { "CS-PROGRAM" } );

	print( mv * glm::vec4( 0., 0., 1., 1. ) );

	std::cout << std::endl;
}

void
CoordinateSystem::resize( int p_width, int p_height ) {

	float
	ratio = ( 1.f * p_width / p_height );

	p = glm::perspective( 45.0f, ratio, 1.0f, 100.f );
}
