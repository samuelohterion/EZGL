#include "obj.hpp"
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

ObjExample::ObjExample ( CStr & p_name, ViewControlData * p_vcd ) :
GLProject ( p_name, p_vcd ),
obj1 ( 0x100000 ),
obj2 ( 0x1000000 ),
Kd( .2f, .4f, .8f ) {

}

ObjExample::~ ObjExample ( ) {

}

std::vector < std::string >
ObjExample::add2GLR ( OBJ & p_obj, std::string const & p_objName, std::string const & p_objPath, std::string const & p_pixPath ) {

	std::vector < std::string >
	contNames;

	p_obj.fromFile ( p_objPath + p_objName );
	for ( auto m : p_obj.mtl.materials ) {

		if ( m.second->map_Kd != "" ) {

			glr.texture(
				"T-" + m.second->map_Kd,
				new GLR::Texture (
					"map_Kd",
					 p_pixPath + m.second->map_Kd ) );
		}
	}
	for ( auto o : p_obj.objekts ) {

		for ( auto s : o.second->surfaces ) {

			std::string
			name = o.second->name ( ) + "-" + s.second->name ( ),
			vaName = "V-" + name,
			coName = "C-" + name;

			GLR::VertexArray
			& va = glr.vertices( vaName );

			GLR::Container
			& co = glr.container ( coName );

			contNames.push_back( coName );

			GLuint
			vCount = 0;

			switch ( s.second->toi ) {

				case TOI_VERTEX_ONLY : {

					va.attrib ( "v", 0, 3 );

					for ( auto id : s.second->idx ) {

						++ vCount;

						GLuint
						i = 3 * id - 3;

						va <<
							p_obj.v [ i + 0 ] << p_obj.v [ i + 1 ] << p_obj.v [ i + 2 ];
					}

					va << GLR::VertexArray::Object ( 0, vCount, GL_TRIANGLES );

					Material
					* mat = p_obj.mtl.materials[ s.second->mat ];

					glr.shader ( "S-" + name, SHADERS[ 0 ][ 0 ], SHADERS[ 0 ][ 1 ], GLR::ShaderCode::FROM_CODE ).
						addUniform ( "model",     GLR::Shader::MAT4,  GLR::Shader::SCALAR, & model ).
						addUniform ( "view",      GLR::Shader::MAT4,  GLR::Shader::SCALAR, & view ).
						addUniform ( "projection", GLR::Shader::MAT4,  GLR::Shader::SCALAR, & projection ).
//							addUniform ( "Kd",        GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Kd ).
//							addUniform ( "Ks",        GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ks ).
						addUniform ( "Ka",        GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ka );//.
//							addUniform ( "Ke",        GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ke ).
//							addUniform ( "Ni",        GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ni ).
//							addUniform ( "Ns",        GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ns ).
//							addUniform ( "d",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->d );

					break;
				}
				case TOI_VERTEX_TEXTURE : {

					std::size_t
					len = s.second->idx.size ( );

					for ( std::size_t id = 0; id < len; id += 2 ) {

						++ vCount;

						GLuint
						i = 3 * s.second->idx [ id + 0 ] - 3,
						j = 2 * s.second->idx [ id + 1 ] - 2;

						va <<
							p_obj.v  [ i + 0 ] << p_obj.v  [ i + 1 ] << p_obj.v [ i + 2 ] <<
							p_obj.vt [ j + 0 ] << p_obj.vt [ j + 1 ];
					}

					va.
					attrib ( "v",  0, 3 ).
					attrib ( "vt", 3, 2 );

					va << GLR::VertexArray::Object ( 0, vCount, GL_TRIANGLES );

					Material
					* mat = p_obj.mtl.materials[ s.second->mat ];

					glr.shader ( "S-" + name, SHADERS[ 1 ][ 0 ], SHADERS[ 1 ][ 1 ], GLR::ShaderCode::FROM_CODE ).
						addUniform ( "model",     GLR::Shader::MAT4,  GLR::Shader::SCALAR, & model ).
						addUniform ( "view",      GLR::Shader::MAT4,  GLR::Shader::SCALAR, & view ).
						addUniform ( "projection", GLR::Shader::MAT4,  GLR::Shader::SCALAR, & projection ).
						addUniform ( "lightPos",  GLR::Shader::VEC3,  GLR::Shader::SCALAR, & lightPos ).
						addUniform ( "Kd",        GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Kd ).
						addUniform ( "Ks",        GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ks ).
						addUniform ( "Ka",        GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ka ).
						addUniform ( "Ke",        GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ke ).
						addUniform ( "Ni",        GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ni ).
						addUniform ( "Ns",        GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ns ).
						addUniform ( "d",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->d ).
						addUniform ( "map_Kd",    GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->map_Kd );

					if ( mat->map_Kd != "" ) {

						co.addInTexture( "T-" + mat->map_Kd );
					}

					break;
				}
				case TOI_VERTEX_NORMAL : {

					va.
						setUsage( GL_STATIC_DRAW ).
						attrib ( "v",  0, 3 ).
						attrib ( "vn", 3, 3 );

					/*
					va <<
						-1.f << -1.f << +0.f << +0.f << +0.f << +1.f <<
						+1.f << -1.f << +0.f << +0.f << +0.f << +1.f <<
						+1.f << +1.f << +0.f << +0.f << +0.f << +1.f <<
						+1.f << +1.f << +0.f << +0.f << +0.f << +1.f <<
						-1.f << +1.f << +0.f << +0.f << +0.f << +1.f <<
						-1.f << -1.f << +0.f << +0.f << +0.f << +1.f <<
						GLR::VertexArray::Object ( 0, 6, GL_TRIANGLES );
					*/

					std::size_t
					len = s.second->idx.size ( );

					for ( std::size_t id = 0; id < len; id += 2 ) {

						++ vCount;

						GLuint
						i = 3 * s.second->idx [ id + 0 ] - 3,
						j = 3 * s.second->idx [ id + 1 ] - 3;

						va << p_obj.v  [ i + 0 ] << p_obj.v  [ i + 1 ] << p_obj.v  [ i + 2 ];
						va << p_obj.vn [ j + 0 ] << p_obj.vn [ j + 1 ] << p_obj.vn [ j + 2 ];

//							std::cout << "v:  " << obj.v  [ i + 0 ] << " " << obj.v  [ i + 1 ] << " " << obj.v  [ i + 2 ] << std::endl;
//							std::cout << "vn: " << obj.vn [ j + 0 ] << " " << obj.vn [ j + 1 ] << " " << obj.vn [ j + 2 ] << std::endl;
						std::cout << ".";
					}

					va << GLR::VertexArray::Object ( 0, vCount, GL_TRIANGLES );

					Material
					* mat = p_obj.mtl.materials[ s.second->mat ];

					glr.shader ( "S-" + name, SHADERS[ 2 ][ 0 ], SHADERS[ 2 ][ 1 ], GLR::ShaderCode::FROM_CODE ).
						addUniform ( "projection", GLR::Shader::MAT4,  GLR::Shader::SCALAR, & projection ).
						addUniform ( "view",       GLR::Shader::MAT4,  GLR::Shader::SCALAR, & view ).
						addUniform ( "model",      GLR::Shader::MAT4,  GLR::Shader::SCALAR, & model ).
						addUniform ( "lightPos",   GLR::Shader::VEC3,  GLR::Shader::SCALAR, & lightPos ).
						addUniform ( "Kd",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Kd ).
						addUniform ( "Ks",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ks ).
						addUniform ( "Ka",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ka ).
						addUniform ( "Ke",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ke ).
						addUniform ( "Ni",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ni ).
						addUniform ( "Ns",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ns ).
						addUniform ( "d",          GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->d );

					break;
				}
				case TOI_VERTEX_TEXTURE_NORMAL : {

					std::size_t
					len = s.second->idx.size ( );

					for ( std::size_t id = 0; id < len; id += 3 ) {

						++ vCount;

						GLuint
						i = 3 * s.second->idx [ id + 0 ] - 3,
						j = 2 * s.second->idx [ id + 1 ] - 2,
						k = 3 * s.second->idx [ id + 2 ] - 3;

						va <<
							p_obj.v  [ i + 0 ] << p_obj.v  [ i + 1 ] << p_obj.v  [ i + 2 ] <<
							p_obj.vt [ j + 0 ] << p_obj.vt [ j + 1 ] <<
							p_obj.vn [ k + 0 ] << p_obj.vn [ k + 1 ] << p_obj.vn [ k + 2 ];
					}

					va.
						attrib ( "v",  0, 3 ).
						attrib ( "vt", 3, 2 ).
						attrib ( "vn", 5, 3 );

					va << GLR::VertexArray::Object ( 0, vCount, GL_TRIANGLES );

					Material
					* mat = p_obj.mtl.materials[ s.second->mat ];

					glr.shader ( "S-" + name, SHADERS[ 3 ][ 0 ], SHADERS[ 3 ][ 1 ], GLR::ShaderCode::FROM_CODE ).
						addUniform ( "model",     GLR::Shader::MAT4,  GLR::Shader::SCALAR, & model ).
						addUniform ( "view",      GLR::Shader::MAT4,  GLR::Shader::SCALAR, & view ).
						addUniform ( "projection", GLR::Shader::MAT4,  GLR::Shader::SCALAR, & projection ).
						addUniform ( "lightPos",  GLR::Shader::VEC3,  GLR::Shader::SCALAR, & lightPos ).
						addUniform ( "Kd",        GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Kd ).
						addUniform ( "Ks",        GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ks ).
						addUniform ( "Ka",        GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ka ).
						addUniform ( "Ke",        GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ke ).
						addUniform ( "Ni",        GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ni ).
						addUniform ( "Ns",        GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ns ).
						addUniform ( "d",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->d ).
						addUniform ( "map_Kd",    GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->map_Kd );

					if ( mat->map_Kd != "" ) {

						co.addInTexture ( "T-" + mat->map_Kd );
					}

					break;
				}

				default : break;
			}

			co.
				setVertexArray ( "V-" + name ).
				setShader ( "S-" + name ).
				build ( );
		}
	}

	return contNames;
}

