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
/*
class
ObjLoaderMaterial {

	public :

		ObjLoaderMaterial ( ) {

		}

		~ ObjLoaderMaterial ( ) {

		}

	public :

		std::string
		filename;
}

class
ObjLoaderContent {

	public :

		class ObjLoaderObject :
		public Named {

			public :

				ObjLoaderObject ( CStr & p_name ) :
				Named( p_name ) {


				}

				~ ObjLoaderObject ( );

			public :

				Str
				texure;

				std::vector< GLfloat >
				v,
				vt,
				vn;

		};

	public :

		class
		Object {

			public :

				Object ( ) {

				}

				~ Object ( ) {

				}

			public :

				std::string
				name;

				std::map< std::string, std::string >
				object;
		};
};

class Obj :
public Named {

	public :

		CStr
		COMMENT_STR [ 1 ] {

			"#"	// comment
		};

		CStr
		VERTEX_DATA_STR [ 4 ] {

			"v",	// Geometric vertices
			"vt",	// Texture vertices
			"vn", // Vertex normals
			"vp" // Parameter space vertices
		};

		CStr
		FREE_FORM_CURVE_SURFACE_ATTRIBUTE_STR [ 4 ] {

			"deg",	// Degree
			"bmat",	// Basis matrix
			"step",	// Step size
			"cstype"	// Curve or surface type
		};

		CStr
		ELEMENTS_STR [ 6 ] {

			"p",		// Point
			"l",		// Line
			"f",		// Face
			"curv",	// Curve
			"curv2",	// 2D curve
			"surf"	// Surface
		};

		CStr
		FREE_FORM_CURVE_SURFACE_BODY_STATEMENT_STR [ 6 ] {

			"parm",	// Parameter values
			"trim",	// Outer trimming loop
			"hole",	// Inner trimming loop
			"scrv",	// Special curve
			"sp",		// Special point
			"end"		// End statement
		};

		CStr
		CONNECTIVITY_BETWEEN_FREE_FORM_SURFACES_STR [ 1 ] {

			"con"	// Connect
		};

		CStr
		GROUPING_STR [ 4 ] {

			"g",	// Group name
			"s",	// Smoothing group
			"mg",	// Merging group
			"o",	// Object name
		};

		CStr
		DISPLAY_RENDER_ATTRIBUTE_STR [ 10 ]{

			"bevel",		// Bevel interpolation
			"c_interp",	// Color interpolation
			"d_interp",	// Dissolve interpolation
			"lod",		// Level of detail
			"usemtl",		// Material name
			"mtllib",		// Material library
			"shadow_obj",	// Shadow casting
			"trace_obj",	//	Ray tracing
			"ctech",		// Curve approximation technique
			"stech"		// Surface approximation technique
		};

	public :

		Obj ( const CStr & p_name );

		~ Obj ( );

	public :

		std::string
		mtllib;

		std::vector< GLfloat >
		v,
		vn,
		vt;

	public :

		bool
		load( CStr & p_filename );
};
*/

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

