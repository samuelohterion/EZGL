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

	tetrahedron . divide ( 7 );
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

	v = glm::translate( v, glm::vec3( 0.f, 0.f, -6.f ) );

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
}

void
Dummy::paint( ) {

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if( vcd->buttons & 0x02 )

		v = glm::translate( v, glm::vec3( 0.f, 0.f, .01f * vcd->dMouse.y ) );

	glm::vec3 const
	X = glm::vec3( 1, 0, 0 ),
	Y = glm::vec3( 0, 1, 0 ),
	Z = glm::vec3( 0, 0, 1 );

	glm::mat3
	f = glm::mat3( inverse( m ) );

	glm::mat4
	vp = p * v;

	glm::vec2
	dAngle = .01f * vcd->dMouse;

	// f fixes the rotation axis to local Y
	m = glm::rotate( m, .01f, f * Y );

	// without f rotation around model Y
	// m = glm::rotate( m, .01f, Y );

	if( 0 < abs( dAngle.y ) && vcd->buttons & 0x01 )

		m = glm::rotate( m, -dAngle.y, f[ 0 ] );

	if( 0 < abs( dAngle.x ) && vcd->buttons & 0x01 )

		m = glm::rotate( m, +dAngle.x, f[ 1 ] );

	m = glm::translate( m, -2.f * Z );

	mvp = vp * glm::rotate( m, + vcd->time, Z );

	glr.run( { "C-DUMMY-TRIANGLE" } );

	m = glm::translate( m, +4.f * Z );

	mvp = vp * glm::rotate( m, - vcd->time, Z );

	glr.run( { "C-DUMMY-TRIANGLE" } );

	m = glm::translate( m, -2.f * ( Y + Z ) );

	mvp = vp * glm::rotate( m, + vcd->time, Y );

	glr.run( { "C-DUMMY-TRIANGLE" } );

	m = glm::translate( m, +4.f * Y );

	mvp = vp * glm::rotate( m, - vcd->time, Y );

	glr.run( { "C-DUMMY-TRIANGLE" } );

	m = glm::translate( m, -2.f * ( X + Y ) );

	mvp = vp * glm::rotate( m, + vcd->time, X );

	glr.run( { "C-DUMMY-TRIANGLE" } );

	m = glm::translate( m, +4.f * X );

	mvp = vp * glm::rotate( m, - vcd->time, X );

	glr.run( { "C-DUMMY-TRIANGLE" } );

	m = glm::translate( m, -2.f * X );
}

void
Dummy::resize( int p_width, int p_height ) {

	float
	w = p_width,
	h = p_height,
	ratio = w / h;

	p = glm::perspective(  45.0f, ratio, 1.0f, 100.f );
}
