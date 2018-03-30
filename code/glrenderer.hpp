#ifndef GLRENDERER_HPP
#define GLRENDERER_HPP

#include <initializer_list>
#include <fstream>
#include <istream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "../../../includes/glm/glm/glm.hpp"
#include <GLES3/gl32.h>
#include "clock.hpp"

typedef std::string Str;
//typedef std::__cxx11::string Str;
typedef Str const CStr;

class
GLRenderer {

	public :

		class
		Named {

			private :

				CStr
				__name;

			public :

				Named( CStr &p_name ) :
				__name( p_name ) {

				}

				CStr
				name( ) const {

					return __name;
				}
		};

		class
		Texture :
		public Named {

			public :

				GLenum
				target;

				GLuint
				level;

				GLint
				internal_format;

				GLint
				min_filter,
				mag_filter,
				wrap_s,
				wrap_t;

				GLenum
				format,
				type;

				GLsizei
				width,
				height;

				GLuint
				id;

			public :

				Texture(
					CStr p_name,
					GLenum p_target	= GL_TEXTURE_2D,
					GLint p_level = 0,
					GLint p_internal_format = GL_RGBA32F,
					GLint p_min_filter = GL_NEAREST,
					GLint p_mag_filter = GL_NEAREST,
					GLint p_wrap_t = GL_CLAMP_TO_EDGE,
					GLint p_wrap_s = GL_CLAMP_TO_EDGE,
					GLenum p_format = GL_RGBA,
					GLenum p_type = GL_FLOAT,
					GLsizei p_width = 64,
					GLsizei p_height = 64 ) :
					Named( p_name ),
					target( p_target ),
					level( p_level ),
					internal_format( p_internal_format ),
					min_filter( p_min_filter ),
					mag_filter( p_mag_filter ),
					wrap_s( p_wrap_s ),
					wrap_t( p_wrap_t ),
					format( p_format ),
					type( p_type ),
					width( p_width ),
					height( p_height ),
					id( 0 ) {

					glGenTextures( 1, &id );
					glBindTexture( target, id );

					//glTexStorage2D( target, 0, GL_RG32F, simWidth, simHeight );
					glTexParameteri( target, GL_TEXTURE_MIN_FILTER, min_filter );
					glTexParameteri( target, GL_TEXTURE_MAG_FILTER, mag_filter );
					glTexParameteri( target, GL_TEXTURE_WRAP_S, wrap_s );
					glTexParameteri( target, GL_TEXTURE_WRAP_T, wrap_t );

					glTexImage2D( target, level, internal_format, width, height, 0, format, type, 0 );

					glBindTexture( GL_TEXTURE_2D, 0 );
				}

				~Texture( ) {

					glDeleteTextures( 1, &id );
				}

				void
				bind( ) {

					glBindTexture( target, id );
				}

				void
				release( ) {

					glBindTexture( target, 0 );
				}

				void
				resize( GLuint p_width, GLuint p_height ) {

					width = p_width;
					height = p_height;

					glBindTexture( target, id );

					//glTexStorage2D( target, 0, GL_RG32F, simWidth, simHeight );
					glTexImage2D( target, level, internal_format, width, height, 0, format, type, 0 );

					glBindTexture( GL_TEXTURE_2D, 0 );
				}
		};

		class
		VertexArray :
		public Named {

			public :

				GLuint
				vaoId,
				vboId,
				stride,
				vertexCount;

				struct Attr{ GLint size, offs; };

				std::map< CStr, Attr >
				attr;

				std::vector< GLfloat >
				arr;

			public :

				VertexArray( CStr &p_name ) :
				Named( p_name ),
				vaoId( 0 ),
				vboId( 0 ),
				stride( 0 ),
				vertexCount( 0 ) {

					glGenVertexArrays( 1, &vaoId );
//					glBindVertexArray( vaoId );

					glGenBuffers( 1, &vboId );
//					glBindBuffer( GL_ARRAY_BUFFER, vboId );
//					glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * arr.size( ), arr.data( ), GL_STATIC_DRAW );

//					glBindVertexArray( 0 );
				}

				~VertexArray( ) {

				}

/*				virtual void
				init( ) {

					*this
					//	    x      y     s     t
						<< -1. << -1. << 0. << 0.
						<< +1. << -1. << 1. << 0.
						<< -1. << +1. << 0. << 1.
						<< +1. << +1. << 1. << 1.;

					addAttrib( "verts", 2 );
					addAttrib( "coords", 2 );
				}
*/
				VertexArray
				&operator << ( GLfloat const & p_value ) {

					arr.push_back( p_value );

					return *this;
				}

				void
				addAttrib( std::string const & p_name, GLuint const &p_size, GLuint const &p_offset ) {

					attr[ p_name ].size = p_size;
					attr[ p_name ].offs = p_offset;
					stride += p_size;

					vertexCount = arr.size( ) / stride;
				}

				void
				bind( ) {

					glBindVertexArray( vaoId );
					glBindBuffer( GL_ARRAY_BUFFER, vboId );
				}

				void
				release( ) {

					glBindVertexArray( 0 );
					glBindBuffer( GL_ARRAY_BUFFER, 0 );
				}
		};

		class
		ShaderCode {

			public :

				enum
				CreationMethod {

					FROM_CODE,
					FROM_FILE
				};

			public :

				ShaderCode( GLenum p_shaderType, CStr &p_text, CreationMethod const &p_method ) :
				__type( p_shaderType ),
				__infoLog( 0 ) {

					if( p_method == FROM_CODE ) {

						createFromCode( p_text );

						return;
					}

					createFromFile( p_text );
				}

				~ShaderCode( ) {

					glDeleteShader( __id );

					if( __infoLog ) {

						delete __infoLog;
					}
				}

			private :

				GLenum
				__type;			// GL_VERTEX_SHADER or GL_FRAGMENT_SHADER

				GLint
				__success;

				std::string
				__fname,
				__code;

				GLuint
				__id;

				GLchar
				*__infoLog;

			private :

				bool
				createFromCode( CStr &p_code ) {

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

						std::string const
						shaderTypeString =
							__type == GL_FRAGMENT_SHADER
								? "FRAGMENT"
								: __type == GL_GEOMETRY_SHADER
									? "GEOMETRY"
									: __type == GL_FRAGMENT_SHADER
										? "FRAGMENT"
										: "UNKNOWN";

						std::cout << "ERROR::SHADER::" << shaderTypeString << "::COMPILATION_FAILED\n" << __infoLog << std::endl;

						return false;
					}

					return true;
				}

				bool
				createFromFile( CStr &p_fileName ) {

					try {

						std::ifstream
						ifs( p_fileName.c_str( ) );

						std::stringstream
						ss;

						ss << ifs.rdbuf( );

						ifs.close( );

						std::cout << ss.str( ) << std::endl;

						return createFromCode( ss.str( ) );
					}
					catch( std::exception p_exception  ) {

						std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
					}

					return true;
				}

			public :

				Str
				code( ) const {

					return __code;
				}

				GLuint
				id( ) const {

					return __id;
				}

				Str
				fileName( ) const {

					return __fname;
				}

				bool
				ok( ) const {

					return __success;
				}

				GLenum
				type( ) const {

					return __type;
				}
		};

		class
		Shader :
		public Named {

			public :

				Shader( CStr &p_name, CStr &p_vertexText, CStr &p_fragmentText, ShaderCode::CreationMethod const &p_method ) :
				Named( p_name ),
				__vertexShader( new ShaderCode( GL_VERTEX_SHADER, p_vertexText, p_method ) ),
				__geometryShader( 0 ),
				__fragmentShader( new ShaderCode( GL_FRAGMENT_SHADER, p_fragmentText, p_method ) ),
				__success( 0 ),
				__id( 0 ),
				__infoLog( 0 ) {

					if( __vertexShader && __fragmentShader &&
						__vertexShader->ok( ) && __fragmentShader->ok( ) ) {

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

				Shader( CStr &p_name, CStr &p_vertexText, CStr &p_fragmentText, ShaderCode::CreationMethod const &p_method, std::vector< GLuint > const &p_locId, std::vector< Str > const &p_names ) :
				Named( p_name ),
				__vertexShader( new ShaderCode( GL_VERTEX_SHADER, p_vertexText, p_method ) ),
				__geometryShader( 0 ),
				__fragmentShader( new ShaderCode( GL_FRAGMENT_SHADER, p_fragmentText, p_method ) ),
				__success( 0 ) {

					if( __vertexShader && __fragmentShader &&
						__vertexShader->ok( ) && __fragmentShader->ok( ) ) {

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

				Shader( CStr &p_name, CStr &p_vertexText, CStr &p_geometryText, CStr &p_fragmentText, ShaderCode::CreationMethod const &p_method ) :
				Named( p_name ),
				__vertexShader( new ShaderCode( GL_VERTEX_SHADER, p_vertexText, p_method ) ),
				__geometryShader( new ShaderCode( GL_GEOMETRY_SHADER, p_geometryText, p_method ) ),
				__fragmentShader( new ShaderCode( GL_FRAGMENT_SHADER, p_fragmentText, p_method ) ),
				__success( 0 ),
				__id( 0 ),
				__infoLog( 0 ) {

					if( __vertexShader && __geometryShader && __fragmentShader &&
						__vertexShader->ok( ) && __geometryShader->ok( ) && __fragmentShader->ok( ) ) {

						__id = glCreateProgram( );
						glAttachShader( __id, __vertexShader->id( ) );
						glAttachShader( __id, __geometryShader->id( ) );
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
						glDeleteShader( __geometryShader->id( ) );
						glDeleteShader( __fragmentShader->id( ) );
					}
				}

				Shader( CStr &p_name, CStr &p_vertexText, CStr &p_geometryText, CStr &p_fragmentText, ShaderCode::CreationMethod const &p_method, std::vector< GLuint > const &p_locId, std::vector< Str > const &p_names ) :
				Named( p_name ),
				__vertexShader( new ShaderCode( GL_VERTEX_SHADER, p_vertexText, p_method ) ),
				__geometryShader( new ShaderCode( GL_GEOMETRY_SHADER, p_geometryText, p_method ) ),
				__fragmentShader( new ShaderCode( GL_FRAGMENT_SHADER, p_fragmentText, p_method ) ),
				__success( 0 ) {

					if( __vertexShader && __geometryShader && __fragmentShader &&
						__vertexShader->ok( ) && __geometryShader->ok( ) && __fragmentShader->ok( ) ) {

						__id = glCreateProgram( );
						glAttachShader( __id, __vertexShader->id( ) );
						glAttachShader( __id, __geometryShader->id( ) );
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
						glDeleteShader( __geometryShader->id( ) );
						glDeleteShader( __fragmentShader->id( ) );
					}
				}

				~Shader( ) {

					glDeleteProgram( __id );

					if( __infoLog ) {

						delete __infoLog;
					}
					if( __fragmentShader ) {

						delete __fragmentShader;
					}
					if( __geometryShader ) {

						delete __geometryShader;
					}
					if( __vertexShader ) {

						delete __vertexShader;
					}

					for( auto u : uniform ) {

						delete u.second;
					}
				}

			private :

				ShaderCode
				*__vertexShader,
				*__geometryShader,
				*__fragmentShader;

				GLint
				__success;

				GLuint
				__id;

				GLchar
				*__infoLog;

			public :


				enum
				TYPE {

					INT, UINT, FLOAT,
					VEC2, VEC3, VEC4,
					IVEC2, IVEC3, IVEC4,
					UVEC2, UVEC3, UVEC4,
					MAT2, MAT2x3, MAT2x4,
					MAT3x2, MAT3, MAT3x4,
					MAT4x2, MAT4x3, MAT4
				};

				enum
				SIZE {

					SCALAR = 0,
					ARR1 = 1,
					ARR2 = 2,
					ARR3 = 3,
					ARR4 = 4,
					ARR5 = 5,
					ARR6 = 6,
					ARR7 = 7,
					ARR8 = 8,
					ARR9 = 9,
					ARR10 = 10
				};

				class
				Uniform {

					public:

						TYPE
						type;

						SIZE
						size;

						void
						*data;

					public:

						Uniform( TYPE const & p_ut, SIZE const & p_us, void* p_data ) :
						type( p_ut ),
						size( p_us ),
						data( p_data ) {

						}
				};

				std::map< CStr, Uniform * >
				uniform;

			public:

				GLuint
				id( ) const {

					return __id;
				}

				bool
				ok( ) const {

					return __success;
				}

				void
				bind( ) {

					return glUseProgram( __id );
				}

				void
				release( ) {

					return glUseProgram( 0 );
				}

			private:

		public:

				void
				addUniform( CStr & p_name, TYPE p_ut, SIZE p_us, void * p_data ) {

					uniform[ p_name ] = new Uniform( p_ut, p_us, p_data );
				}

				void
				setUniformSamplerId( CStr & p_name, GLint p_samplerId ) {

					glUniform1i( glGetUniformLocation( __id, p_name.c_str( ) ), p_samplerId );
				}

				void
				sendUniforms( ) {

					for( auto u : uniform ) {

						GLint
						uLoc = glGetUniformLocation( __id, u.first.c_str( ) );

						if( u.second->size < 1 ) {

							switch( u.second->type ) {

								case INT : {

									glUniform1i( uLoc, ( static_cast< GLint * >( u.second->data ) )[ 0 ] );

									break;
								}

								case UINT : {

									glUniform1ui( uLoc, ( static_cast< GLuint * >( u.second->data ) )[ 0 ] );

									break;
								}

								case FLOAT : {

									glUniform1f( uLoc, ( static_cast< GLfloat * >( u.second->data ) )[ 0 ] );

									break;
								}

								case IVEC2 : {

									glUniform2i(
										uLoc,
										( static_cast< GLint * >( u.second->data ) )[ 0 ],
										( static_cast< GLint * >( u.second->data ) )[ 1 ] );

									break;
								}

								case IVEC3 : {

									glUniform3i(
										uLoc,
										( static_cast< GLint * >( u.second->data ) )[ 0 ],
										( static_cast< GLint * >( u.second->data ) )[ 1 ],
										( static_cast< GLint * >( u.second->data ) )[ 2 ] );

									break;
								}

								case IVEC4 : {

									glUniform4i(
										uLoc,
										( static_cast< GLint * >( u.second->data ) )[ 0 ],
										( static_cast< GLint * >( u.second->data ) )[ 1 ],
										( static_cast< GLint * >( u.second->data ) )[ 2 ],
										( static_cast< GLint * >( u.second->data ) )[ 3 ] );

									break;
								}

								case UVEC2 : {

									glUniform2ui(
										uLoc,
										( static_cast< GLuint * >( u.second->data ) )[ 0 ],
										( static_cast< GLuint * >( u.second->data ) )[ 1 ] );

									break;
								}

								case UVEC3 : {

									glUniform3ui(
										uLoc,
										( static_cast< GLuint * >( u.second->data ) )[ 0 ],
										( static_cast< GLuint * >( u.second->data ) )[ 1 ],
										( static_cast< GLuint * >( u.second->data ) )[ 2 ] );

									break;
								}

								case UVEC4 : {

									glUniform4ui(
										uLoc,
										( static_cast< GLuint * >( u.second->data ) )[ 0 ],
										( static_cast< GLuint * >( u.second->data ) )[ 1 ],
										( static_cast< GLuint * >( u.second->data ) )[ 2 ],
										( static_cast< GLuint * >( u.second->data ) )[ 3 ] );

									break;
								}

								case VEC2 : {

									glUniform2f(
										uLoc,
										( static_cast< GLfloat * >( u.second->data ) )[ 0 ],
										( static_cast< GLfloat * >( u.second->data ) )[ 1 ] );

									break;
								}

								case VEC3 : {

									glUniform3f(
										uLoc,
										( static_cast< GLfloat * >( u.second->data ) )[ 0 ],
										( static_cast< GLfloat * >( u.second->data ) )[ 1 ],
										( static_cast< GLfloat * >( u.second->data ) )[ 2 ] );

									break;
								}

								case VEC4 : {

									glUniform4f(
										uLoc,
										( static_cast< GLfloat * >( u.second->data ) )[ 0 ],
										( static_cast< GLfloat * >( u.second->data ) )[ 1 ],
										( static_cast< GLfloat * >( u.second->data ) )[ 2 ],
										( static_cast< GLfloat * >( u.second->data ) )[ 3 ] );

									break;
								}

								case MAT2 : {

									glUniformMatrix2fv( uLoc, 1, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT2x3 : {

									glUniformMatrix2x3fv( uLoc, 1, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT2x4 : {

									glUniformMatrix2x4fv( uLoc, 1, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT3x2 : {

									glUniformMatrix3x2fv( uLoc, 1, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT3 : {

									glUniformMatrix3fv( uLoc, 1, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT3x4 : {

									glUniformMatrix3x4fv( uLoc, 1, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT4x2 : {

									glUniformMatrix4x2fv( uLoc, 1, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT4x3 : {

									glUniformMatrix4x3fv( uLoc, 1, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT4 : {

									glUniformMatrix4fv( uLoc, 1, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}
							}
						}
						else {

							switch( u.second->type ) {

								case INT : {

									glUniform1iv( uLoc, u.second->size, static_cast< GLint * >( u.second->data ) );

									break;
								}

								case UINT : {

									glUniform1uiv( uLoc, u.second->size, static_cast< GLuint * >( u.second->data ) );

									break;
								}

								case FLOAT : {

									glUniform1fv( uLoc, u.second->size,static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case IVEC2 : {

									glUniform2iv( uLoc, u.second->size, static_cast< GLint * >( u.second->data ) );

									break;
								}

								case IVEC3 : {

									glUniform3iv( uLoc, u.second->size, static_cast< GLint * >( u.second->data ) );

									break;
								}

								case IVEC4 : {

									glUniform3iv( uLoc, u.second->size, static_cast< GLint * >( u.second->data ) );

									break;
								}

								case UVEC2 : {

									glUniform2uiv( uLoc, u.second->size, static_cast< GLuint * >( u.second->data ) );

									break;
								}

								case UVEC3 : {

									glUniform3uiv( uLoc, u.second->size, static_cast< GLuint * >( u.second->data ) );

									break;
								}

								case UVEC4 : {

									glUniform4uiv( uLoc, u.second->size, static_cast< GLuint * >( u.second->data ) );

									break;
								}

								case VEC2 : {

									glUniform2fv( uLoc, u.second->size, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case VEC3 : {

									glUniform3fv( uLoc, u.second->size, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case VEC4 : {

									glUniform4fv( uLoc, u.second->size, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT2 : {

									glUniformMatrix2fv( uLoc, u.second->size, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT2x3 : {

									glUniformMatrix2x3fv( uLoc, u.second->size, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT2x4 : {

									glUniformMatrix2x4fv( uLoc, u.second->size, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT3x2 : {

									glUniformMatrix3x2fv( uLoc, u.second->size, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT3 : {

									glUniformMatrix3fv( uLoc, u.second->size, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT3x4 : {

									glUniformMatrix3x4fv( uLoc, u.second->size, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT4x2 : {

									glUniformMatrix4x2fv( uLoc, u.second->size, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT4x3 : {

									glUniformMatrix4x3fv( uLoc, u.second->size, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}

								case MAT4 : {

									glUniformMatrix4fv( uLoc, u.second->size, false, static_cast< GLfloat * >( u.second->data ) );

									break;
								}
							}
						}
					}
				}

				GLint
				setVertexAttrib( std::string const &p_name, GLint p_size, GLenum p_type, GLboolean p_normalized, GLsizei p_stride, void const *p_offset ) {

					GLint
					var = glGetAttribLocation( __id,  p_name.c_str( ) );
					glEnableVertexAttribArray( var );
					glVertexAttribPointer( var, p_size, p_type, p_normalized, p_stride, p_offset );

					return var;
				}
		};

//		class Shader :
//		public Named {

//			public :

//				ShaderProgram
//				*prg;

//			public :

//				Shader( CStr &p_name, CStr &p_vertexText, CStr &p_fragmentText, ShaderCode::CreationMethod const &p_method ) :
//				Named( p_name ),
//				prg( new ShaderProgram( p_name, p_vertexText, p_fragmentText, p_method ) ) {
//				}

//				Shader( CStr &p_name, CStr &p_vertexText, CStr &p_fragmentText, ShaderCode::CreationMethod const &p_method, std::vector< GLuint > const &p_locId, std::vector< Str > const &p_names ) :
//				Named( p_name ),
//				prg( new ShaderProgram( p_name, p_vertexText, p_fragmentText, p_method, p_locId, p_names ) ) {
//				}

//				Shader( CStr &p_name, CStr &p_vertexText, CStr &p_geometryText, CStr &p_fragmentText, ShaderCode::CreationMethod const &p_method ) :
//				Named( p_name ),
//				prg( new ShaderProgram( p_name, p_vertexText, p_geometryText, p_fragmentText, p_method ) ) {
//				}

//				Shader( CStr &p_name, CStr &p_vertexText, CStr &p_geometryText, CStr &p_fragmentText, ShaderCode::CreationMethod const &p_method, std::vector< GLuint > const &p_locId, std::vector< Str > const &p_names ) :
//				Named( p_name ),
//				prg( new ShaderProgram( p_name, p_vertexText, p_geometryText, p_fragmentText, p_method, p_locId, p_names ) ) {
//				}

//				~Shader( ) {

//					delete prg;
//				}

//				void
//				bind( ) {

//					prg->bind( );
//				}

//				void
//				release( ) {

//					prg->release( );
//				}
//		};

		class FrameBuffer :
		public Named {

			public :

				GLuint
				id;

			public :

				FrameBuffer( CStr & p_name ) :
				Named( p_name ),
				id( 0 ) {

					glGenFramebuffers( 1, &id );
				}

				~FrameBuffer( ) {

					glDeleteFramebuffers( 1, &id);
				}

				void
				bind( ) {

					glBindFramebuffer( GL_FRAMEBUFFER, id );
				}

				void
				release( ) {

					glBindFramebuffer( GL_FRAMEBUFFER, 0 );
				}
		};

		class
		Program :
		public Named {

			private :

				GLRenderer
				* glr;

			public :

				Program( CStr &p_name, GLRenderer * p_glr ) :
				Named( p_name ),
				glr( p_glr ) {

				}

				~Program( ) {

				}

			public:

				Str
				frameBuffer;

				Str
				vertexArray;

				std::vector< Str >
				inTextures,
				outTextures;

				Str
				shader;

			public :

				void
				setFrameBuffer( CStr & p_frameBufferName ) {

					frameBuffer = p_frameBufferName;
				}

				void
				setVertexArray( CStr & p_vertexArrayName ) {

					vertexArray = p_vertexArrayName;
				}

				void
				addInTexture( CStr & p_textureName ) {

					inTextures.push_back( p_textureName );
				}

				void
				addOutTexture( CStr & p_textureName ) {

					outTextures.push_back( p_textureName );
				}

				void
				setShader( CStr & p_shaderName ) {

					shader = p_shaderName;
				}

				void
				build( ) {

					VertexArray
					*vaLoc = glr->va[ vertexArray ];

					vaLoc->bind( );

					Shader
					*shLoc = glr->sh[ shader ];

					shLoc->bind( );

					glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * vaLoc->arr.size( ), vaLoc->arr.data( ), GL_STATIC_DRAW );

					for( std::map< CStr, VertexArray::Attr >::const_iterator a = vaLoc->attr.cbegin( ); a != vaLoc->attr.cend( ); ++ a ) {

						CStr
						nm = a->first;

						GLint
						sz = a->second.size,
						of = a->second.offs,
						st = vaLoc->stride;

						shLoc->setVertexAttrib( nm, sz, GL_FLOAT, GL_FALSE, st * sizeof( GLfloat ), ( void* ) ( of * sizeof( GLfloat ) ) );
//						shLoc->prg->setVertexAttrib( a->first, a->second, GL_FLOAT, GL_FALSE, vaLoc->attrCount * sizeof( GLfloat ), ( void* ) ( vaLoc->attrOffset[ a->first ] * sizeof( GLfloat ) ) );
					}

					for( std::size_t i = 0; i < inTextures.size( ); ++ i ) {

						glActiveTexture( GL_TEXTURE0 + i );

						Texture
						*tx = glr->tx[ inTextures[ i ] ];

						tx->bind( );

						shLoc->setUniformSamplerId( tx->name( ).c_str( ), i );
					}

					for( std::size_t i = 0; i < inTextures.size( ); ++ i ) {

						glr->tx[ inTextures[ i ] ]->release( );
					}

					shLoc->release( );

					vaLoc->release( );
				}

				void
				run( ) {

					if( 0 < frameBuffer.length( ) ) {

						GLenum const
						drawBuffers[ 8 ] = {

							GL_COLOR_ATTACHMENT0,
							GL_COLOR_ATTACHMENT1,
							GL_COLOR_ATTACHMENT2,
							GL_COLOR_ATTACHMENT3,
							GL_COLOR_ATTACHMENT4,
							GL_COLOR_ATTACHMENT5,
							GL_COLOR_ATTACHMENT6,
							GL_COLOR_ATTACHMENT7
						};

						FrameBuffer
						*fb = glr->fb[ frameBuffer ];

						fb->bind( );

						for( GLuint i = 0; i < outTextures.size( ); ++ i ) {

							Texture
							*tx = glr->tx[ outTextures[ i ] ];

							tx->bind( );

							glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tx->id, 0 );
						}

						glDrawBuffers( outTextures.size( ), drawBuffers );
					}

					VertexArray
					*va = glr->va[ vertexArray ];

					va->bind( );

					Shader
					*sh = glr->sh[ shader ];

					sh->bind( );

					for( GLuint i = 0; i < inTextures.size( ); ++ i ) {

						glActiveTexture( GL_TEXTURE0 + i );

						Texture
						*tx = glr->tx[ inTextures[ i ] ];

						tx->bind( );

					}

					sh->sendUniforms( );
					//addUniformInt( tx->name( ).c_str( ), i );
					glDrawArrays( GL_TRIANGLE_STRIP, 0, va->vertexCount );

					for( GLuint i = 0; i < inTextures.size( ); ++ i ) {

						glr->tx[ inTextures[ i ] ]->release( );
					}

					sh->release( );

					va->release( );

					if( 0 < frameBuffer.size( ) ) {

						glr->fb[ frameBuffer ]->release( );

						for( GLuint i = 0; i < outTextures.size( ); ++ i ) {

							glr->tx[ outTextures[ i ] ]->release( );
						}
					}
			}
		};

	public :

		std::map< CStr, FrameBuffer * >
		fb;

		std::map< CStr, VertexArray * >
		va;

		std::map< CStr, Texture * >
		tx;

		std::map< CStr, Shader * >
		sh;

		std::map< CStr, Program * >
		pr;

		FrameBuffer
		*currentFrameBuffer;

		VertexArray
		*currentVertexArray;

		Shader
		*currentShader;

		Program
		*currentProgram;

	public :

		GLRenderer( ) {

		}

		~GLRenderer( ) {

			for( auto s : sh )

				delete s.second;

			for( auto t : tx )

				delete t.second;

			for( auto v : va )

				delete v.second;

			for( auto f : fb )

				delete f.second;

			for( auto p : pr )

				delete p.second;
		}

		VertexArray
		&vertices( CStr p_name ) {

			if( va.find( p_name ) == va.cend( ) ) {

				va[ p_name ] = new VertexArray( p_name );
			}

			currentVertexArray = va[ p_name ];

			return *currentVertexArray;
		}

		FrameBuffer
		&frameBuffer( CStr p_name ) {

			if( fb.find( p_name ) == fb.cend( ) ) {

				fb[ p_name ] = new FrameBuffer( p_name );
			}

			currentFrameBuffer = fb[ p_name ];

			return *currentFrameBuffer;
		}

		void
		addTexture( CStr p_name, Texture *p_texture ) {

			tx[ p_name ] = p_texture;
		}

		Shader
		&shader( CStr &p_name, CStr &p_vertexText, CStr &p_fragmentText, ShaderCode::CreationMethod const &p_method ) {

			if( sh.find( p_name ) == sh.cend( ) ) {

				sh[ p_name ] = new Shader( p_name, p_vertexText, p_fragmentText, p_method );
			}

			currentShader = sh[ p_name ];

			return *currentShader;
		}

		Shader
		&shader( CStr &p_name, CStr &p_vertexText, CStr &p_fragmentText, ShaderCode::CreationMethod const &p_method, std::vector< GLuint > const &p_locId, std::vector< Str > const &p_names ){

			if( sh.find( p_name ) == sh.cend( ) ) {

				sh[ p_name ] = new Shader( p_name, p_vertexText, p_fragmentText, p_method, p_locId, p_names );
			}

			currentShader = sh[ p_name ];

			return *currentShader;
		}

		Shader
		&shader( CStr &p_name, CStr &p_vertexText, CStr &p_geometryText, CStr &p_fragmentText, ShaderCode::CreationMethod const &p_method ) {

			if( sh.find( p_name ) == sh.cend( ) ) {

				sh[ p_name ] = new Shader( p_name, p_vertexText, p_geometryText, p_fragmentText, p_method );
			}

			currentShader = sh[ p_name ];

			return *currentShader;
		}

		Shader
		&shader( CStr &p_name, CStr &p_vertexText, CStr &p_geometryText, CStr &p_fragmentText, ShaderCode::CreationMethod const &p_method, std::vector< GLuint > const &p_locId, std::vector< Str > const &p_names ) {

			if( sh.find( p_name ) == sh.cend( ) ) {

				sh[ p_name ] = new Shader( p_name, p_vertexText, p_geometryText, p_fragmentText, p_method, p_locId, p_names );
			}

			currentShader = sh[ p_name ];

			return *currentShader;
		}

		Shader
		&shader( CStr &p_name ) {

			currentShader = sh[ p_name ];

			return *currentShader;
		}

		Program
		&program( CStr p_name ) {

			if( pr.find( p_name ) == pr.cend( ) ) {

				pr[ p_name ] = new Program( p_name, this );
			}

			currentProgram = pr[ p_name ];

			return *currentProgram;
		}

		void
		run( std::initializer_list< CStr > const & p_prg  ) {

			for( auto s : p_prg ) {

				pr[ s ]->run( );
			}
		}
};

#endif // GLRENDERER_HPP

