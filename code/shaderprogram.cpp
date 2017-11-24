#include "shaderprogram.hpp"

Shader::Shader( GLenum p_shaderType, std::string const &p_text, CreationMethod const &p_method ) :
__type( p_shaderType ),
__infoLog( 0 ) {

	if( p_method == FROM_CODE ) {

		createFromCode( p_text );

		return;
	}

	createFromFile( p_text );
}

Shader::~Shader( ) {

	glDeleteShader( __id );

	if( __infoLog ) {

		delete __infoLog;
	}

}

bool
Shader::createFromCode( std::string const &p_code ) {

	__code = p_code;

	GLchar const
	*cStr = p_code.c_str( );

	__id = glCreateShader( __type );

	glShaderSource( __id, 1, &cStr, 0 );
	glCompileShader( __id );

	glGetShaderiv( __id, GL_COMPILE_STATUS, &__success );

	if( !__success ) {

		__infoLog = new GLchar[ 0x200 ];

		glGetShaderInfoLog( __id, 0x200, 0, __infoLog );

		std::cout << "ERROR::SHADER::" << ( __type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT" ) << "::COMPILATION_FAILED\n" << __infoLog << std::endl;

		return false;
	}

	return true;
}

bool
Shader::createFromFile( std::string const &p_fileName ) {

	try {

		std::ifstream
		ifs( p_fileName.c_str( ) );

		std::stringstream
		ss;

		ss << ifs.rdbuf( );

		ifs.close( );

		return createFromCode( ss.str( ) );
	}
	catch( std::exception p_exception  ) {

		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return true;
}

std::string
Shader::code( ) const {

	return __code;
}

GLuint
Shader::id( ) const {

	return __id;
}

std::string
Shader::fileName( ) const {

	return __fname;
}

bool
Shader::ok( ) const {

	return __success;
}

GLenum
Shader::type( ) const {

	return __type;
}


ShaderProgram::ShaderProgram( std::__cxx11::string const &p_vertexText, std::__cxx11::string const &p_fragmentText, Shader::CreationMethod const &p_method ) :
__vertexShader( new Shader( GL_VERTEX_SHADER, p_vertexText, p_method ) ),
__fragmentShader( new Shader( GL_FRAGMENT_SHADER, p_fragmentText, p_method ) ),
__success( 0 ),
__id( 0 ),
__infoLog( 0 ) {

	if( __vertexShader && __fragmentShader && __vertexShader->ok( ) && __fragmentShader->ok( ) ) {

		__id = glCreateProgram( );
		glAttachShader( __id, __vertexShader->id( ) );
		glAttachShader( __id, __fragmentShader->id( ) );

		glLinkProgram( __id );

		glGetProgramiv( __id, GL_LINK_STATUS, &__success );

		if( !__success ) {

			__infoLog = new GLchar[ 0x200 ];

			glGetProgramInfoLog( __id, 0x200, 0, __infoLog );
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << __infoLog << std::endl;

			return;
		}

		glDeleteShader( __vertexShader->id( ) );
		glDeleteShader( __fragmentShader->id( ) );
	}
}

ShaderProgram::ShaderProgram( std::__cxx11::string const &p_vertexText, std::__cxx11::string const &p_fragmentText, Shader::CreationMethod const &p_method, std::vector< GLuint > const &p_locId, std::vector< std::string > const &p_names ) :
__vertexShader( new Shader( GL_VERTEX_SHADER, p_vertexText, p_method ) ),
__fragmentShader( new Shader( GL_FRAGMENT_SHADER, p_fragmentText, p_method ) ),
__success( 0 ) {

	if( __vertexShader && __fragmentShader && __vertexShader->ok( ) && __fragmentShader->ok( ) ) {

		__id = glCreateProgram( );
		glAttachShader( __id, __vertexShader->id( ) );
		glAttachShader( __id, __fragmentShader->id( ) );

		for( unsigned int i = 0; i < p_locId.size( ); ++i ) {

			glBindAttribLocation( __id, p_locId[ i ], p_names[ i ].c_str( ) );
		}

		glLinkProgram( __id );

		glGetProgramiv( __id, GL_LINK_STATUS, &__success );

		if( !__success ) {

			__infoLog = new GLchar[ 0x200 ];

			glGetProgramInfoLog( __id, 0x200, 0, __infoLog );
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << __infoLog << std::endl;

			return;
		}

		glDeleteShader( __vertexShader->id( ) );
		glDeleteShader( __fragmentShader->id( ) );
	}
}

ShaderProgram::~ShaderProgram( ) {

	glDeleteProgram( __id );

	if( __infoLog ) {

		delete __infoLog;
	}
	if( __fragmentShader ) {

		delete __fragmentShader;
	}
	if( __vertexShader ) {

		delete __vertexShader;
	}
}

GLuint
ShaderProgram::id( ) const {

	return __id;
}

bool
ShaderProgram::ok( ) const {

	return __success;
}

void
ShaderProgram::bind( ) {

	return glUseProgram( __id );
}

void
ShaderProgram::release( ) {

	return glUseProgram( 0 );
}

void
ShaderProgram::setUniformBool( GLchar const *p_name, GLboolean const &p_value ) {

	glUniform1i( glGetUniformLocation( __id, p_name ), p_value );
}

void
ShaderProgram::setUniformInt( GLchar const *p_name, GLint const &p_value ) {

	glUniform1i( glGetUniformLocation( __id, p_name ), p_value );
}

void
ShaderProgram::setUniformFloat( GLchar const *p_name, GLfloat const &p_value ) {

	glUniform1f( glGetUniformLocation( __id, p_name ), p_value );
}

GLint
ShaderProgram::setVertexAttrib( std::string const &p_name, GLint p_size, GLenum p_type, GLboolean p_normalized, GLsizei p_stride, void const *p_offset ) {

	GLint
	var = glGetAttribLocation( __id,  p_name.c_str( ) );
	glVertexAttribPointer( var, p_size, p_type, p_normalized, p_stride, p_offset );
	glEnableVertexAttribArray( var );

	return var;
}

void
ShaderProgram::setUniformVec2F( GLchar const *p_name, GLfloat const *p_vec2 ) {

	glUniform2fv( glGetUniformLocation( __id, p_name ), 1, p_vec2 );
}

void
ShaderProgram::setUniformVec3F( GLchar const *p_name, GLfloat const *p_vec3 ) {

	glUniform3fv( glGetUniformLocation( __id, p_name ), 1, p_vec3 );
}

void
ShaderProgram::setUniformVec4F( GLchar const *p_name, GLfloat const *p_vec4 ) {

	glUniform4fv( glGetUniformLocation( __id, p_name ), 1, p_vec4 );
}

void
ShaderProgram::setUniformMat4x4F( GLchar const *p_name, GLfloat const *p_mat4x4 ) {

	glUniformMatrix4fv( glGetUniformLocation( __id, p_name ), 1, false, p_mat4x4 );
}

