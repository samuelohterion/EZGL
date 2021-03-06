#ifndef OBJ_HPP
#define OBJ_HPP

#include "../../code/glr.hpp"
#include "../../code/glmprinter.hpp"
#include "../../code/filesys.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>
#include <unistd.h>

enum PASSES {

	PASSES_1 = 0,
	PASSES_2 = 1,
	PASSES_COUNT = 2
};

enum SHADER_ALTERNATIVE {

	SA_V     = 0,
	SA_VVT   = 1,
	SA_VVN   = 2,
	SA_VVTVN = 3,
	SA_COUNT = 4
};

enum SHADER_TYPES {

	ST_VERTEX = 0,
	ST_FRAGMENT = 1,
	ST_COUNT = 2
};

CStr
SHADERS [ PASSES_COUNT ][ SA_COUNT ][ ST_COUNT ] = {
	// PASS 1
	{
		// S-V
		{
			// vertex shader
			"#version 330 core\n"
			"in vec3 v;\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"

			"void main( ) {\n"
			"	gl_Position = projection * view * model * vec4( v, 1 );\n"
			"}\n",

			// fragment shader
			"#version 330 core\n"

			"uniform float Ns;\n"
			"uniform float Ni;\n"
			"uniform float d;\n"
			"uniform vec3 Ka;\n"
			"uniform vec3 Kd;\n"
			"uniform vec3 Ks;\n"
			"uniform vec3 Ke;\n"
			"uniform int pass;\n"
			"out vec4 fColor;\n"
			"void main( ) {\n"
			"	fColor = vec4( Kd, 1 );\n"
			"}\n"
		},
		// S-VVT
		{
			// vertex shader
			"#version 330 core\n"
			"in vec3 v;\n"
			"in vec2 vt;\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"

			"out VS2FS {\n"
			"	vec2 vt;\n"
			"} vs2fs;\n"

			"void main( ) {\n"
			"	vs2fs.vt = vt;\n"
			"	gl_Position = projection * view * model * vec4( v, 1 );\n"
			"}\n",

			// fragment shader
			"#version 330 core\n"

			"in VS2FS {\n"
			"	vec2 vt;\n"
			"} vs2fs;\n"
			//		"uniform float Ns;\n"
			//		"uniform float Ni;\n"
			//		"uniform float d;\n"
			//		"uniform vec3 Ka;\n"
			"uniform vec3 Kd;\n"
			//		"uniform vec3 Ks;\n"
			//		"uniform vec3 Ke;\n"
			"uniform sampler2D map_Kd;\n"
			"uniform int pass;\n"
			"out vec4 fColor;\n"
			"void main( ) {\n"
			"	fColor = texture( map_Kd, vs2fs.vt );\n"
			"}\n"
		},
		// S-VVN
		{
			// vertex shader
			"#version 330 core\n"
			"in vec3 v;\n"
			"in vec3 vn;\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"

			"out VS2FS {\n"
			"	vec3 v;\n"
			"	vec3 vn;\n"
			"} vs2fs;\n"

			"void main( ) {\n"
			"	vs2fs.v  = vec3( model * vec4( v, 1 ) );\n"
			"	vs2fs.vn = normalize( vec3( vec4( vn, 0 ) * inverse( model ) ) );\n"
			"	gl_Position = projection * view * model * vec4( v, 1 );\n"
			"}\n",

			// fragment shader
			"#version 330 core\n"

			"in VS2FS {\n"
			"	vec3 v;\n"
			"	vec3 vn;\n"
			"} vs2fs;\n"
			"const vec3 lightColor = vec3( 1., 1., 1. );\n"
			"const float lightPower = 3.;\n"
			"const float screenGamma = 1.5;\n" // Assume the monitor is calibrated to the sRGB color space
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform float Ns;\n"
			"uniform float Ni;\n"
			"uniform float d;\n"
			"uniform vec3 Ka;\n"
			"uniform vec3 Kd;\n"
			"uniform vec3 Ks;\n"
			"uniform vec3 Ke;\n"
			"uniform vec3 lightPos;\n"
			"out vec4 fColor;\n"
			"void main( ) {\n"
			"	vec3 ray = lightPos - vs2fs.v;\n"
			"	float distance = length( ray );\n"
			"	ray /= distance;\n"
			"	float lambertian1 = max( dot( ray, vs2fs.vn ), 0. );\n"
			"	float lambertian2 = max( dot( vec3( 0, 0, 2 ), vs2fs.vn ), 0. );\n"
			"	float specular = 0.0;\n"
			"	if ( lambertian1 > 0.0 ) {\n"
			"		vec3 viewDir = normalize( -vs2fs.v );\n"
			"		vec3 halfDir = normalize( ray + viewDir );\n"
			"		float specAngle = max( dot( halfDir, vs2fs.vn ), 0.0 );\n"
			"		specular = pow( specAngle, Ns );\n"
			"	}\n"
			"	vec3 colorLinear = Ka + ( Kd * ( lambertian1 + lambertian2 ) + Ks * specular ) * lightColor * lightPower / distance;\n"
			"	vec3 colorGammaCorrected = pow( colorLinear, vec3( 1.0 / screenGamma ) );\n"
			"	fColor = vec4( colorGammaCorrected, 1. );"
			"}\n"
		},
		// S-VVTVN
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

			"in VS2FS {\n"
			"	vec3 v;\n"
			"	vec2 vt;\n"
			"	vec3 vn;\n"
			"} vs2fs;\n"
			"const vec3 lightColor = vec3( 1., 1., 1. );\n"
			"const float lightPower = 3.;\n"
			"const float screenGamma = 1.5;\n" // Assume the monitor is calibrated to the sRGB color space
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform float Ns;\n"
			"uniform float Ni;\n"
			"uniform float d;\n"
			"uniform vec3 Ka;\n"
			"uniform vec3 Kd;\n"
			"uniform vec3 Ks;\n"
			"uniform vec3 Ke;\n"
			"uniform sampler2D map_Kd;\n"
			"uniform vec3 lightPos;\n"
			"uniform int pass;\n"
			"out vec4 fColor;\n"
			"void main( ) {\n"
			"	vec3 ray = lightPos - vs2fs.v;\n"
			"	float distance = length( ray );\n"
			"	ray /= distance;\n"
			"	float lambertian1 = max( dot( ray, vs2fs.vn ), 0. );\n"
			"	float lambertian2 = max( dot( vec3( 0, 0, 2 ), vs2fs.vn ), 0. );\n"
			"	float specular = 0.0;\n"
			"	if ( lambertian1 > 0.0 ) {\n"
			"		vec3 viewDir = normalize( -vs2fs.v );\n"
			"		vec3 halfDir = normalize( ray + viewDir );\n"
			"		float specAngle = max( dot( halfDir, vs2fs.vn ), 0.0 );\n"
			"		specular = pow( specAngle, Ns );\n"
			"	}\n"
			"	vec3 texCol = vec3( texture( map_Kd, vs2fs.vt ) );\n"
			"	vec3 colorLinear = Ka + ( ( texCol + Kd ) * ( lambertian1 + lambertian2 ) + Ks * specular ) * lightColor * lightPower / distance;\n"
			"	vec3 colorGammaCorrected = pow( colorLinear, vec3( 1.0 / screenGamma ) );\n"
			"	fColor = vec4( colorGammaCorrected, 1. );"
			"}\n"
		}
	},

	// PASS 2
	{
		// S-V
		{
			// vertex shader
			"#version 330 core\n"
			"in vec3 v;\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"out VS2FS {\n"
			"	vec4 tx;\n"
			"} vs2fs;\n"

			"void main( ) {\n"
			"	vs2fs.v  = vec3( model * vec4( v, 1 ) );\n"
			"	vec4 p = projection * view * model * vec4( v, 1 );\n"
			"	vs2fs.tx = .5 + .5 * ( p / p.a );\n"
			"	gl_Position = p;\n"
			"}\n",

			// fragment shader
			"#version 330 core\n"

			"in VS2FS {\n"
			"	vec4 tx;\n"
			"} vs2fs;\n"
			"uniform float Ns;\n"
			"uniform float Ni;\n"
			"uniform float d;\n"
			"uniform vec3 Ka;\n"
			"uniform vec3 Kd;\n"
			"uniform vec3 Ks;\n"
			"uniform vec3 Ke;\n"
			"uniform sampler2D pass1C;\n"
			"out vec4 fColor;\n"
			"void main( ) {\n"
			"	vec3 col = vec3( texture( pass1C, vs2fs.tx.xy ) );\n"
			"	fColor = vec4( mix( Kd, col, d ), 1 );\n"
			"}\n"
		},
		// S-VVT
		{
			// vertex shader
			"#version 330 core\n"
			"in vec3 v;\n"
			"in vec2 vt;\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"

			"out VS2FS {\n"
			"	vec2 vt;\n"
			"	vec4 tx;\n"
			"} vs2fs;\n"

			"void main( ) {\n"
			"	vs2fs.v  = vec3( model * vec4( v, 1 ) );\n"
			"	vs2fs.vt = vt;\n"
			"	vs2fs.vn = normalize( vec3( vec4( vn, 0 ) * inverse( model ) ) );\n"
			"	vec4 p = projection * view * model * vec4( v, 1 );\n"
			"	vs2fs.tx = .5 + .5 * ( p / p.a );\n"
			"	gl_Position = p;\n"
			"}\n",

			// fragment shader
			"#version 330 core\n"

			"in VS2FS {\n"
			"	vec2 vt;\n"
			"	vec4 tx;\n"
			"} vs2fs;\n"
			"uniform float Ns;\n"
			"uniform float Ni;\n"
			"uniform float d;\n"
			"uniform vec3 Ka;\n"
			"uniform vec3 Kd;\n"
			"uniform vec3 Ks;\n"
			"uniform vec3 Ke;\n"
			"uniform sampler2D map_Kd;\n"
			"uniform sampler2D pass1C;\n"
			"out vec4 fColor;\n"
			"void main( ) {\n"
			"	vec3 texCol  = vec3( texture( map_Kd, vs2fs.vt ) );\n"
			"	vec3 backCol = vec3( texture( pass1C, vs2fs.tx.xy ) );\n"
			"	fColor = mix( backCol, texCol, d );\n"
			"}\n"
		},
		// S-VVN
		{
			// vertex shader
			"#version 330 core\n"
			"in vec3 v;\n"
			"in vec3 vn;\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"

			"out VS2FS {\n"
			"	vec3 v;\n"
			"	vec3 vn;\n"
			"	vec4 tx;\n"
			"} vs2fs;\n"

			"void main( ) {\n"
			"	vs2fs.v  = vec3( model * vec4( v, 1 ) );\n"
			"	vs2fs.vn = normalize( vec3( vec4( vn, 0 ) * inverse( model ) ) );\n"
			"	vec4 p = projection * view * model * vec4( v, 1 );\n"
			"	vs2fs.tx = .5 + .5 * ( p / p.a );\n"
			"	gl_Position = p;\n"
			"}\n",

			// fragment shader
			"#version 330 core\n"

			"in VS2FS {\n"
			"	vec3 v;\n"
			"	vec3 vn;\n"
			"	vec4 tx;\n"
			"} vs2fs;\n"
			"const vec3 lightColor = vec3( 1., 1., 1. );\n"
			"const float lightPower = 3.;\n"
			"const float screenGamma = 1.5;\n" // Assume the monitor is calibrated to the sRGB color space
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform float Ns;\n"
			"uniform float Ni;\n"
			"uniform float d;\n"
			"uniform vec3 Ka;\n"
			"uniform vec3 Kd;\n"
			"uniform vec3 Ks;\n"
			"uniform vec3 Ke;\n"
			"uniform vec3 lightPos;\n"
			"uniform sampler2D pass1C;\n"
			"out vec4 fColor;\n"
			"void main( ) {\n"
			"	vec3 ray = lightPos - vs2fs.v;\n"
			"	float distance = length( ray );\n"
			"	ray /= distance;\n"
			"	float lambertian1 = max( dot( ray, vs2fs.vn ), 0. );\n"
			"	float lambertian2 = max( dot( vec3( 0, 0, 2 ), vs2fs.vn ), 0. );\n"
			"	float specular = 0.0;\n"
			"	if ( lambertian1 > 0.0 ) {\n"
			"		vec3 viewDir = normalize( -vs2fs.v );\n"
			"		vec3 halfDir = normalize( ray + viewDir );\n"
			"		float specAngle = max( dot( halfDir, vs2fs.vn ), 0.0 );\n"
			"		specular = pow( specAngle, Ns );\n"
			"	}\n"
			"	vec3 backCol = vec3( texture( pass1C, vs2fs.tx.xy ) );\n"
			"	vec3 colorLinear = Ka + ( mix( backCol, Kd, d ) * ( lambertian1 + lambertian2 ) + Ks * specular ) * lightColor * lightPower / distance;\n"
			"	vec3 colorGammaCorrected = pow( colorLinear, vec3( 1.0 / screenGamma ) );\n"
			"	fColor = vec4( colorGammaCorrected, 1. );"
			"}\n"
		},
		// S-VVTVN
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
			"	vec4 tx;\n"
			"} vs2fs;\n"

			"void main( ) {\n"
			"	vs2fs.v  = vec3( model * vec4( v, 1 ) );\n"
			"	vs2fs.vt = vt;\n"
			"	vs2fs.vn = normalize( vec3( vec4( vn, 0 ) * inverse( model ) ) );\n"
			"	vec4 p = projection * view * model * vec4( v, 1 );\n"
			"	vs2fs.tx = .5 + .5 * ( p / p.a );\n"
			"	gl_Position = p;\n"
			"}\n",

			// fragment shader
			"#version 330 core\n"

			"in VS2FS {\n"
			"	vec3 v;\n"
			"	vec2 vt;\n"
			"	vec3 vn;\n"
			"	vec4 tx;\n"
			"} vs2fs;\n"
			"const vec3 lightColor = vec3( 1., 1., 1. );\n"
			"const float lightPower = 3.;\n"
			"const float screenGamma = 1.5;\n" // Assume the monitor is calibrated to the sRGB color space
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"uniform float Ns;\n"
			"uniform float Ni;\n"
			"uniform float d;\n"
			"uniform vec3 Ka;\n"
			"uniform vec3 Kd;\n"
			"uniform vec3 Ks;\n"
			"uniform vec3 Ke;\n"
			"uniform sampler2D map_Kd;\n"
			"uniform sampler2D pass1C;\n"
			"uniform vec3 lightPos;\n"
			"out vec4 fColor;\n"
			"void main( ) {\n"
			"	vec3 ray = lightPos - vs2fs.v;\n"
			"	float distance = length( ray );\n"
			"	ray /= distance;\n"
			"	float lambertian1 = max( dot( ray, vs2fs.vn ), 0. );\n"
			"	float lambertian2 = max( dot( vec3( 0, 0, 2 ), vs2fs.vn ), 0. );\n"
			"	float specular = 0.0;\n"
			"	if ( lambertian1 > 0.0 ) {\n"
			"		vec3 viewDir = normalize( -vs2fs.v );\n"
			"		vec3 halfDir = normalize( ray + viewDir );\n"
			"		float specAngle = max( dot( halfDir, vs2fs.vn ), 0.0 );\n"
			"		specular = pow( specAngle, Ns );\n"
			"	}\n"
			"	vec3 texCol = vec3( texture( map_Kd, vs2fs.vt ) );\n"
			"	vec3 backCol = vec3( texture( pass1C, vs2fs.tx.xy ) );\n"
			"	vec3 colorLinear = Ka + ( mix( backCol, texCol * Kd, d ) * ( lambertian1 + lambertian2 ) + Ks * specular ) * lightColor * lightPower / distance;\n"
			"	vec3 colorGammaCorrected = pow( colorLinear, vec3( 1.0 / screenGamma ) );\n"
			"	fColor = vec4( colorGammaCorrected, 1. );"
			"}\n"
		}
	}
};

