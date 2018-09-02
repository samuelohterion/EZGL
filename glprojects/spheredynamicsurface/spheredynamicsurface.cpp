#include "spheredynamicsurface.hpp"

typedef glm::vec3 V3;
typedef glm::mat4 M4;

SphereDynamicalSurface::SphereDynamicalSurface( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
SphereDynamicalSurface::init( ) {

	glClearColor( .0f, .0f, .0f, 1.f );

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
	glDepthFunc( GL_LESS );

	m = v = p = glm::mat4( 1. );

	nrm = glm::mat3( 1. );

	GLR::VertexArray
	&va = glr.vertices( "SDS-VERTICES" );

	va.	setUsage( GL_STATIC_DRAW ).
		attrib( "vertex", 3, 0 ) <<
		GLR::VertexArray::Object( 0, 0, GL_POINTS );

	glr.shader(
		"SDS-SHADER",
		//Vertex Shader
		"#version 330 core\n"
		"in vec3 vertex;\n"
		"uniform mat4 mvp;\n"
		"void main( void ) {\n"
			"gl_Position  = vec4( vertex.x, vertex.y, 0., 1. );\n"
		"}\n",

		//Fragment Shader
		"#version 330 core\n"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
			"fColor = vec4( 1., 1., 1., 1. );\n"
		"}\n",
		GLR::ShaderCode::FROM_CODE ).
			addUniform( "mvp", GLR::Shader::MAT4, GLR::Shader::SCALAR, & mvp );

	glr.container( "SDS-PROGRAM" ).
		setVertexArray( "SDS-VERTICES" ).
		setShader( "SDS-SHADER" ).
		build( );
}

void
SphereDynamicalSurface::simulate( ) {

	float
	dt = .01f;

	GLR::VertexArray
	va = glr.vertices( "SDS-VERTICES" );

	std::vector< float >
	& v = va.arr;

	for( int i = 0; i < va.vertexCount( ); ++ i ) {

		glm::vec3
		a( v[ 3 * i + 0 ], v[ 3 * i + 1 ], v[ 3 * i + 2 ] );

		for( int j = i + 1; j < va.vertexCount( ); ++ j ) {

			glm::vec3
			b( v[ 3 * j + 0 ], v[ 3 * j + 1 ], v[ 3 * j + 2 ] ),
			c = b - a,
			cn = glm::normalize( c );

			float
			d2 = glm::dot( c, c );

			acc[ i ] = acc[ i ] - cn / d2;
			acc[ j ] = acc[ j ] + cn / d2;
		}
	}

	for( std::size_t i = 0; i < va.vertexCount( ); ++ i ) {

		glm::vec3
		r( v[ 3 * i + 0 ], v[ 3 * i + 1 ], v[ 3 * i + 2 ] );

		r = r + vel[ i ] * dt;

		r = glm::normalize( r );

		v[ 3 * i + 0 ] = r.x;
		v[ 3 * i + 1 ] = r.y;
		v[ 3 * i + 2 ] = r.z;

		vel[ i ] = vel[ i ] + acc[ i ] * dt;

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

	mvp = glm::mat4( 1.f );//p * v * m;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glr.run( { "SDS-PROGRAM" } );

	GLR::VertexArray
	& va = glr.vertices( "SDS-VERTICES" );

	if( vcd->time < 1000 ) {

		if( va.vertexCount( ) < vcd->time ) {

			va << glm::fract( 1.f * rand( ) / RAND_MAX - .5f )  << glm::fract( 1.f * rand( ) / RAND_MAX - .5f ) << glm::fract( 1.f * rand( ) / RAND_MAX - .5f );

			vel.push_back( glm::vec3( 0.f, 0.f, 0.f ) );
			acc.push_back( glm::vec3( 0.f, 0.f, 0.f ) );

			va.obj[ 0 ].size = va.vertexCount( );
		}
	}

	simulate( );

	for( auto i : va.arr )

		std::cout << i << "  ";

	std::cout << std::endl;
}

void
SphereDynamicalSurface::resize( int p_width, int p_height ) {

	float
	ratio = ( 1.f * p_width / p_height );

	p = glm::perspective( 45.0f, ratio, 0.1f, 10.f );
}