class
Material :
public
Named {

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

enum
TypeOfIndices {

	TOI_UNKOWN,
	TOI_VERTEX_ONLY,
	TOI_VERTEX_TEXTURE,
	TOI_VERTEX_NORMAL,
	TOI_VERTEX_TEXTURE_NORMAL
};

class
Element {

	public :

		Element ( ) {
		}

	public :

		virtual CStr
		etype ( ) const {

			return "void";
		}
};

class
Line :
public
Element {

	public :

		Line ( GLuint * const p_vMem ) :
		Element ( ),
		from( p_vMem[ 0 ] ),
		to( p_vMem[ 1 ] ) {
		}

	public :

		GLuint
		from,
		to;

		CStr
		etype ( ) const {

			return "LINE";
		}
};

template < typename T >
void
copyMem ( T * p_dst, T * p_src, std::size_t const & p_size ) {

	T
	* end = p_dst + p_size;

	while ( p_dst != end ) {

		* p_dst = * p_src;
		++ p_dst;
		++ p_src;
	}
}

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

class
Face :
public
Element {

	public :

		Face ( TypeOfIndices const & p_toi ) :
		Element ( ),
		toi ( p_toi ) {
		}

	public :

		TypeOfIndices
		toi;

	public :

		CStr
		etype ( ) const {

			return "FACE";
		}

		virtual CStr
		ftype ( ) const {

			return "UNKNOWN";
		}
};

class
Triangle :
public
Face {

	public :

		Triangle ( GLuint * p_vMem, GLuint * p_vtMem, GLuint * p_vnMem, TypeOfIndices const & p_toi ) :
		Face ( p_toi ) {

			copyMem < GLuint, 3 > ( vId, p_vMem );

			if ( toi == TOI_VERTEX_TEXTURE || toi == TOI_VERTEX_TEXTURE_NORMAL ) {

				copyMem < GLuint, 3 > ( vtId, p_vtMem );
			}

			if ( toi == TOI_VERTEX_NORMAL || toi == TOI_VERTEX_TEXTURE_NORMAL ) {

				copyMem < GLuint, 3 > ( vnId, p_vnMem );
			}
		}

	public :

		GLuint
		vId[ 3 ],
		vtId[ 3 ],
		vnId[ 3 ];

	public :

		virtual CStr
		ftype ( ) const {

			return "TRIANGLE";
		}
};

class
Quad :
public
Face {

	public :

		Quad ( GLuint * p_vMem, GLuint * p_vtMem, GLuint * p_vnMem, TypeOfIndices const & p_toi ) :
		Face ( p_toi ) {

			copyMem < GLuint, 4 > ( vId, p_vMem );

			if ( toi == TOI_VERTEX_TEXTURE || toi == TOI_VERTEX_TEXTURE_NORMAL ) {

				copyMem < GLuint, 4 > ( vtId, p_vtMem );
			}

			if ( toi == TOI_VERTEX_NORMAL || toi == TOI_VERTEX_TEXTURE_NORMAL ) {

				copyMem < GLuint, 4 > ( vnId, p_vnMem );
			}
		}

	public :

		GLuint
		vId[ 4 ],
		vtId[ 4 ],
		vnId[ 4 ];

	public :

		CStr
		ftype ( ) const {

			return "QUAD";
		}
};

class
TriangleFan :
public
Face {

	public :

		TriangleFan (
			GLuint * p_vMem,
			GLuint * p_vtMem,
			GLuint * p_vnMem,
			std::size_t const & p_size,
			TypeOfIndices const & p_toi ) :
		Face ( p_toi ),
		vId ( new GLuint[ p_size ] ),
		vtId ( toi == TOI_VERTEX_TEXTURE || toi == TOI_VERTEX_TEXTURE_NORMAL ? new GLuint[ p_size ] : nullptr ),
		vnId ( toi == TOI_VERTEX_NORMAL  || toi == TOI_VERTEX_TEXTURE_NORMAL ? new GLuint[ p_size ] : nullptr ) {

			copyMem < GLuint > ( vId, p_vMem, p_size );

			if ( toi == TOI_VERTEX_TEXTURE || toi == TOI_VERTEX_TEXTURE_NORMAL ) {

				copyMem < GLuint > ( vtId, p_vtMem, p_size );
			}

			if ( toi == TOI_VERTEX_NORMAL || toi == TOI_VERTEX_TEXTURE_NORMAL ) {

				copyMem < GLuint > ( vnId, p_vnMem, p_size );
			}
		}

	public :

		CStr
		etype ( ) const {

			return "TRIANGLE_FAN";
		}

		GLuint
		* vId,
		* vtId,
		* vnId;
};


class
Objekt :
public
Named,
public
GLRRef {

	public :

		Objekt ( CStr & p_name, GLR & p_glr ) :
		Named ( p_name ),
		GLRRef ( p_glr ) {

		}

		~ Objekt ( ) {

		}

	public :

		M4
		model;

		Str
		materialName;

		std::vector < Element * >
		elements;
};

class
Scene :
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

		std::map < CStr, Objekt * >
		objekts;

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
