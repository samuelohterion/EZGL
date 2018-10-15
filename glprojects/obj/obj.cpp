#include "obj.hpp"
#include "../../code/glmprinter.hpp"

ObjExample::ObjExample ( CStr & p_name ) :
GLProject ( p_name ),
obj1 ( 0x100000 ),
obj2 ( 0x1000000 ),
Kd( .2f, .4f, .8f ) {

}

ObjExample::~ ObjExample ( ) {

}

std::vector < std::vector < std::string > >
ObjExample::add2GLR ( OBJ & p_obj, std::string const & p_objName, std::string const & p_objPath, std::string const & p_pixPath ) {

	std::vector < std::vector < std::string > >
	contNames;

	contNames.push_back ( std::vector < std::string > ( ) );
	contNames.push_back ( std::vector < std::string > ( ) );

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

					if ( .999f < mat->d ) {

						glr.shader (
							"S-PASS1-" + name,
							SHADERS[ PASSES_1 ][ SA_V ][ ST_VERTEX ],
							SHADERS[ PASSES_1 ][ SA_V ][ ST_FRAGMENT ],
							GLR::ShaderCode::FROM_CODE ).
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

						co.setShader ( "S-PASS1-" + name );

						contNames[ PASSES_1 ].push_back( coName );
					}
					else {

						glr.shader (
							"S-PASS2-" + name,
							SHADERS[ PASSES_2 ][ SA_V ][ ST_VERTEX ],
							SHADERS[ PASSES_2 ][ SA_V ][ ST_FRAGMENT ],
							GLR::ShaderCode::FROM_CODE ).
							addUniform ( "model",      GLR::Shader::MAT4,  GLR::Shader::SCALAR, & model ).
							addUniform ( "view",       GLR::Shader::MAT4,  GLR::Shader::SCALAR, & view ).
							addUniform ( "projection", GLR::Shader::MAT4,  GLR::Shader::SCALAR, & projection ).
							addUniform ( "lightPos",   GLR::Shader::VEC3,  GLR::Shader::SCALAR, & lightPos ).
							addUniform ( "Kd",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Kd ).
							addUniform ( "Ks",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ks ).
							addUniform ( "Ka",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ka ).
							addUniform ( "Ke",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ke ).
							addUniform ( "Ni",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ni ).
							addUniform ( "Ns",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ns ).
							addUniform ( "d",          GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->d );

						co.addInTexture ( "T-PASS1-COL" );

						co.setShader ( "S-PASS2-" + name );

						contNames[ PASSES_2 ].push_back ( coName );
					}

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

					if ( .999f < mat->d ) {

						glr.shader (
							"S-PASS1-" + name,
							SHADERS[ PASSES_1 ][ SA_VVT ][ ST_VERTEX ],
							SHADERS[ PASSES_1 ][ SA_VVT ][ ST_FRAGMENT ],
							GLR::ShaderCode::FROM_CODE ).
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

						co.setShader ( "S-PASS1-" + name );

						contNames[ PASSES_1 ].push_back( coName );
					}
					else {

						glr.shader (
							"S-PASS2-" + name,
							SHADERS[ PASSES_2 ][ SA_VVT ][ ST_VERTEX ],
							SHADERS[ PASSES_2 ][ SA_VVT ][ ST_FRAGMENT ],
							GLR::ShaderCode::FROM_CODE ).
							addUniform ( "model",      GLR::Shader::MAT4,  GLR::Shader::SCALAR, & model ).
							addUniform ( "view",       GLR::Shader::MAT4,  GLR::Shader::SCALAR, & view ).
							addUniform ( "projection", GLR::Shader::MAT4,  GLR::Shader::SCALAR, & projection ).
							addUniform ( "lightPos",   GLR::Shader::VEC3,  GLR::Shader::SCALAR, & lightPos ).
							addUniform ( "Kd",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Kd ).
							addUniform ( "Ks",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ks ).
							addUniform ( "Ka",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ka ).
							addUniform ( "Ke",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ke ).
							addUniform ( "Ni",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ni ).
							addUniform ( "Ns",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ns ).
							addUniform ( "d",          GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->d );

						co.addInTexture ( "T-PASS1-COL" );

						co.setShader ( "S-PASS2-" + name );

						contNames[ PASSES_2 ].push_back ( coName );
					}

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
						//std::cout << ".";
					}

					va << GLR::VertexArray::Object ( 0, vCount, GL_TRIANGLES );

					Material
					* mat = p_obj.mtl.materials[ s.second->mat ];

					if ( .999f < mat->d ) {

						glr.shader (
							"S-PASS1-" + name,
							SHADERS[ PASSES_1 ][ SA_VVN ][ ST_VERTEX ],
							SHADERS[ PASSES_1 ][ SA_VVN ][ ST_FRAGMENT ],
							GLR::ShaderCode::FROM_CODE ).
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

						co.setShader ( "S-PASS1-" + name );

						contNames[ PASSES_1 ].push_back( coName );
					}
					else {

						glr.shader (
							"S-PASS2-" + name,
							SHADERS[ PASSES_2 ][ SA_VVN ][ ST_VERTEX ],
							SHADERS[ PASSES_2 ][ SA_VVN ][ ST_FRAGMENT ],
							GLR::ShaderCode::FROM_CODE ).
							addUniform ( "model",      GLR::Shader::MAT4,  GLR::Shader::SCALAR, & model ).
							addUniform ( "view",       GLR::Shader::MAT4,  GLR::Shader::SCALAR, & view ).
							addUniform ( "projection", GLR::Shader::MAT4,  GLR::Shader::SCALAR, & projection ).
							addUniform ( "lightPos",   GLR::Shader::VEC3,  GLR::Shader::SCALAR, & lightPos ).
							addUniform ( "Kd",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Kd ).
							addUniform ( "Ks",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ks ).
							addUniform ( "Ka",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ka ).
							addUniform ( "Ke",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ke ).
							addUniform ( "Ni",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ni ).
							addUniform ( "Ns",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ns ).
							addUniform ( "d",          GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->d );

						co.addInTexture ( "T-PASS1-COL" );

						co.setShader ( "S-PASS2-" + name );

						contNames[ PASSES_2 ].push_back ( coName );
					}

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

					if ( .999f < mat->d ) {

						glr.shader (
							"S-PASS1-" + name,
							SHADERS[ PASSES_1 ][ SA_VVTVN ][ ST_VERTEX ],
							SHADERS[ PASSES_1 ][ SA_VVTVN ][ ST_FRAGMENT ],
							GLR::ShaderCode::FROM_CODE ).
							addUniform ( "model",      GLR::Shader::MAT4,  GLR::Shader::SCALAR, & model ).
							addUniform ( "view",       GLR::Shader::MAT4,  GLR::Shader::SCALAR, & view ).
							addUniform ( "projection", GLR::Shader::MAT4,  GLR::Shader::SCALAR, & projection ).
							addUniform ( "lightPos",   GLR::Shader::VEC3,  GLR::Shader::SCALAR, & lightPos ).
							addUniform ( "Kd",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Kd ).
							addUniform ( "Ks",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ks ).
							addUniform ( "Ka",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ka ).
							addUniform ( "Ke",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ke ).
							addUniform ( "Ni",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ni ).
							addUniform ( "Ns",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ns ).
							addUniform ( "d",          GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->d );

						co.setShader ( "S-PASS1-" + name );

						contNames[ PASSES_1 ].push_back( coName );
					}
					else {

						glr.shader (
							"S-PASS2-" + name,
							SHADERS[ PASSES_2 ][ SA_VVTVN ][ ST_VERTEX ],
							SHADERS[ PASSES_2 ][ SA_VVTVN ][ ST_FRAGMENT ],
							GLR::ShaderCode::FROM_CODE ).
							addUniform ( "model",      GLR::Shader::MAT4,  GLR::Shader::SCALAR, & model ).
							addUniform ( "view",       GLR::Shader::MAT4,  GLR::Shader::SCALAR, & view ).
							addUniform ( "projection", GLR::Shader::MAT4,  GLR::Shader::SCALAR, & projection ).
							addUniform ( "lightPos",   GLR::Shader::VEC3,  GLR::Shader::SCALAR, & lightPos ).
							addUniform ( "Kd",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Kd ).
							addUniform ( "Ks",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ks ).
							addUniform ( "Ka",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ka ).
							addUniform ( "Ke",         GLR::Shader::VEC3,  GLR::Shader::SCALAR, & mat->Ke ).
							addUniform ( "Ni",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ni ).
							addUniform ( "Ns",         GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->Ns ).
							addUniform ( "d",          GLR::Shader::FLOAT, GLR::Shader::SCALAR, & mat->d );

						co.addInTexture ( "T-PASS1-COL" );

						co.setShader ( "S-PASS2-" + name );

						contNames[ PASSES_2 ].push_back ( coName );
					}

					if ( mat->map_Kd != "" ) {

						co.addInTexture ( "T-" + mat->map_Kd );
					}

					break;
				}

				default : break;
			}

			co.
				setVertexArray ( "V-" + name ).
				build ( );
		}
	}

	return contNames;
}