class Material :
public Named  {

	public :

		Material ( CStr & p_name ) :
		Named ( p_name ),
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

		Material ( Material const & p_material ) :
		Named ( p_material.name( ) ),
		Ns ( p_material.Ns ),
		Ni ( p_material.Ni ),
		d ( p_material.d ),
		illum( p_material.illum ),
		Ka ( p_material.Ka ),
		Kd ( p_material.Kd ),
		Ks ( p_material.Ks ),
		Ke ( p_material.Ke ),
		map_Ka ( p_material.map_Ka ),
		map_Kd ( p_material.map_Kd ),
		map_Ks ( p_material.map_Ks ),
		map_Ke ( p_material.map_Ke ) {

		}

		~ Material ( ) {

			// delete shader;
		}

	public :

		GLfloat
		Ns,
		Ni,
		d;

		GLuint
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

class MTL {

	public :

		MTL ( ) {

		}

		~ MTL ( ) {

			for ( auto & m : materials ) {

				delete m.second;
			}
		}

	public :

		std::map < CStr, Material * >
		materials;

	private :

		Str
		currMaterialName = "";

		Material
		* currMaterial = nullptr;

	public :

		bool
		fromFile ( CStr & p_filename ) {

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

						std::cout << "    material: " << currMaterialName << std::endl;

						currMaterial = new Material ( currMaterialName );

						materials[ currMaterialName ] = currMaterial;
					}
					else if( arg == "#" ) {

						Str
						s;

						std::cout << "  " << line << std::endl;

						break;
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

			return true;
		}
};

enum TypeOfIndices {

	TOI_UNKOWN,
	TOI_VERTEX_ONLY,
	TOI_VERTEX_TEXTURE,
	TOI_VERTEX_NORMAL,
	TOI_VERTEX_TEXTURE_NORMAL
};

class Surface :
public Named  {

	public :

		Surface ( CStr & p_name, TypeOfIndices const & p_toi = TOI_UNKOWN, CStr & p_material = "NONE" ) :
		Named ( p_name  ),
		mat( p_material ),
		toi ( p_toi ) {

		}

		~ Surface ( ) {

		}

	public :

		Str
		mat;

		TypeOfIndices
		toi;

		std::vector < GLuint >
		idx;
};

class Objekt :
public Named  {

	public :

		Objekt ( CStr & p_name ) :
		Named ( p_name  ) {

		}

		~ Objekt ( ) {

			for ( auto & s : surfaces ) {

				delete s.second;
			}
		}

	public :

		std::map < CStr, Surface * >
		surfaces;
};

class OBJ {

	private :

		void
		read2Floats( GLfloat * p_x, GLfloat * p_y, std::string const & p_str ) {

			std::stringstream
			ss( p_str );

			std::string
			str;

			ss >> str >> * p_x >> * p_y;
		}

		void
		read3Floats( GLfloat * p_x, GLfloat * p_y, GLfloat * p_z, std::string const & p_str ) {

			std::stringstream
			ss( p_str );

			std::string
			str;

			ss >> str >> * p_x >> * p_y >> * p_z;
		}

		void
		read2int( GLint * p_x, GLint * p_y, std::string const & p_str ) {

			std::stringstream
			ss( p_str );

			ss >> * p_x >> * p_y;
		}

		void
		read3int( GLint * p_x, GLint * p_y, GLint * p_z, std::string const & p_str ) {

			std::stringstream
			ss( p_str );

			ss >> * p_x >> * p_y >> * p_z;
		}

	public :

		OBJ ( std::size_t const & p_tempMemSize = 0x10000 ) :
		v ( p_tempMemSize, 0.f ),
		vt ( p_tempMemSize, 0.f ),
		vn ( p_tempMemSize, 0.f ) {

		}

		~ OBJ ( ) {

		}

	public :

		MTL
		mtl;

	private :

		Str
		currObjektName = "",
		currSurfaceName = "";

		GLuint
		currSurfaceId = 0;

		Objekt
		* currObjekt;

		Surface
		* currSurface;

	public :

		std::map < CStr, Objekt * >
		objekts;

		std::vector < GLfloat >
		v,
		vt,
		vn;

	public :

		TypeOfIndices
		typeOfIndices ( std::string const & p_string ) {

			int
			firstSlash  = p_string.find_first_of( "/" );

			if ( firstSlash < 1 ) {

				return TOI_VERTEX_ONLY;
			}

			int
			secondSlash = p_string.find_first_of( "/", firstSlash + 1 );

			if ( secondSlash - firstSlash < 2 ) {

				return TOI_VERTEX_NORMAL;
			}

			int
			thirdSlash = p_string.find_first_of( "/", secondSlash + 1 ),
			fourthSlash = p_string.find_first_of( "/", thirdSlash + 1 );

			if ( fourthSlash < 1 ) {

				return TOI_VERTEX_TEXTURE;
			}

			return TOI_VERTEX_TEXTURE_NORMAL;
		}

		bool
		fromFile ( CStr & p_filename ) {

			std::string
			cpath = FileSys::currentPath ( ),
			pathName,
			fileName;

			FileSys::extractPathAndFile ( pathName, fileName, p_filename );

			std::cout << "load " << fileName << " from " << pathName << std::endl;

			FileSys::changeDir ( pathName );

			std::ifstream
			file ( fileName );

			if ( ! file.is_open ( ) ) {

				std::cout << "Couldn't find file " << p_filename << ".\n exit...\n";

				return false;
			}

			GLfloat
			* vIt = v.data ( ),
			* vtIt = vt.data ( ),
			* vnIt = vn.data ( );

			GLuint
			vId[ 10 ];

			while ( file.good ( ) ) {

				std::string
				line,
				arg;

				std::getline ( file, line );

				char
				token = line [ 0 ];

				if ( token == 'v' ) {

					if ( line [ 1 ] == 't' ) {

//						sscanf ( line.c_str ( ), "%*s%f%*s%f", vtIt + 0, vtIt + 1 );

						read2Floats( vtIt, vtIt + 1, line );

//						std::cout << "vt:\t" << vtIt [ 0 ] << "\t" << vtIt [ 1 ] << std::endl;

						vtIt += 2;
					}
					else if ( line [ 1 ] == 'n' ) {

//						sscanf ( line.c_str ( ), "%*s%f%*s%f%*s%f", vnIt + 0, vnIt + 1, vnIt + 2 );

						read3Floats ( vnIt, vnIt + 1, vnIt + 2, line );

//						std::cout << "vn:\t" << vnIt [ 0 ] << "\t" << vnIt [ 1 ] << "\t" << vnIt [ 2 ] << std::endl;

						vnIt += 3;
					}
					else {

//						sscanf ( line.c_str ( ), "%*s%f%*s%f%*s%f", vIt + 0, vIt + 1, vIt + 2 );

						read3Floats ( vIt, vIt + 1, vIt + 2, line );

//						std::cout << "v:\t" << vIt [ 0 ] << "\t" << vIt [ 1 ] << "\t" << vIt [ 2 ] << std::endl;

						vIt += 3;
					}
				}
				else if ( token == 'f' ) {

					if ( currSurface->toi == TOI_UNKOWN ) {

						currSurface->toi = typeOfIndices ( line );
					}

					switch ( currSurface->toi ) {

						case TOI_VERTEX_ONLY : {

							sscanf ( line.c_str ( ), "%*s %d %d %d", vId, vId + 1, vId + 2 );

							for ( std::size_t i = 0; i < 3; ++ i ) {

								currSurface->idx.push_back( vId[ i ] );
							}

							break;
						}
						case TOI_VERTEX_TEXTURE : {

							sscanf ( line.c_str ( ), "%*s %d/%d %d/%d %d/%d", vId, vId + 1, vId + 2, vId + 3, vId + 4, vId + 5 );

							for ( std::size_t i = 0; i < 6; ++ i ) {

								currSurface->idx.push_back( vId[ i ] );
							}

							break;
						}
						case TOI_VERTEX_NORMAL : {

							sscanf ( line.c_str ( ), "%*s %d//%d %d//%d %d//%d", vId, vId + 1, vId + 2, vId + 3, vId + 4, vId + 5 );

							for ( std::size_t i = 0; i < 6; ++ i ) {

								currSurface->idx.push_back( vId[ i ] );
							}

							break;
						}
						case TOI_VERTEX_TEXTURE_NORMAL : {

							sscanf ( line.c_str ( ), "%*s %d/%d/%d %d/%d/%d %d/%d/%d", vId, vId + 1, vId + 2, vId + 3, vId + 4, vId + 5, vId + 6, vId + 7, vId + 8 );

							for ( std::size_t i = 0; i < 9; ++ i ) {

								currSurface->idx.push_back( vId[ i ] );
							}

							break;
						}
						default : break;
					}
				}
				else if ( token == 'u' ) {

					currSurfaceName = std::to_string ( currSurfaceId ++ );

					std::cout << "  surface: " << currSurfaceName << "   ";

					currSurface = new Surface ( currSurfaceName );

					currSurface->mat = std::string ( line.cbegin ( ) + 7, line.cend ( ) );

					std::cout << "mat: " << currSurface->mat << std::endl;

					currObjekt->surfaces[ currSurfaceName ] = currSurface;
				}
				else if ( token == 'o' ) {

					currObjektName = std::string ( line.cbegin ( ) + 2, line.cend ( ) );

					std::cout << "object: " << currObjektName << std::endl;

					currObjekt = new Objekt ( currObjektName );

					objekts[ currObjektName ] = currObjekt;

					currSurfaceId = 0;
				}
				else if ( token == 'm' ) {

					char
					mtllib[ 7 ],
					fn[ 0x200 ];

					sscanf ( line.c_str ( ), "%s %s", mtllib, fn );

					mtl.fromFile ( fn );
				}
				else if ( token == '#' ) {

					std::cout << line << std::endl;
				}
			}

			file.close ( );

			FileSys::changeDir ( cpath );

			return true;
		}

		std::vector< std::vector< std::string > >
		add2GLR( GLR & p_glr, CStr & p_objName , CStr & p_pixPath );
};

class ObjExample :
public GLProject {

	public :

		ObjExample ( CStr & p_name = "ObjExample" );

		~ ObjExample ( );

	public :

		OBJ
		obj1,
		obj2;

		M4
		projection,
		view,
		model;

		V3
		lightPos;

		std::vector < std::vector < Str > >
		cAudi,
		cWoman;

		V3
		Kd;

		GLint
		renderPass;

		std::vector< V3 >
		piecePositions;

	public :

	public:

		void
		init ( );

		void
		paint ( );

		void
		resize ( int p_width, int p_height );

		std::vector < std::vector < std::string > >
		add2GLR ( OBJ & p_obj, std::string const & p_objName, std::string const & p_objPath, std::string const & p_pixPath );
};

#endif // SCENEEXAMPLE_HPP
