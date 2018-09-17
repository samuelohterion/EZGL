#include "objloader.hpp"
#include "../../code/glmprinter.hpp"

Obj:: Obj ( CStr const & p_name) :
Named ( p_name ) {

}

Obj:: ~ Obj( ) {

}

bool
Obj:: load ( CStr & p_filename ) {

	std::ifstream
	file( p_filename );

	if( ! file.is_open( ) ) {

		std::cout << "couldn't find file " << p_filename << ".\n exit...\n";

		return false;
	}

	GLuint
	faceId = 0,
	vertexId = 0,
	normalId = 0,
	textureId = 0;

	while ( file.good ( ) ) {

		std::string
		line,
		arg;

		std::getline ( file, line );

		std::istringstream
		iss( line, std::istringstream::in );

		while( iss >> arg ) {

			if ( arg == "mtllib" ) {

				iss >> mtllib;
				std::cout << "mtllib: " << mtllib << std::endl;
			}
			else if( arg == "v" ) {

				std::cout << "v: " << vertexId ++ << std::endl;

				GLfloat
				p;

				iss >> p;
				v.push_back( p );
				std::cout << "  x: " << p << std::endl;

				iss >> p;
				v.push_back( p );
				std::cout << "  y: " << p << std::endl;

				iss >> p;
				v.push_back( p );
				std::cout << "  z: " << p << std::endl;
			}
			else if( arg == "vt" ) {

				std::cout << "vt: " << textureId ++ << std::endl;

				GLfloat
				p;

				iss >> p;
				vt.push_back( p );
				std::cout << "  u: " << p << std::endl;

				iss >> p;
				vt.push_back( p );
				std::cout << "  v: " << p << std::endl;
			}
			else if( arg == "vn" ) {

				std::cout << "vn: " << normalId ++ << std::endl;

				GLfloat
				p;

				iss >> p;
				vn.push_back( p );
				std::cout << "  x: " << p << std::endl;

				iss >> p;
				vn.push_back( p );
				std::cout << "  y: " << p << std::endl;

				iss >> p;
				vn.push_back( p );
				std::cout << "  z: " << p << std::endl;
			}
			else if( arg == "f" ) {

				std::cout << "face: " << faceId ++ << std::endl;

				std::regex
				re_v   ( "([0-9]+)[:space:]+([0-9]+)[:space:]+([0-9]+)" ),
				re_vt  ( "([0-9]+)[:space:]*/[:space:]*([0-9]+)" ),
				re_vn  ( "([0-9]+)[:space:]*/[:space:]*/[:space:]*([0-9]+)" ),
				re_vtn ( "([0-9]+)[:space:]*/[:space:]*([0-9]+)[:space:]*/[:space:]*([0-9]+)" );

				std::smatch
				sm;

				std::string
				loc;

				GLuint
				vertexID = 0;

				while( iss >> loc ) {

					if ( std::regex_search ( loc, sm, re_vtn ) ) {

						std::cout << "  vertex: " << vertexID ++ << std::endl;

						std::string
						v  = sm[ 1 ].str( ),
						vt = sm[ 2 ].str( ),
						vn = sm[ 3 ].str( );

						std::cout << "    v: " << v << "\n    vt: " << vt << "\n    vn: " << vn << std::endl;
					}
					else if ( std::regex_search ( loc, sm, re_vn ) ) {

						std::cout << "  vertex: " << vertexID ++ << std::endl;

						std::string
						v  = sm[ 1 ].str( ),
						vn = sm[ 2 ].str( );

						std::cout << "    v: " << v << "\n    vn: " << vn << std::endl;
					}
					else if ( std::regex_search ( loc, sm, re_vt ) ) {

						std::cout << "  vertex: " << vertexID ++ << std::endl;

						std::string
						v  = sm[ 1 ].str( ),
						vt = sm[ 2 ].str( );

						std::cout << "    v: " << v << "\n    vt: " << vt << std::endl;
					}
					else {

						std::cout << "  vertex: " << vertexID ++ << std::endl;

						GLint
						i = atoi( loc.c_str ( ) );

						std::cout << "    v: " << i << std::endl;

						while( iss >> i ) {

							std::cout << "  vertex: " << vertexID ++ << std::endl;
							std::cout << "    v: " << i << std::endl;
						}
					}
				}
			}
			else if( arg == "s" ) {

				GLint
				s = -1;

				iss >> s;

				std::cout << "smoothing factor: " << s << std::endl;
			}
			else if( arg == "o" ) {

				GLint
				s = -1;

				iss >> s;

				std::cout << "smoothing factor: " << s << std::endl;
			}
			else if( arg == "#" ) {

				Str
				s = "comment";

				iss >> s;

				std::cout << "comment: " << s << std::endl;
			}
			else if( arg == "usemtl" ) {

				Str
				s = "no material";

				iss >> s;

				std::cout << "material: " << s << std::endl;
			}
		}
	}

	file.close ( );

	return true;
}

ObjLoader::ObjLoader ( const CStr & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

	obj = new Obj( p_name + "-001" );

	obj->load( "../EZGL/obj/cube.obj" );
}

ObjLoader::~ ObjLoader ( ) {

	delete obj;
}

void
ObjLoader::init( ) {

	// frame buffer
	{
	}

	// vertex arrays
	{
	}

	// index arrays
	{
	}

	// shaders
	{
	}

	// textures
	{
	}

	// containers
	{
	}

	glClearColor( .01f, .02f, .03f, 1. );

	projection = model = glm::mat4( 1. );

	//view = obj->view;

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
}

void
ObjLoader:: paint ( ) {

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void
ObjLoader:: resize( int p_width, int p_height ) {

	// get aspect ratio
	float
	ratio = ( 1.f * p_width / p_height );

	// create a projection matrix
	projection = glm::perspective( 30.f * 3.14159f / 180.f, ratio, 1.0f, 300.f );
}
