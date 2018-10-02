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
		"uniform float Ns;\n"
		"uniform float Ni;\n"
		"uniform float d;\n"
		"uniform vec3 Ka;\n"
		"uniform vec3 Kd;\n"
		"uniform vec3 Ks;\n"
		"uniform vec3 Ke;\n"
		"uniform sampler2D map_Kd;\n"
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
public Named,
public GLRRef {

	public :

		Material ( CStr & p_name, GLR & p_glr ) :
		Named ( p_name ),
		GLRRef ( p_glr ),
		Ns ( 1. ),
		Ni ( 1. ),
		d ( 1. ),
		illum( 2 ),
		Ka ( 0., 0., 0. ),
		Kd ( 0., 0., 0. ),
		Ks ( 0., 0., 0. ),
		Ke ( 0., 0., 0. ),
		map_Ka ( "" ),
		map_Kd ( "" ),
		map_Ks ( "" ),
		map_Ke ( "" ) {

		}

		~ Material ( ) {

			// delete shader;
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

		Str
		map_Ka,
		map_Kd,
		map_Ks,
		map_Ke;
	};

enum
TypeOfIndices {

	TOI_UNKOWN,
	TOI_VERTEX_ONLY,
	TOI_VERTEX_TEXTURE,
	TOI_VERTEX_NORMAL,
	TOI_VERTEX_TEXTURE_NORMAL
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

		std::vector < Str >
		surfaces;

		Str
		currObjektName,
		currGroupName,
		currMaterialName,
		currPathOrig,
		currPathObj;

		Material
		* currMaterial;

		GLR::Container
		* currContainer;

		GLR::VertexArray
		* currVertexArray;

		M4
		projection,
		view,
		model;

		V3
		lightPos;

	public :

		Scene
		& build ( ) {

/*
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
/*				va <<
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
*/



			return * this;
		}

		Scene
		& clear ( ) {
/*
			for ( auto o : objekts ) {

				delete o.second;
			}
*/
			return * this;
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

						currMaterial = new Material ( currMaterialName, glr ( ) );

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
					else if( arg == "map_Ka" ) {

						iss >> currMaterial->map_Ka;
					}
					else if( arg == "map_Ks" ) {

						iss >> currMaterial->map_Ks;
					}
					else if( arg == "map_Kd" ) {

						iss >> currMaterial->map_Kd;
					}
					else if( arg == "map_Ke" ) {

						iss >> currMaterial->map_Ke;
					}
				}
			}

			file.close ( );

			glr ( ).shader (
				"S-VVTVN",
				SHADERS[ 0 ][ 0 ],
				SHADERS[ 0 ][ 1 ],
				GLR::ShaderCode::FROM_CODE ).
				addUniform( "model",      GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
				addUniform( "view",       GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
				addUniform( "projection", GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection ).
				addUniform( "lightPos",   GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightPos ).
				addUniform( "Ka", GLR::Shader::VEC3, GLR::Shader::SCALAR, nullptr ).
				addUniform( "Kd", GLR::Shader::VEC3, GLR::Shader::SCALAR, nullptr ).
				addUniform( "Ks", GLR::Shader::VEC3, GLR::Shader::SCALAR, nullptr ).
				addUniform( "Ke", GLR::Shader::VEC3, GLR::Shader::SCALAR, nullptr ).
				addUniform( "Ns", GLR::Shader::FLOAT, GLR::Shader::SCALAR, nullptr ).
				addUniform( "Ni", GLR::Shader::FLOAT, GLR::Shader::SCALAR, nullptr ).
				addUniform( "d",  GLR::Shader::FLOAT, GLR::Shader::SCALAR, nullptr );

			for ( auto m : materials ) {

				if ( m.second->map_Kd != "" ) {

					glr ( ).texture(
						"T-" + m.second->name ( ),
						new GLR::Texture (
							"map_Kd",
						m.second->map_Kd ) );
				}
			}

			return true;
		}

		bool
		fromObjFile ( CStr & p_filename ) {

			currContainer = nullptr;
			currVertexArray = nullptr;

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

			GLuint
			firstVertexOfSurface = 0,
			vertexCount = 0;

			TypeOfIndices
			toi = TOI_UNKOWN;
//			GLuint
//			faceId = 0,
//			vertexId = 0,
//			normalId = 0,
//			textureId = 0;

//			GLuint
//			vMem[ 0x400 ],
//			vtMem[ 0x400 ],
//			vnMem[ 0x400 ];

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

						if ( ! currContainer ) {

							std::cout << "Sorry! But no container defined for faces. Exit ...\n";

							exit ( 1 );
						}

						if ( ! currVertexArray ) {

							std::cout << "Sorry! But no vertex array defined for storing vertices. Exit ...\n";

							exit ( 1 );
						}

						GLuint
						vId0, vId1, vId2,
						vtId0, vtId1, vtId2,
						vnId0, vnId1, vnId2;

						Str
						loc;

						iss >> loc;

						GLint
						firstSlash  = loc.find_first_of( "/" ),
						secondSlash = loc.find_first_of( "/", firstSlash + 1 );

						if ( toi == TOI_UNKOWN )
							toi = secondSlash < 0
								? firstSlash < 0
									? TOI_VERTEX_ONLY
									: TOI_VERTEX_TEXTURE
								: firstSlash == secondSlash - 1
									? TOI_VERTEX_NORMAL
									: TOI_VERTEX_TEXTURE_NORMAL;

						switch ( toi ) {

							case TOI_VERTEX_ONLY : {

								vId0 = std::stoi( loc );

								iss >> loc;
								vId1 = std::stoi( loc );

								iss >> loc;
								vId2 = std::stoi( loc );

								* currVertexArray <<
									v[ 3 * vId0 - 3 ] << v[ 3 * vId0 - 2 ] << v[ 3 * vId0 - 1 ] <<
									v[ 3 * vId1 - 3 ] << v[ 3 * vId1 - 2 ] << v[ 3 * vId1 - 1 ] <<
									v[ 3 * vId2 - 3 ] << v[ 3 * vId2 - 2 ] << v[ 3 * vId2 - 1 ];

								break;
							}
							case TOI_VERTEX_TEXTURE : {

								vId0  = std::stoi( loc.substr( 0, firstSlash ) );
								vtId0 = std::stoi( loc.substr( firstSlash + 1 ) );

								iss >> loc;
								vId1  = std::stoi( loc.substr( 0, firstSlash ) );
								vtId1 = std::stoi( loc.substr( firstSlash + 1 ) );

								iss >> loc;
								vId2  = std::stoi( loc.substr( 0, firstSlash ) );
								vtId2 = std::stoi( loc.substr( firstSlash + 1 ) );

								vId0 = 3 * vId0 - 3;
								vId1 = 3 * vId1 - 3;
								vId2 = 3 * vId2 - 3;

								vtId0 = 2 * vtId0 - 2;
								vtId1 = 2 * vtId1 - 2;

								* currVertexArray <<
									v[  vId0  ] << v[  vId0  + 1 ] << v[ vId0 + 2 ] <<
									vt[ vtId0 ] << vt[ vtId0 + 1 ] <<

									v[  vId1  ] << v[  vId1  + 1 ] << v[ vId1 + 2 ] <<
									vt[ vtId1 ] << vt[ vtId1 + 1 ] <<

									v[  vId2  ] << v[  vId2  + 1 ] << v[ vId2 + 2 ] <<
									vt[ vtId2 ] << vt[ vtId2 + 1 ];

									break;
								}
							case TOI_VERTEX_NORMAL : {

								vId0  = std::stoi( loc.substr( 0, firstSlash ) );
								vnId0 = std::stoi( loc.substr( secondSlash + 1 ) );

								iss >> loc;
								vId1  = std::stoi( loc.substr( 0, firstSlash ) );
								vnId1 = std::stoi( loc.substr( secondSlash + 1 ) );

								iss >> loc;
								vId2  = std::stoi( loc.substr( 0, firstSlash ) );
								vnId2 = std::stoi( loc.substr( secondSlash + 1 ) );

								vId0 = 3 * vId0 - 3;
								vId1 = 3 * vId1 - 3;
								vId2 = 3 * vId2 - 3;

								vnId0 = 3 * vnId0 - 3;
								vnId1 = 3 * vnId1 - 3;
								vnId2 = 3 * vnId2 - 3;

								* currVertexArray <<
									v[  vId0  ] << v[  vId0  + 1 ] << v[  vId0  + 2 ] <<
									vn[ vnId0 ] << vn[ vnId0 + 1 ] << vn[ vnId0 + 2 ] <<

									v[  vId1  ] << v[  vId1  + 1 ] << v[  vId1  + 2 ] <<
									vn[ vnId1 ] << vn[ vnId1 + 1 ] << vn[ vnId1 + 2 ] <<

									v[  vId2  ] << v[  vId2  + 1 ] << v[  vId2  + 2 ] <<
									vn[ vnId2 ] << vn[ vnId2 + 1 ] << vn[ vnId2 + 2 ];

								break;
							}
							case TOI_VERTEX_TEXTURE_NORMAL : {

								vId0  = std::stoi( loc.substr( 0, firstSlash ) );
								vtId0 = std::stoi( loc.substr( firstSlash + 1, secondSlash ) );
								vnId0 = std::stoi( loc.substr( secondSlash + 1 ) );

								iss >> loc;
								vId1  = std::stoi( loc.substr( 0, firstSlash ) );
								vtId1 = std::stoi( loc.substr( firstSlash + 1, secondSlash ) );
								vnId1 = std::stoi( loc.substr( secondSlash + 1 ) );

								iss >> loc;
								vId2  = std::stoi( loc.substr( 0, firstSlash ) );
								vtId2 = std::stoi( loc.substr( firstSlash + 1, secondSlash ) );
								vnId2 = std::stoi( loc.substr( secondSlash + 1 ) );

								vId0 = 3 * vId0 - 3;
								vId1 = 3 * vId1 - 3;
								vId2 = 3 * vId2 - 3;

								vtId0 = 2 * vtId0 - 2;
								vtId1 = 2 * vtId1 - 2;

								vnId0 = 3 * vnId0 - 3;
								vnId1 = 3 * vnId1 - 3;
								vnId2 = 3 * vnId2 - 3;

								* currVertexArray <<
									v[  vId0  ] << v[  vId0  + 1 ] << v[  vId0  + 2 ] <<
									vt[ vtId0 ] << vt[ vtId0 + 1 ] <<
									vn[ vnId0 ] << vn[ vnId0 + 1 ] << vn[ vnId0 + 2 ] <<

									v[  vId1  ] << v[  vId1  + 1 ] << v[  vId1  + 2 ] <<
									vt[ vtId1 ] << vt[ vtId1 + 1 ] <<
									vn[ vnId1 ] << vn[ vnId1 + 1 ] << vn[ vnId1 + 2 ] <<

									v[  vId2  ] << v[  vId2  + 1 ] << v[  vId2  + 2 ] <<
									vt[ vtId2 ] << vt[ vtId2 + 1 ] <<
									vn[ vnId2 ] << vn[ vnId2 + 1 ] << vn[ vnId2 + 2 ];

								break;
							}
							default : break;
						}

						vertexCount += 3;
					}
					else if( arg == "s" ) {

						GLint
						s = -1;

						iss >> s;

						std::cout << "smoothing factor: " << s << std::endl;
					}
					else if( arg == "o" ) {

						iss >> currObjektName;

						std::cout << "new object: " << currObjektName << std::endl;
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
						s;

						iss >> s;

						std::cout << "new surface: " << s << std::endl;

						if ( currContainer && currVertexArray ) {

							( * currVertexArray ) <<
								GLR::VertexArray::Object ( firstVertexOfSurface, vertexCount, GL_TRIANGLES );

							currVertexArray->attrib ( "v", 0, 3 );

							switch ( toi ) {

								case TOI_VERTEX_ONLY : {

									currContainer->setShader ( "S-V" );

									break;
								}
								case TOI_VERTEX_NORMAL : {

									currVertexArray->attrib ( "vn", 3, 3 );
									currContainer->setShader ( "S-VVN" );

									break;
								}
								case TOI_VERTEX_TEXTURE : {

									currVertexArray->attrib ( "vt", 3, 2 );
									currContainer->setShader ( "S-VVT" );

									break;
								}
								case TOI_VERTEX_TEXTURE_NORMAL : {

									currVertexArray->attrib ( "vt", 3, 2 );
									currVertexArray->attrib ( "vn", 5, 3 );
									currContainer->setShader ( "S-VVTVN" );

									break;
								}
								default : {

									break;
								}
							}

							currContainer->setVertexArray ( "V-" + currGroupName );

							currContainer->setShader ( "S-illum2" );

							firstVertexOfSurface += vertexCount;

							toi = TOI_UNKOWN;
						}
						else {

							firstVertexOfSurface = 0;
						}

						vertexCount = 0;

						currGroupName = currObjektName + s;

						surfaces.push_back ( "C-" + currGroupName );

						currContainer = & glr ( ).container ( "C-" + currGroupName );

						currVertexArray = & glr ( ).vertices ( "V-" + currGroupName );
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

			return surfaces;
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
