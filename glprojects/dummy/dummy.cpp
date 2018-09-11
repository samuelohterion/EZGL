#include "dummy.hpp"
#include "../../code/glmprinter.hpp"

Dummy::Dummy ( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ),
m ( glm::mat4( 1.f ) ),
v ( m ),
p ( m ),
tetrahedron(
	Triangle ( glm::vec3 ( -0.4142136, -0.4142136, -0.4142136 ), glm::vec3 ( 1.f, 0.f,0.f ), glm::vec3 ( 0.f, 1.f, 0.f ), glm::vec3 ( 0.f, 1.f, 0.f ) ),
	Triangle ( glm::vec3 ( -0.4142136, -0.4142136, -0.4142136 ), glm::vec3 ( 0.f, 1.f,0.f ), glm::vec3 ( 0.f, 0.f, 1.f ), glm::vec3 ( 0.f, 1.f, 0.f ) ),
	Triangle ( glm::vec3 ( -0.4142136, -0.4142136, -0.4142136 ), glm::vec3 ( 0.f, 0.f,1.f ), glm::vec3 ( 1.f, 0.f, 0.f ), glm::vec3 ( 0.f, 1.f, 0.f ) ),
	Triangle ( glm::vec3 ( 1.f, 0.f, 0.f ), glm::vec3 ( 0.f, 1.f,0.f ), glm::vec3 ( 0.f, 0.f, 1.f ), glm::vec3 ( 0.f, 1.f, 0.f ) ) ) {

	float
	a = sqrtf( 1.f / 24.f ),
	b = sqrtf( 2.f / 24.f ),
	c = sqrtf( 1.f / 3.f ),
	d = sqrtf( 2.f / 3.f );

	glm::vec3
	p0 = glm::vec3( +0.f, +0.f, d - a ),
	p1 = glm::vec3( -b,   -.5f,    -a ),
	p2 = glm::vec3( -b,   +.5f,    -a ),
	p3 = glm::vec3( +c,   +0.f,    -a ),
	pc = .25f * ( p0 + p1 + p2 + p3 );

	p0 -= pc;
	p1 -= pc;
	p2 -= pc;
	p3 -= pc;

	tetrahedron.a = Triangle( p0, p1, p2, glm::vec3( 1,0,0 ) );
	tetrahedron.b = Triangle( p0, p2, p3, glm::vec3( 0,1,0 ) );
	tetrahedron.c = Triangle( p0, p3, p1, glm::vec3( 0,0,1 ) );
	tetrahedron.d = Triangle( p3, p2, p1, glm::vec3( 1,1,0 ) );

	tetrahedron . divide ( 5 );
}

Dummy::~Dummy ( ){

}

void
Dummy::init ( ) {

	glClearColor ( .0f, .0f, .0f, 1.f );

	// frame buffers
	{
	}

	// textures
	{
	}

	// vertex arrays
	{
		// V-DUMMY-TRIANGLE
		{
			GLR::VertexArray
			& va = glr.vertices ( "V-DUMMY-TRIANGLE" ).
				attrib ( "vertex", 0, 3 ).
				attrib ( "color",  3, 3 ).
				setUsage ( GL_DYNAMIC_DRAW );

			tetrahedron.fill ( va );

			va << GLR::VertexArray::Object ( 0, va.vertexCount ( ), GL_TRIANGLES );
		}
	}

	// index arrays
	{
	}

	// shaders
	{
		//S-DUMMY-TRIANGLE"
		{
			glr.shader (
				"S-DUMMY-TRIANGLE",

				//Vertex Shader
				"#version 330 core\n"
				"layout( location = 0 ) in vec3 vertex;\n"
				"layout( location = 1 ) in vec3 color;\n"
				"uniform mat4 mvp;\n"
				"out VS2FS {\n"
				"	vec3 color;\n"
				"} vs2fs;\n"
				"void main( void ) {\n"
					"vs2fs.color = color;\n"
					"gl_Position = mvp * vec4( vertex, 1. );"
				"}\n",

				//Fragment Shader
				"#version 330 core\n"
				"in VS2FS {\n"
				"	vec3 color;\n"
				"} vs2fs;\n"
				"out vec4 fColor;\n"
				"void main( void ) {\n"
					"fColor = vec4( vs2fs.color, 1. );\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE ).
				addUniform( "mvp", GLR::Shader::MAT4, GLR::Shader::SCALAR, & mvp );
		}
	}

	// container
	{
		// C-DUMMY-TRIANGLE
		{
			glr.container( "C-DUMMY-TRIANGLE" ).
				setVertexArray( "V-DUMMY-TRIANGLE" ).
				setShader( "S-DUMMY-TRIANGLE" ).
				build( );
		}
	}

	p = v = m = glm::mat4( 1. );

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
}

void
Dummy::paint( ) {

	float
	angle = .015f * vcd->time;

	glm::mat4
	e = glm::mat4( 1. );

	v = glm::translate( e, glm::vec3( 0.f, 0.f, -3.f ) );
	m = glm::rotate( e, angle, glm::vec3( 0.f, 1.f, 0.f ) );
	e = m;
	m = glm::rotate( m, 7.f * vcd->mousex / vcd->width,  glm::vec3( glm::vec4( 0.f, 1.f, 0.f, 0.f ) ) );
	m = glm::rotate( m, 7.f * vcd->mousey / vcd->height, glm::vec3( glm::vec4( 1.f, 0.f, 0.f, 0.f ) * glm::inverse( e ) ) );

	mvp = p * v * m;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glr.run( { "C-DUMMY-TRIANGLE" } );
}

void
Dummy::resize( int p_width, int p_height ) {

	float
	w = p_width,
	h = p_height,
	ratio = w / h;

	p = glm::perspective(  45.0f, ratio, 1.0f, 100.f );

	std::cout << std::endl;
}