void
ObjExample::init ( ) {

//	obj1.fromFile ( "/home/friedrich/Downloads/models/audi000/audi000tri.obj" );
//	obj2.fromFile ( "/home/friedrich/Development/c++/2018.06.03/obj/surfaceSphereTri.obj" );

	contList1 = add2GLR( obj1, "audi000tri.obj", "/home/friedrich/Downloads/models/audi000/", "/home/friedrich/Downloads/models/audi000/" );
	contList2 = add2GLR( obj2, "rosawomantri.obj", "/home/friedrich/Development/c++/2018.06.03/obj/", "/home/friedrich/Development/c++/2018.06.03/pix/" );

	// frame buffer
	{
	}

	// vertex arrays
	{
		glr.
			vertices ( "V-LIGHT" ).
			attrib ( "v", 0, 3 ) <<
			+0.f << +0.f << +0.f <<
			GLR::VertexArray::Object ( 0, 1, GL_POINTS );
	}

	// index arrays
	{
	}

	// shaders
	{
		glr.
			shader (
				"S-LIGHT",

				"#version 330 core\n"
				"uniform mat4 projection;\n"
				"uniform mat4 view;\n"
				"uniform mat4 model;\n"
				"uniform vec3 lightPos;\n"
				"in vec3 v;\n"
				"void main ( ) {\n"
				"	gl_Position = projection * view * vec4( lightPos, 1 );\n"
				"}\n",

				"#version 330 core\n"
				"out vec4 fColor;"
				"void main ( ) {\n"
				"	fColor = vec4( 1, 1, 1, 1 );\n"
				"}\n",
				GLR::ShaderCode::FROM_CODE ).
			addUniform ( "projection", GLR::Shader::MAT4, GLR::Shader::SCALAR, & projection ).
			addUniform ( "view", GLR::Shader::MAT4, GLR::Shader::SCALAR, & view ).
			addUniform ( "model", GLR::Shader::MAT4, GLR::Shader::SCALAR, & model ).
			addUniform ( "lightPos", GLR::Shader::VEC3, GLR::Shader::SCALAR, & lightPos );
	}

	// textures
	{
	}

	// containers
	{
		glr.
			container ( "C-LIGHT" ).
			setVertexArray ( "V-LIGHT" ).
			setShader( "S-LIGHT" ).
			build ( );
	}

	projection = view = model = glm::mat4 ( 1. );

	model = glm::translate( model, V3 ( +0, +0, +0 ) );
	view  = glm::translate( view,  V3 ( +0, +0, -10 ) );

	glClearColor( .51f, .52f, .53f, 1. );

	glEnable ( GL_DEPTH_TEST );
	glEnable ( GL_CULL_FACE );
}