void
ObjExample::init ( ) {

	// offscreen
	{
		glr.
			createOffScreen ( ).
			addOutTexture ( "T-PASS1-COL" ).
			addOutTexture ( "T-PASS1-Z" );
	}

	// vertex arrays
	{
		// V-LIGHT
		{
			glr.
				vertices ( "V-LIGHT" ).
				attrib ( "v", 0, 3 ) <<
				-.1f << -.1f << +0.f <<
				+.1f << -.1f << +0.f <<
				+.1f << +.1f << +0.f <<
				-.1f << +.1f << +0.f <<
				GLR::VertexArray::Object ( 0, 4, GL_TRIANGLE_FAN );
		}
		// V-PASS2
		{
			glr.
				vertices ( "V-PASS2" ).
				attrib ( "v", 0, 2 ) <<
				-1.f << -1.f <<
				+1.f << -1.f <<
				+1.f << +1.f <<
				-1.f << +1.f <<
				GLR::VertexArray::Object ( 0, 4, GL_TRIANGLE_FAN );
		}
	}

	// index arrays
	{
	}

	// shaders
	{
		// S-LIGHT
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
	//				"	gl_PointSize = 10.;\n"
					"	gl_Position = projection * view * vec4( lightPos + v, 1 );\n"
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
		// S-PASS2
		{
			glr.
				shader (
					"S-PASS2",

					"#version 330 core\n"
					"in vec2 v;\n"
					"out VS2FS {\n"
					"	vec2 tx;\n"
					"} vs2fs;\n"
					"void main ( ) {\n"
					"	gl_Position = vec4 ( v, 0, 1 );\n"
					"	vs2fs.tx = .5 + .5 * v;\n"
					"}\n",

					"#version 330 core\n"
					"in VS2FS {\n"
					"	vec2 tx;\n"
					"} vs2fs;\n"
					"uniform sampler2D pass1C;\n"
					"out vec4 fColor;"
					"void main ( ) {\n"
					"	fColor = texture( pass1C, vs2fs.tx );\n"
					"}\n",
					GLR::ShaderCode::FROM_CODE );
		}
	}

	// textures
	{
		//T-PASS1-COL
		{
			glr.texture (
				"T-PASS1-COL",
				new GLR::Texture (
					"pass1C",
					GL_TEXTURE_2D, 0, GL_RGBA32F,
					GL_NEAREST, GL_NEAREST,
					GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
					GL_RGBA, GL_FLOAT,
					16, 9 ) );
		}
		//T-PASS1-Z
		{
			glr.texture (
				"T-PASS1-Z",
				new GLR::Texture (
					"pass1Z",
					GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
					GL_NEAREST, GL_NEAREST,
					GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
					GL_DEPTH_COMPONENT, GL_UNSIGNED_INT,
					16, 9 ) );
		}
	}

	// containers
	{
		glr.
			container ( "C-LIGHT" ).
			setVertexArray ( "V-LIGHT" ).
			setShader( "S-LIGHT" ).
			build ( );

		glr.container ( "C-PASS2" ).
			addInTexture ( "T-PASS1-COL" ).
			setVertexArray ( "V-PASS2" ).
			setShader( "S-PASS2" ).
			build ( );
	}

	cAudi  = add2GLR( obj1, "audi000tri.obj", "/home/friedrich/Downloads/models/audi000/", "/home/friedrich/Downloads/models/audi000/" );
	cWoman = add2GLR( obj2, "rosawomantri.obj", "/home/friedrich/Development/c++/2018.06.03/obj/", "/home/friedrich/Development/c++/2018.06.03/pix/" );

	projection = view = model = glm::mat4 ( 1. );

	model = glm::translate( model, V3 ( +0, +0, +0 ) );
	view  = glm::translate( view,  V3 ( +0, +0, -10 ) );

	glClearColor( .51f, .52f, .53f, 1. );

	glEnable ( GL_DEPTH_TEST );
	glDisable ( GL_CULL_FACE );
}

