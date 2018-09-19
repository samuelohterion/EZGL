#ifndef OBJ_IMPORTER_HPP
#define OBJ_IMPORTER_HPP

#include "glr.hpp"
#include "glmprinter.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>

class
OBJImporter {

	public :

		class Material :
		public Named {

			public :

				Material ( CStr & p_name ) :
				Named( p_name ) {

				}

				~ Material ( ) {

				}

			public :

			bool
			load( CStr & p_filename ) {

				return false;
			}
		};

	public :

		OBJImporter ( GLR & p_glr ) :
		glr( p_glr ) {

		}

		~ OBJImporter ( ) {

		}

	public :

		GLR
		& glr;

		std::string
		mtllib;

		std::vector< GLfloat >
		v,
		vn,
		vt;

	public :

		bool
		load( CStr & p_filename ) {

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

						std::cout << "trying to load material file  " << mtllib << std::endl;

						Material
						m( mtllib );

						glr.
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
};

#endif // OBJ_IMPORTER