void
ObjExample:: paint ( ) {

	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	GLR::CameraCenterView
	ccv( model, view, vcd, glm::vec3 ( .02f, .02f, .1f ) );

	ccv.reactOnMouse ( );

	ccv.rotate_around_y ( .001f );

	model = ccv.model ( );

	view  = ccv.view ( );

//	scene->lightPos = V3 ( scene->model * V4( V3 ( 0., 4., 2. ), 1 ) );
	lightPos = V3( model * V4 ( 5.f * sinf( 1.f * vcd->time ), 3.f + 10.f * sinf( .1f * vcd->time ), + 5.f * cosf( 1.f * vcd->time ), 1. ) );

	M4
	tmp = model;

//	GLfloat
//	phase = 0.f;

//	for ( auto s : containerNames ) {

//		model = glm::translate ( tmp, V3 ( 1.f * sinf ( phase + 1.1 * vcd->time ), 1.f * sinf ( phase + 1.11 * vcd->time ), 1.f * sinf ( phase + 1.12 * vcd->time ) ) );

//		phase += .3f;

//		glr.run ( { s } );
//	}

//	model = tmp;

	model = glm::translate ( model, V3 ( 0.f, -10.f, 0.f ) );

	glr.run ( { "C-LIGHT" } );


	model = glm::scale ( model, V3 ( 5.f, 5.f, 5.f ) );
	glr.run ( contList2 );
	model = glm::scale ( model, V3 ( .2f, .2f, .2f ) );
	model = glm::rotate ( model, 1.57f, V3 ( 0.f, 1.f, 0.f ) );
	model = glm::translate ( model, V3 ( 0.f, +4.f, 0.f ) );

	glr.run ( contList1 );

	model = tmp;
}

void
ObjExample:: resize ( int p_width, int p_height ) {

	// get aspect ratio
	float
	ratio = ( 1.f * p_width / p_height );

	// create a projection matrix
	projection = glm::perspective( 30.f * 3.14159f / 180.f, ratio, 1.0f, 300.f );
}

