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
Material {

	public :

	Material ( ) {

	}

	~ Material ( ) {

	}

	public :

	V3
	ambient,
	diffuse,
	specular;

	public :

	bool
	fromMTLFile( CStr & p_filename ) {

		return true;
	}
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

		Material
		material;

		std::vector < std::size_t >
		vId,
		vtId,
		vnId;
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

		Str
		currObjektName,
		currGroupName;

		Objekt
		* currObjekt;

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

		bool
		fromObjFile ( CStr & p_filename ) {

			clear ( );

			std::ifstream
			file ( p_filename );

			if( ! file.is_open ( ) ) {

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
				iss ( line, std::istringstream::in );

				while ( iss >> arg ) {

					if ( arg == "mtllib" ) {

						iss >> mtllib;

						std::cout << "trying to load material file  " << mtllib << std::endl;

//						Material
//						m ( mtllib );
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

//						std::cout << "face[" << faceId ++ << "]:\n";

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

//								std::cout << "  vertex[" << vertexID ++ << "]:\n";

								std::string
								vStr  = sm[ 1 ].str ( ),
								vtStr = sm[ 2 ].str ( ),
								vnStr = sm[ 3 ].str ( );

//								std::cout <<
//									"    v:  " << vStr <<
//									"\n    vt: " << vtStr <<
//									"\n    vn: " << vnStr << std::endl;

								if ( ! currObjekt ) {

									std::cout <<
										"no Object defined for vertices.\n";

									exit ( 1 );
								}

								currObjekt->vId.push_back ( atoi ( vStr.c_str ( ) ) );
								currObjekt->vtId.push_back ( atoi ( vtStr.c_str ( ) ) );
								currObjekt->vnId.push_back ( atoi ( vnStr.c_str ( ) ) );
							}
							else if ( std::regex_search ( loc, sm, re_vn ) ) {

//								std::cout << "  vertex[" << vertexID ++ << "]:\n";

								std::string
								vStr  = sm[ 1 ].str( ),
								vnStr = sm[ 2 ].str( );

//								std::cout <<
//									"    v:  " << vStr <<
//									"\n    vn: " << vnStr << std::endl;

								if ( ! currObjekt ) {

									std::cout <<
										"no Object defined for vertices.\n";

									exit ( 1 );
								}

								currObjekt->vId.push_back ( atoi ( vStr.c_str ( ) ) );
								currObjekt->vnId.push_back ( atoi ( vnStr.c_str ( ) ) );
							}
							else if ( std::regex_search ( loc, sm, re_vt ) ) {

//								std::cout << "  vertex[" << vertexID ++ << "]:\n";

								std::string
								vStr  = sm[ 1 ].str( ),
								vtStr = sm[ 2 ].str( );

//								std::cout <<
//									"    v:  " << vStr <<
//									"\n    vt: " << vtStr << std::endl;

								if ( ! currObjekt ) {

									std::cout <<
										"no Object defined for vertices.\n";

									exit ( 1 );
								}

								currObjekt->vId.push_back ( atoi ( vStr.c_str ( ) ) );
								currObjekt->vtId.push_back ( atoi ( vtStr.c_str ( ) ) );
							}
							else {

//								std::cout << "  vertex[" << vertexID ++ << "]:\n";

								GLint
								i = atoi( loc.c_str ( ) );

//								std::cout << "    v: " << i << std::endl;

								if ( ! currObjekt ) {

									std::cout <<
										"no Object defined for vertices.\n";

									exit ( 1 );
								}

								currObjekt->vId.push_back ( i );

								while( iss >> i ) {

//									std::cout << "    v:  " << i << std::endl;

									currObjekt->vId.push_back ( i );
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

						currObjekt = new Objekt ( s, glr ( ) );

						objekts[ s ] = currObjekt;
					}
					else if( arg == "g" ) {

						Str
						s;

						iss >> s;

						std::cout << "object: " << s << std::endl;

						currObjekt = new Objekt ( s, glr ( ) );

						objekts[ s ] = currObjekt;
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

			build ( );

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
