#include "sceneexample.hpp"
#include "../../code/glmprinter.hpp"

/*
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

				std::cout << "v[" << vertexId ++ << "]:\n";

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

				std::cout << "vt[" << textureId ++ << "]:\n";

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

				std::cout << "vn[" << normalId ++ << "]\n";

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

				std::cout << "face[" << faceId ++ << "]:\n";

				std::regex
				re_v   ( "([0-9]+)[:space:]+([0-9]+)[:space:]+([0-9]+)" ),
				re_vt  ( "([0-9]+)[:space:]* /[:space:]*([0-9]+)" ),
				re_vn  ( "([0-9]+)[:space:]* /[:space:]* /[:space:]*([0-9]+)" ),
				re_vtn ( "([0-9]+)[:space:]* /[:space:]*([0-9]+)[:space:]* /[:space:]*([0-9]+)" );

				std::smatch
				sm;

				std::string
				loc;

				GLuint
				vertexID = 0;

				while( iss >> loc ) {

					if ( std::regex_search ( loc, sm, re_vtn ) ) {

						std::cout << "  vertex[" << vertexID ++ << "]:\n";

						std::string
						v  = sm[ 1 ].str( ),
						vt = sm[ 2 ].str( ),
						vn = sm[ 3 ].str( );

						std::cout << "    v:  " << v << "\n    vt: " << vt << "\n    vn: " << vn << std::endl;
					}
					else if ( std::regex_search ( loc, sm, re_vn ) ) {

						std::cout << "  vertex[" << vertexID ++ << "]:\n";

						std::string
						v  = sm[ 1 ].str( ),
						vn = sm[ 2 ].str( );

						std::cout << "    v:  " << v << "\n    vn: " << vn << std::endl;
					}
					else if ( std::regex_search ( loc, sm, re_vt ) ) {

						std::cout << "  vertex[" << vertexID ++ << "]:\n";

						std::string
						v  = sm[ 1 ].str( ),
						vt = sm[ 2 ].str( );

						std::cout << "    v:  " << v << "\n    vt: " << vt << std::endl;
					}
					else {

							std::cout << "  vertex[" << vertexID ++ << "]:\n";

						GLint
						i = atoi( loc.c_str ( ) );

						std::cout << "    v: " << i << std::endl;

						while( iss >> i ) {

							std::cout << "  vertex[" << vertexID ++ << "]:\n";
							std::cout << "    v:  " << i << std::endl;
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

				Str
				s;

				iss >> s;

				std::cout << "object: " << s << std::endl;
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
*/

SceneExample::SceneExample ( CStr & p_name, ViewControlData * p_vcd ) :
GLProject ( p_name, p_vcd ) {

	scene = new Scene ( "Scene", glr );
}

SceneExample::~ SceneExample ( ) {

	delete scene;
}

void
SceneExample::init ( ) {

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

	scene->fromObjFile ( "../obj/cube6Quads.obj" );

	glClearColor( .01f, .02f, .03f, 1. );

	scene->projection = scene->view = scene->model = glm::mat4 ( 1. );

	scene->model = glm::translate( scene->model, V3 ( 0, -2, 0 ) );
	scene->view  = glm::translate( scene->view,  V3 ( 0, 0, -4 ) );

	glEnable ( GL_DEPTH_TEST );
	glEnable ( GL_CULL_FACE );
}

void
SceneExample:: paint ( ) {

	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	GLR::CameraCenterView
	ccv( scene->model, scene->view, vcd, glm::vec3 ( .02f, .02f, .1f ) );

	ccv.reactOnMouse ( );

	ccv.rotate_around_y ( .001f );

	scene->model = ccv.model ( );

	scene->view  = ccv.view ( );

//	scene->lightPos = V3 ( scene->model * V4( V3 ( 0., 4., 2. ), 1 ) );
	scene->lightPos = V3 ( 2.f, 1.f, 0.f );

	glr.run ( scene->objektNames ( ) );
}

void
SceneExample:: resize ( int p_width, int p_height ) {

	// get aspect ratio
	float
	ratio = ( 1.f * p_width / p_height );

	// create a projection matrix
	scene->projection = glm::perspective( 30.f * 3.14159f / 180.f, ratio, 1.0f, 300.f );
}
