#include "sphere.hpp"

typedef glm::vec3 V3;
typedef glm::mat4 M4;

Sphere::Sphere( CStr const & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

}

void
Sphere::init( ) {

	glClearColor( .0f, .0f, .0f, 1.f );

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
	glDepthFunc( GL_LESS );

	m = v = p = glm::mat4( 1. );

	nrm = glm::mat3( 1. );

	glr.vertices( "SPHERE-VERTICES" ).
		setUsage( GL_STATIC_DRAW ).
		addAttrib( "vertex", 3, 0 ).addAttrib( "normal", 3, 3 );

	GLRenderer::VertexArray
	&va = glr.vertices( "SPHERE-VERTICES" );

	glm::vec3
	normal = glm::vec3( 1., 0., 0. );

	int
	vertexSiz = 0,
	vertexOff = 0;

	va << normal << normal;
	++ vertexSiz;

	int
	sizeLong = 16,
	sizeLat  = 8;

	float
	factorLong = 2.f * 3.14159265f / ( sizeLong - 0 ),
	factorLat  = 3.14159265f / ( sizeLat - 0 ),
	x,y,z;

	int latitude = 1;

	for( ; latitude < 2; ++ latitude ) {

		float
		clat = cos( factorLat * latitude ),
		slat = sin( factorLat * latitude );

		for( int longitude = 0; longitude <= sizeLong + 1; ++ longitude ) {

			float
			clon = cosf( factorLong * longitude ),
			slon = sinf( factorLong * longitude );

			x = clat;
			y = slat * clon;
			z = slat * slon;

			va << x << y << z << x << y << z;
			++ vertexSiz;
		}
	}

	va << GLRenderer::VertexArray::Object( vertexOff, vertexSiz, GL_TRIANGLE_FAN );
	vertexOff = vertexSiz - vertexOff;

	for( ; latitude < sizeLat - 1; ++ latitude ) {

//		va << x << y << z;
//		++ vertexId;

		float
		clat1 = cosf( factorLat * ( latitude - 1 ) ),
		slat1 = sinf( factorLat * ( latitude - 1 ) ),
		clat2 = cosf( factorLat * ( latitude + 0 ) ),
		slat2 = sinf( factorLat * ( latitude + 0 ) );

		for( int longitude = 0; longitude <= sizeLong; ++ longitude ) {

			float
			clon1 = cosf( factorLong * longitude ),
			slon1 = sinf( factorLong * longitude ),
			clon2 = cosf( factorLong * ( longitude + 1 ) ),
			slon2 = sinf( factorLong * ( longitude + 1 ) ),
			x1 = clat2,
			y1 = slat2 * clon1,
			z1 = slat2 * slon1,
			x2 = clat1,
			y2 = slat1 * clon2,
			z2 = slat1 * slon2;

			va <<
			x1 << y1 << z1 << x1 << y1 << z1 <<
			x2 << y2 << z2 << x2 << y2 << z2;

			vertexSiz += 2;
		}
		va << GLRenderer::VertexArray::Object( vertexOff, vertexSiz, GL_TRIANGLE_STRIP );
		vertexOff = vertexSiz - vertexOff;
	}


	glr.shader(
		"SPHERE-SHADER",
		//Vertex Shader
		"#version 330 core\n"
		"layout( location = 0 ) in vec3 vertex;\n"
		"layout( location = 1 ) in vec3 normal;\n"
		"uniform mat4 mv;\n"
		"uniform mat4 p;\n"
		"uniform mat3 nrm;\n"
		"out VS2FS { vec4 vertex, normal; } vs2fs;\n"
		"void main( void ) {\n"
			"vs2fs.vertex = p * mv * vec4( vertex, 1. );\n"
//			"vs2fs.normal = vec4( normalize( vec3( p * vec4( nrm * normal, 1. ) ) ), 0. );\n"
			"vs2fs.normal = vec4( normalize( nrm * normal ), 0. );\n"
			"gl_Position  = vs2fs.vertex;\n"
		"}\n",

		//Fragment Shader
		"#version 330 core\n"
		"in VS2FS { vec4 vertex, normal; } vs2fs;\n"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
//			"fColor = vec4( vec3( .125 * ( vs2fs.vertex.z - 0. ) ), 1. );\n"
			"fColor = vec4( ( .125 * ( vs2fs.vertex.z - 0. ) ) * vs2fs.vertex.xyz, 1. );\n"
//			"fColor = vec4( vec3( gl_FragCoord.z ), 1. );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "mv", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv ).
			addUniform( "p", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p ).
			addUniform( "nrm",  GLRenderer::Shader::MAT3, GLRenderer::Shader::SCALAR, & nrm );

	glr.program( "SPHERE-PROGRAM" ).
		setVertexArray( "SPHERE-VERTICES" ).
		setShader( "SPHERE-SHADER" ).
		build( );


	glr.shader(
		"NORMALS-SHADER",

		"#version 330 core\n"
		"precision highp float;\n"
		"precision highp int;\n"
		"in vec3 vertex;\n"
		"in vec3 normal;\n"
		"out VS2GS {\n"
			"vec4 vertex, normal;\n"
			"vec3 mvn;\n"
		"} vs2gs;\n"
		"uniform mat4 p;\n"
		"uniform mat4 mv;\n"
		"uniform mat3 nrm;\n"
		"void main( ) {\n"
			"vs2gs.vertex = ( p * mv * vec4( vertex, 1. ) );\n"
			"vs2gs.normal = vec4( normalize( vec3( p * vec4( nrm * normal, 1.0 ) ) ), 0. );\n"
			"vs2gs.mvn    = normalize( nrm * normal );\n"
			"gl_Position  = p * mv * vec4( vertex, 1. );\n"
		"}\n",

		"#version 330 core\n"
		"precision highp float;\n"
		"precision highp int;\n"
		"layout ( triangles ) in;\n"
		"layout ( line_strip, max_vertices = 6 ) out;\n"
		"in VS2GS {\n"
			"vec4 vertex;\n"
			"vec4 normal;\n"
			"vec3 mvn;\n"
		"} vs2gs[ ];\n"
		"out GS2FS {\n"
			"vec3 color;\n"
		"} gs2fs;\n"
		"const float MAGNITUDE = .3;\n"
		"void generateLine( int index ) {\n"
			"gl_Position = gl_in[ index ].gl_Position + 1.25 * vs2gs[ index ].normal * MAGNITUDE;\n"
			"gs2fs.color = max( vec3( 0. ), vs2gs[ index ].mvn );\n"
			"EmitVertex( );\n"
			"gl_Position = gl_in[ index ].gl_Position + 2.25 * vs2gs[ index ].normal * MAGNITUDE;\n"
			"gs2fs.color = max( vec3( 0. ), vs2gs[ index ].mvn );\n"
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
			"vec3 color;\n"
		"} gs2fs;\n"
		"out vec4 fColor;\n"
		"void main( void ) {\n"
//			"fColor = vec4( 1.f, 1.f, 1.f, 1.f );\n"
			"fColor = vec4( gs2fs.color, 1.f );\n"
//			"fColor = vec4( vec3( gl_FragCoord.z ), 1. );\n"
		"}\n",
		GLRenderer::ShaderCode::FROM_CODE ).
			addUniform( "p",  GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & p ).
			addUniform( "mv",  GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mv ).
			addUniform( "nrm",  GLRenderer::Shader::MAT3, GLRenderer::Shader::SCALAR, & nrm );

	glr.program( "NORMALS-PROGRAM" ).
		setVertexArray(	"SPHERE-VERTICES" ).
		setShader( "NORMALS-SHADER" ).
		build( );
	}

void
Sphere::paint( ) {

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

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	for( int i = 0; i < 5; ++ i ){

		for( int j = 0; j < 5; ++ j ){

			m = glm::translate( glm::mat4( 1.f ), glm::vec3( 5.f * sinf( 3.14 * i / 5. * angle ), 5.f * sinf( j * angle ), -5.f ) );

			mv = m * v;

			nrm = transpose( inverse( mv ) );

			glr.run( { "SPHERE-PROGRAM" } );
		}
	}

//	m = glm::translate( glm::mat4( 1.f ), glm::vec3( cosf( angle ), 0.f, 0.f ) );

//	mv = m * v;

//	nrm = transpose( inverse( mv ) );


//	glr.run( { "SPHERE-PROGRAM", "NORMALS-PROGRAM" } );

//	m = glm::translate( glm::mat4( 1.f ), glm::vec3( sinf( angle ), 0.f, 0.f ) );

//	mv = m * v;

//	nrm = transpose( inverse( mv ) );

//	glr.run( { "SPHERE-PROGRAM", "NORMALS-PROGRAM" } );
}

void
Sphere::resize( int p_width, int p_height ) {

	float
	ratio = ( 1.f * p_width / p_height );

	p = glm::perspective( 45.0f, ratio, 1.0f, 10.f );
}

