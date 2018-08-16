#include "spheredynamicsurface.hpp"

typedef glm::vec3 V3;
typedef glm::mat4 M4;

SphereDynamicalSurface::SphereDynamicalSurface( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
SphereDynamicalSurface::init( ) {

	glClearColor( .0f, .0f, .0f, 1.f );

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
	glDepthFunc( GL_LESS );

	m = v = p = glm::mat4( 1. );

	nrm = glm::mat3( 1. );

	GLRenderer::VertexArray
	&va = glr.vertices( "SDS-VERTICES" );

	va.	setUsage( GL_STATIC_DRAW ).
		addAttrib( "vertex", 3, 0 ) <<
		GLRenderer::VertexArray::Object( 0, 0, GL_POINTS );

	glr.shader(
		"SDS-SHADER",
		//Vertex Shader
		"#version 330 core\n"
		"layout( location = 0 ) in vec3 vertex;\n"
		"uniform mat4 mvp;\n"
		"void main( void ) {\n"
			"gl_Position  = mvp * vec4( vertex, 1. );\n"
		"}\n",

		//Fragment Shader
		"#version 330 core\n"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
//			"fColor = vec4( 1., 1., 1., 1. );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "mvp", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mvp );

	glr.program( "SDS-PROGRAM" ).
		setVertexArray( "SDS-VERTICES" ).
		setShader( "SDS-SHADER" ).
		build( );
}

void
SphereDynamicalSurface::simulate( ) {

	GLRenderer::VertexArray
	va = glr.vertices( "SDS-VERTICES" );

	for( int i = 0; i < va.vertexCount( ); ++ i ) {

		glm::vec3
		a( va.arr[ 3 * i + 0 ], va.arr[ 3 * i + 1 ], va.arr[ 3 * i + 2 ] );

		for( int j = i + 1; j < va.vertexCount( ); ++ j ) {

			glm::vec3
			b( va.arr[ 3 * j + 0 ], va.arr[ 3 * j + 1 ], va.arr[ 3 * j + 2 ] ),
			c = b - a,
			cn = glm::normalize( c );

			float
			d2= glm::dot( c, c );

			acc[ i ] = acc[ i ] - cn / d2;
			acc[ j ] = acc[ j ] + cn / d2;
		}
	}

	float
	dt = .1f;

	std::vector< float >
	& v = va.arr;

	for( std::size_t i = 0; i < va.vertexCount( ); ++ i ) {

		glm::vec3
		p( v[ 3 * i + 0 ], v[ 3 * i + 1 ], v[ 3 * i + 2 ] );

		p = p + vel[ i ] * dt;

		if( 1. < glm::dot( p.z, p.z ) )

			p = glm::normalize( p );

		v[ 3 * i + 0 ] = p.x;
		v[ 3 * i + 1 ] = p.y;
		v[ 3 * i + 2 ] = p.z;

		vel[ i ] = vel[ i ] + acc[ i ] + dt;
		acc[ i ] = glm::vec3( 0.f );
	}
}


void
SphereDynamicalSurface::paint( ) {

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
			glm::vec3( sin( .3 * angle ), sin( .23 * angle ), sin( .13 * angle ) ) );

	mvp = p * m * v;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glr.run( { "SDS-PROGRAM" } );

	GLRenderer::VertexArray
	& va = glr.vertices( "SDS-VERTICES" );

	if( vcd->time < 10 ) {

		if( va.vertexCount( ) < vcd->time ) {

			va << 0.f << 0.f << 0.f;

			vel.push_back( glm::vec3( 0.f, 0.f, 0.f ) );
			acc.push_back( glm::vec3( 0.f, 0.f, 0.f ) );
		}
	}

	simulate( );
}

void
SphereDynamicalSurface::resize( int p_width, int p_height ) {

	float
	ratio = ( 1.f * p_width / p_height );

	p = glm::perspective( 45.0f, ratio, 1.0f, 10.f );
}