void
ObjExample:: paint ( ) {

	GLR::CameraCenterView
	ccv( model, view, vcd, glm::vec3 ( .02f, .02f, .1f ) );

	ccv.reactOnMouse ( );

	ccv.rotate_around_y ( .001f );

	model = ccv.model ( );

	view  = ccv.view ( );

	glr.run ( cWoman [ PASSES_1 ] );

	GLfloat const
	pi = 3.141593f;

	GLfloat
	tm = glm::mod ( vcd->time, 20.f * pi ),
	r;

	if ( 10.f * pi < tm )

		tm = 20.f * pi - tm;

	r  = 2.f + .33f * tm;

	lightPos = V3( model * V4 ( r * sinf( vcd->time ), 2.f + 8.f * cosf( .1f * vcd->time ), r * cosf( vcd->time ), 1. ) );

	glr.screenoff ( );

	glEnable ( GL_DEPTH_TEST );
	glDisable ( GL_CULL_FACE );
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	M4
	tmpGlobal = model;

	model = glm::translate ( model, V3 ( 0.f, -10.f, 0.f ) );

	M4
	tmpLocal = model;

	model = glm::scale ( model, V3 ( 5.f, 5.f, 5.f ) );

	glr.run ( cWoman [ PASSES_1 ] );

	model = glm::rotate ( tmpLocal, 1.57f, V3 ( 0.f, 1.f, 0.f ) );
	model = glm::translate ( model, V3 ( 0.f, +4.5f, 0.f ) );

	glr.run ( cAudi [ PASSES_1 ] );
	glr.run ( { "C-LIGHT" } );

	glr.screenon ( );
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

//	glr.run ( { "C-PASS2" } );

	glEnable ( GL_DEPTH_TEST );
	glr.run ( cAudi [ PASSES_2 ] );

	model = glm::scale ( tmpLocal, V3 ( 5.f, 5.f, 5.f ) );

	glr.run ( cWoman [ PASSES_1 ] );

	model = glm::rotate ( tmpLocal, 1.57f, V3 ( 0.f, 1.f, 0.f ) );
	model = glm::translate ( model, V3 ( 0.f, +4.5f, 0.f ) );

	glr.run ( cAudi [ PASSES_1 ] );
	glr.run ( { "C-LIGHT" } );

	model = tmpGlobal;

/*
	GLfloat const
	pi = 3.141593f;

	GLfloat
	tm = glm::mod ( vcd->time, 20.f * pi ),
	r;

	if ( 10.f * pi < tm )

		tm = 20.f * pi - tm;

	r  = 2.f + .33f * tm;

	lightPos = V3( model * V4 ( r * sinf( vcd->time ), 2.f + 8.f * cosf( .1f * vcd->time ), r * cosf( vcd->time ), 1. ) );

	M4
	tmpGlobal = model;

	model = glm::translate ( model, V3 ( 0.f, -10.f, 0.f ) );

	M4
	tmpLocal = model;

	model = glm::scale ( model, V3 ( 5.f, 5.f, 5.f ) );

	glr.run ( cWoman [ PASSES_1 ] );

	model = glm::rotate ( tmpLocal, 1.57f, V3 ( 0.f, 1.f, 0.f ) );
	model = glm::translate ( model, V3 ( 0.f, +4.5f, 0.f ) );

	glr.run ( cAudi [ PASSES_1 ] );

	glr.run ( { "C-PASS2" } );

//	model = glm::scale ( tmpLocal, V3 ( 5.f, 5.f, 5.f ) );

//	glr.run ( cWoman [ PASSES_2 ] );

	model = glm::rotate ( tmpLocal, 1.57f, V3 ( 0.f, 1.f, 0.f ) );
	model = glm::translate ( model, V3 ( 0.f, +4.5f, 0.f ) );

	glr.run ( cAudi [ PASSES_2 ] );

	glr.run ( { "C-LIGHT" } );

	model = tmpGlobal;
*/
}

void
ObjExample:: resize ( int p_width, int p_height ) {

	// get aspect ratio
	float
	ratio = ( 1.f * p_width / p_height );

	// create a projection matrix
	projection = glm::perspective( 30.f * 3.14159f / 180.f, ratio, 1.0f, 300.f );

	glr.screenon ( );
}
