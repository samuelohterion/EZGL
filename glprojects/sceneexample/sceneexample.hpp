#ifndef SCENEEXAMPLE_HPP
#define SCENEEXAMPLE_HPP

#include "../../code/glr.hpp"
#include "../../code/glmprinter.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>
#include <unistd.h>

CStr
SHADERS [ 3 ][ 2 ] =
{
	// shaders v, vt, vn
	{
		// vertex shader
		"#version 330 core\n"
		"in vec3 v;\n"
		"in vec2 vt;\n"
		"in vec3 vn;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"out VS2FS {\n"
		"	vec3 v;\n"
		"	vec2 vt;\n"
		"	vec3 vn;\n"
		"} vs2fs;\n"

		"void main( ) {\n"
		"	vs2fs.v  = vec3( model * vec4( v, 1 ) );\n"
		"	vs2fs.vt = vt;\n"
		"	vs2fs.vn = normalize( vec3( vec4( vn, 0 ) * inverse( model ) ) );\n"
		"	gl_Position = projection * view * model * vec4( v, 1 );\n"
		"}\n",

		// fragment shader
		"#version 330 core\n"

		//	light in every space that makes sense to show their behavior
		"in VS2FS {\n"
		"	vec3 v;\n"
		"	vec2 vt;\n"
		"	vec3 vn;\n"
		"} vs2fs;\n"
		"uniform vec3 light;\n"
		"out vec4 fColor;\n"
		"void main( ) {\n"
		"	vec3 ray = light - vs2fs.v;\n"
		"	float amp = max( 0, dot( vs2fs.vn, ray ) * inversesqrt( dot( ray, ray ) ) );\n"
		"	fColor = vec4( amp * vec3( 1 ), 1 );\n"
		"}\n"
	},
	// shaders v, vn
	{
		// vertex shader
		"#version 330 core\n"
		"in vec3 v;\n"
		"in vec2 vn;\n"
		"out VS2FS {\n"
		"	vec4 v;\n"
		"	vec4 vn;\n"
		"} vs2fs;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 proj;\n"

		"void main( ) {\n"
		"	vs2fs.v  = model * vec4( v, 1 );\n"
		"	vs2fs.vn = vec4( vn, 0 ) * inverse( model );\n"
		"	gl_Position = projection * view * model * vec4( v, 1 );\n"
		"}\n",

		// fragment shader
		"#version 330 core\n"

		//	light in every space that makes sense to show their behavior
		"uniform float time;\n"
		"uniform sampler2D txSphere;\n"
		"in VS2FS {\n"
		"	vec4 v;\n"
		"	vec4 vn;\n"
		"} vs2fs;\n"
		"out vec4 fColor;\n"
		"void main( ) {\n"
		"	fColor = vec4( 1 );\n"
		"}\n"
	},
	// shaders v, vn
	{
		// vertex shader
		"#version 330 core\n"
		"in vec3 v;\n"
		"in vec2 vn;\n"
		"out VS2FS {\n"
		"	vec4 v;\n"
		"	vec4 vn;\n"
		"} vs2fs;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 proj;\n"

		"void main( ) {\n"
		"	vs2fs.v  = model * vec4( v, 1 );\n"
		"	vs2fs.vn = vec4( vn, 0 ) * inverse( model );\n"
		"	gl_Position = projection * view * model * vec4( v, 1 );\n"
		"}\n",

		// fragment shader
		"#version 330 core\n"

		//	light in every space that makes sense to show their behavior
		"uniform float time;\n"
		"uniform sampler2D txSphere;\n"
		"in VS2FS {\n"
		"	vec4 v;\n"
		"	vec4 vn;\n"
		"} vs2fs;\n"
		"out vec4 fColor;\n"
		"void main( ) {\n"
		"	fColor = vec4( 1 );\n"
		"}\n"
	}
};

class Material :
public Named {

	public :

		Material ( CStr & p_name ) :
		Named( p_name ) {

		}

		~ Material ( ) {

	}

	public :

		GLfloat
		Ns,
		Ni,
		d,
		illum;

		V3
		Ka,
		Kd,
		Ks,
		Ke;
};

template < typename T, unsigned int S >
void
copyMem ( T * p_dst, T * p_src ) {

	T
	* end = p_dst + S;

	while ( p_dst != end ) {

		* p_dst = * p_src;
		++ p_dst;
		++ p_src;
	}
}


class Scene :
public Named,
public GLRRef {

	public :

		Scene ( CStr & p_name, GLR & p_glr ) :
		Named ( p_name ),
		GLRRef ( p_glr ),
		lightPos( 1, 1, 1 ) {

		}

		~ Scene ( ) {

			clear ( );
		}

	public :

		std::string
		mtllib;

		std::vector< GLfloat >
		v,
		vn,
		vt;

		std::map < CStr, Material * >
		materials;

		Str
		currObjektName,
		currGroupName,
		currMaterialName,
		currPathOrig,
		currPathObj;

		Objekt
		* currObjekt;

		Material
		* currMaterial;

		M4
		projection,
		view,
		model;

		V3
		lightPos;

	public :

		Scene
		& build ( ) {

			for( auto o : objekts ) {

				Objekt
				& obj = * o.second;

				Str
				objName = "C-" + obj.name ( );

				GLR::Container
				& co = glr ( ).container ( objName );

				Str
				vaName = "C-" + obj.name ( );

				GLR::VertexArray
				& va = glr ( ).vertices ( vaName );
/*
				if ( 0 < obj.vnId.size ( ) ) {

					if ( 0 < obj.vtId.size ( ) ) {

						va.
							attrib ( "v",  0, 3 ).
							attrib ( "vt", 3, 2 ).
							attrib ( "vn", 5, 3 );

						for ( std::size_t i = 0; i < obj.vId.size ( ); ++ i ) {

							va <<
								 v[ 3 * obj.vId[ i ] - 3 ]  <<  v[ 3 * obj.vId[ i ] - 2 ]  <<  v[ 3 * obj.vId[ i ] - 1 ] <<
								vt[ 2 * obj.vtId[ i ] - 2 ] << vt[ 2 * obj.vtId[ i ] - 1 ] <<
								vn[ 3 * obj.vnId[ i ] - 3 ] << vn[ 3 * obj.vnId[ i ] - 2 ] << vn[ 3 * obj.vnId[ i ] - 1 ];
						}
					}
					else {

						va.
							attrib ( "v",  0, 3 ).
							attrib ( "vn", 3, 3 );

						for ( std::size_t i = 0; i < obj.vId.size ( ); ++ i ) {

							va <<
								 v[ 3 * obj.vId[ i ] - 3 ]  <<  v[ 3 * obj.vId[ i ] - 2 ]  <<  v[ 3 * obj.vId[ i ] - 1 ] <<
								vn[ 3 * obj.vnId[ i ] - 3 ] << vn[ 3 * obj.vnId[ i ] - 2 ] << vn[ 3 * obj.vnId[ i ] - 1 ];
						}
					}
				}
				else {

					if ( 0 < obj.vtId.size ( ) ) {

						va.
							attrib ( "v",  0, 3 ).
							attrib ( "vt", 3, 2 );

						for ( std::size_t i = 0; i < obj.vId.size ( ); ++ i ) {

							va <<
								 v[ 3 * obj.vId[ i ] - 3 ]  <<  v[ 3 * obj.vId[ i ] - 2 ]  <<  v[ 3 * obj.vId[ i ] - 1 ] <<
								vt[ 2 * obj.vtId[ i ] - 2 ] << vt[ 2 * obj.vtId[ i ] - 1 ];
						}
					}
					else {

						va.
							attrib ( "v",  0, 3 );

						for ( std::size_t i = 0; i < obj.vId.size ( ); ++ i ) {

							va <<
								 v[ 3 * obj.vId[ i ] - 3 ]  <<  v[ 3 * obj.vId[ i ] - 2 ]  <<  v[ 3 * obj.vId[ i ] - 1 ];
						}
					}
				}
*/
				va <<
					GLR::VertexArray::Object( 0, va.vertexCount ( ), GL_TRIANGLES );

				Str
				shaderName = "S-" + obj.name ( );

				glr ( ).shader (

					shaderName,
					SHADERS[ 0 ][ 0 ],
					SHADERS[ 0 ][ 1 ],
					GLR::ShaderCode::FROM_CODE
				 ).
				 addUniform( "model", GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
				 addUniform( "view", GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
				 addUniform( "projection", GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection ).
				 addUniform( "light", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightPos );

				co.
					setVertexArray ( vaName ).
					setShader ( shaderName ).
					build ( );
			}

			return * this;
		}

		Scene
		& clear ( ) {

			for ( auto o : objekts ) {

				delete o.second;
			}

			return * this;
		}

		void
		changeDir ( CStr & p_path ) const {

			chdir ( p_path.c_str ( ) );
		}

		Str
		currentPath ( ) const {

			char
			fn[ 0x200 ];

			getcwd( fn, 0x200 );

			return Str ( fn );
		}

		void
		extractPath ( Str & p_path, CStr & p_filename ) const {

			std::size_t
			lof = p_filename.find_last_of( "/\\" );

			p_path = p_filename.substr( 0, lof );
		}

		void
		extractPathAndFile ( Str & p_path, Str & p_file, CStr & p_filename ) const {

			std::size_t
			lof = p_filename.find_last_of( "/\\" );

			p_path = p_filename.substr( 0, lof );
			p_file = p_filename.substr( lof + 1 );
		}

		bool
		fromMTLFile ( CStr & p_filename ) {

			std::ifstream
			file ( p_filename );

			if ( ! file.is_open ( ) ) {

				std::cout << "Couldn't find file " << p_filename << ".\n exit...\n";

				return false;
			}

			while ( file.good ( ) ) {

				std::string
				line,
				arg;

				std::getline ( file, line );

				std::istringstream
				iss ( line, std::istringstream::in );

				while ( iss >> arg ) {

					if ( arg == "newmtl" ) {

						iss >> currMaterialName;

						std::cout << "new material " << currMaterialName << std::endl;

						currMaterial = new Material ( currMaterialName );

						materials[ currMaterialName ] = currMaterial;
					}
					else if( arg == "#" ) {

						Str
						s = "comment";

						std::cout << "comment:";

						while( iss >> s )

							std::cout << " " << s;
					}
					else if( arg == "Ns" ) {

						iss >> currMaterial->Ns;
					}
					else if( arg == "Ni" ) {

						iss >> currMaterial->Ni;
					}
					else if( arg == "d" ) {

						iss >> currMaterial->d;
					}
					else if( arg == "illum" ) {

						iss >> currMaterial->illum;
					}
					else if( arg == "Ka" ) {

						iss >> currMaterial->Ka.x;
						iss >> currMaterial->Ka.y;
						iss >> currMaterial->Ka.z;
					}
					else if( arg == "Ks" ) {

						iss >> currMaterial->Ks.x;
						iss >> currMaterial->Ks.y;
						iss >> currMaterial->Ks.z;
					}
					else if( arg == "Kd" ) {

						iss >> currMaterial->Kd.x;
						iss >> currMaterial->Kd.y;
						iss >> currMaterial->Kd.z;
					}
					else if( arg == "Ke" ) {

						iss >> currMaterial->Ke.x;
						iss >> currMaterial->Ke.y;
						iss >> currMaterial->Ke.z;
					}
				}
			}

			file.close ( );

			build ( );

			return true;
		}

		bool
		fromObjFile ( CStr & p_filename ) {

			Str
			pathToObj,
			fileStrObj;

			currPathOrig = currentPath ( );

			extractPathAndFile( pathToObj, fileStrObj, p_filename );

			changeDir ( pathToObj );

			currPathObj = currentPath ( );

			clear ( );

			std::ifstream
			file ( p_filename );

			if( ! file.is_open ( ) ) {

				std::cout << "couldn't find file " << p_filename << ".\n exit...\n";

				changeDir ( currPathOrig );

				return false;
			}

//			GLuint
//			faceId = 0,
//			vertexId = 0,
//			normalId = 0,
//			textureId = 0;

			GLuint
			vMem[ 0x400 ],
			vtMem[ 0x400 ],
			vnMem[ 0x400 ];

			while ( file.good ( ) ) {

				std::string
				line,
				arg;

				std::getline ( file, line );

				std::istringstream
				iss ( line, std::istringstream::in );

				while ( iss >> arg ) {

					if ( arg == "mtllib" ) {

						iss >> mtllib;

						std::cout << "trying to load material file  " << mtllib << std::endl;

						if( ! fromMTLFile( mtllib ) ) {

							std::cout << "material file  " << mtllib << " not found. " << std::endl;

							changeDir ( currPathOrig );

							return false;
						}
					}
					else if( arg == "v" ) {

//						std::cout << "v[" << vertexId ++ << "]:\n";

						GLfloat
						p;

						iss >> p;
						v.push_back( p );
//						std::cout << "  x: " << p << std::endl;

						iss >> p;
						v.push_back( p );
//						std::cout << "  y: " << p << std::endl;

						iss >> p;
						v.push_back( p );
//						std::cout << "  z: " << p << std::endl;
					}
					else if( arg == "vt" ) {

//						std::cout << "vt[" << textureId ++ << "]:\n";

						GLfloat
						p;

						iss >> p;
						vt.push_back( p );
//						std::cout << "  u: " << p << std::endl;

						iss >> p;
						vt.push_back( p );
//						std::cout << "  v: " << p << std::endl;
					}
					else if( arg == "vn" ) {

//						std::cout << "vn[" << normalId ++ << "]\n";

						GLfloat
						p;

						iss >> p;
						vn.push_back( p );
//						std::cout << "  x: " << p << std::endl;

						iss >> p;
						vn.push_back( p );
//						std::cout << "  y: " << p << std::endl;

						iss >> p;
						vn.push_back( p );
//						std::cout << "  z: " << p << std::endl;
					}
					else if( arg == "f" ) {

						if ( ! currObjekt ) {

							std::cout << "Sorry! But no Object defined for vertices. Exit ...\n";

							exit ( 1 );
						}

						GLuint
						vertexID = 0;

						Str
						loc;

						TypeOfIndices
						toi = TOI_UNKOWN;

						while ( iss >> loc ) {

							GLint
							firstSlash  = loc.find_first_of( "/" ),
							secondSlash = loc.find_first_of( "/", firstSlash + 1 );

							if ( vertexID == 0 )
								toi =
									secondSlash < 0
										? firstSlash < 0
											? TOI_VERTEX_ONLY
											: TOI_VERTEX_TEXTURE
										: firstSlash == secondSlash - 1
											? TOI_VERTEX_NORMAL
											: TOI_VERTEX_TEXTURE_NORMAL;

							switch ( toi ) {

								case TOI_VERTEX_ONLY : {

									vMem[ vertexID ] = std::stoi( loc.substr( 0, firstSlash ) );

									break;
								}
								case TOI_VERTEX_TEXTURE : {

									vMem[ vertexID ]  = std::stoi( loc.substr( 0, firstSlash ) );
									vtMem[ vertexID ] = std::stoi( loc.substr( firstSlash + 1 ) );

									break;
								}
								case TOI_VERTEX_NORMAL : {

									vMem[ vertexID ] = std::stoi( loc.substr( 0, firstSlash ) );
									vnMem[ vertexID ] = std::stoi( loc.substr( secondSlash ) );

									break;
								}
								case TOI_VERTEX_TEXTURE_NORMAL : {

									vMem[ vertexID ] = std::stoi( loc.substr( 0, firstSlash ) );
									vtMem[ vertexID ] = std::stoi( loc.substr( firstSlash + 1 ) );
									vnMem[ vertexID ] = std::stoi( loc.substr( secondSlash ) );

									break;
								}

								default : break;
							}

							vertexID ++;
						}

						if ( vertexID < 4 ) {

							currObjekt->elements.push_back( new Triangle( vMem, vtMem, vnMem, toi ) );
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

						currObjekt = new Objekt ( s, glr ( ) );

						objekts[ s ] = currObjekt;
					}
/*					else if( arg == "g" ) {

						Str
						s;

						iss >> s;

						std::cout << "object: " << s << std::endl;

						currObjekt = new Objekt ( s, glr ( ) );

						objekts[ s ] = currObjekt;
					}
*/
					else if( arg == "#" ) {

						Str
						s = "comment";

						std::cout << "comment:";

						while( iss >> s )

							std::cout << " " << s;
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

			build ( );

			changeDir ( currPathOrig );

			return true;
		}

		std::vector < Str >
		objektNames ( ) {

			std::vector < Str >
			r;

			for ( auto o : objekts ) {

				r.push_back( "C-" + o.second->name ( ) );
			}

			return r;
		}
};

class SceneExample :
public GLProject {

	public:

		SceneExample ( CStr & p_name = "SceneExample", ViewControlData * p_vcd = nullptr );

		~ SceneExample ( );

	public:

		Scene
		* scene;

		public:

		void
		init ( );

		void
		paint ( );

		void
		resize ( int p_width, int p_height );
};

#endif // SCENEEXAMPLE_HPP
