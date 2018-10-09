#ifndef GLR_HPP
#define GLR_HPP
#ifndef GL_VERTEX_PROGRAM_POINT_SIZE
#define GL_VERTEX_PROGRAM_POINT_SIZE      0x8642
#endif
//	#ifndef GL_VERTEX_ATTRIB_ARRAY_NORMALIZED
//	#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
//	#endif

#include <initializer_list>
#include <fstream>
#include <istream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "../../glm/glm/glm.hpp"
#include "../../glm/glm/vec3.hpp" // glm::vec3
#include "../../glm/glm/vec4.hpp" // glm::vec4
#include "../../glm/glm/mat4x4.hpp" // glm::mat4
#include "../../glm/glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "../../glm/glm/gtc/type_ptr.hpp"
#include <GLES3/gl32.h>
#include "clock.hpp"
#include <QImage>
#include <QString>
#include <QGLWidget>

//#include <SOIL.h>

typedef std::string Str;
//typedef std::__cxx11::string Str;
typedef Str const CStr;

class
Named {

	private :

		Str
		__name;

	public :

		Named ( CStr const & p_name = "NONAME" ) :
		__name ( p_name ) {

		}

		Named ( Named const & p_name ) :
		__name ( p_name.name ( ) ) {

		}

		CStr
		name ( ) const {

			return __name;
		}

		void
		rename ( CStr & p_name ) {

			__name = p_name;
		}
};

struct
ViewControlData {

	GLint
	width,
	height,
	mousex,
	mousey,
	ticks;

	GLuint
	buttons;

	GLfloat
	time;

	glm::vec2
	aspect,
	dMouse;
};

class
GLR;

class
GLRRef {

	public :

	GLRRef ( GLR & p_glr ) :
	__glr ( p_glr ) {
	}

	private :

	GLR
	& __glr;

	public :

	GLR
	& glr ( ) {

		return __glr;
	}
};

class
GLR {

	public :

		GLenum const
		drawBuffers[ 0x20 ] = {

			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3,
			GL_COLOR_ATTACHMENT4,
			GL_COLOR_ATTACHMENT5,
			GL_COLOR_ATTACHMENT6,
			GL_COLOR_ATTACHMENT7,
			GL_COLOR_ATTACHMENT8,
			GL_COLOR_ATTACHMENT9,
			GL_COLOR_ATTACHMENT10,
			GL_COLOR_ATTACHMENT11,
			GL_COLOR_ATTACHMENT12,
			GL_COLOR_ATTACHMENT13,
			GL_COLOR_ATTACHMENT14,
			GL_COLOR_ATTACHMENT15,
			GL_COLOR_ATTACHMENT16,
			GL_COLOR_ATTACHMENT17,
			GL_COLOR_ATTACHMENT18,
			GL_COLOR_ATTACHMENT19,
			GL_COLOR_ATTACHMENT20,
			GL_COLOR_ATTACHMENT21,
			GL_COLOR_ATTACHMENT22,
			GL_COLOR_ATTACHMENT23,
			GL_COLOR_ATTACHMENT24,
			GL_COLOR_ATTACHMENT25,
			GL_COLOR_ATTACHMENT26,
			GL_COLOR_ATTACHMENT27,
			GL_COLOR_ATTACHMENT28,
			GL_COLOR_ATTACHMENT29,
			GL_COLOR_ATTACHMENT30,
			GL_COLOR_ATTACHMENT31
		};

		class CameraCenterView {

			public :

				CameraCenterView ( glm::mat4 const & p_model = glm::mat4 ( 1.f ), glm::mat4 const & p_view = glm::mat4( 1.f ), ViewControlData * p_vcd = nullptr, glm::vec3 const & p_par = glm::vec3( .01f, .01f, .1f ) ) :
				__model ( p_model ),
				__view ( p_view ),
				__loc ( glm::mat3 ( inverse ( __model ) ) ),
				__vcd ( p_vcd ),
				__par ( p_par ) {

				}

				~ CameraCenterView ( ) {

				}

			private :

				glm::mat4
				__model,
				__view;

				glm::mat3
				__loc;

				ViewControlData
				* __vcd;

				glm::vec3
				__par;

			public :

				CameraCenterView
				& fixModel( glm::mat4 const & p_model ) {

					__model = p_model;
					__loc   = inverse( __model );

					return *this;
				}

				CameraCenterView
				& fixView ( glm::mat4 const & p_view ) {

					__view = p_view;

					return *this;
				}

				CameraCenterView
				& fixVCD ( ViewControlData * p_viewControlData ) {

					__vcd = p_viewControlData;

					return *this;
				}

				void
				rotate_around_x( GLfloat const & p_angle ) {

					__model = glm::rotate( __model, p_angle, __loc[ 0 ] );
				}

				void
				rotate_around_y( GLfloat const & p_angle ) {

					__model = glm::rotate( __model, p_angle, __loc[ 1 ] );
				}

				void
				rotate_around_z( GLfloat const & p_angle ) {

					__model = glm::rotate( __model, p_angle, __loc[ 2 ] );
				}

				void
				rotate_around( GLfloat const & p_angle, glm::vec3 const & p_axis ) {

					__model = glm::rotate( __model, p_angle, p_axis * __loc );
				}

				glm::mat4
				model( ) const {

					return __model;
				}

				void
				move( GLfloat const & dS ) {

					__view = glm::translate( __view, glm::vec3( 0, 0, dS ) );
				}

				glm::mat4
				view( ) const {

					return __view;
				}

				glm::vec3
				param( ) const {

					return __par;
				}

				void
				setParam( glm::vec3 const & p_par = glm::vec3( .01f, .01f, .1f ) ) {

					__par = p_par;
				}

				void
				reactOnMouse( ) {

					glm::vec2
					dAngle = glm::vec2( __par ) * __vcd->dMouse;

					if ( __vcd->buttons & 0x02 )

						move( __par.z * __vcd->dMouse.y );

					if ( __vcd->ticks != 0 ) {

						move( .1f * __par.z * __vcd->ticks );
					}

					if ( __vcd->buttons & 0x01 ) {

						if( 0 < abs( dAngle.y ) ) {

							rotate_around_x( - dAngle.y );
						}

						if( 0 < abs( dAngle.x ) ) {

							rotate_around_y( + dAngle.x );
						}
					}
				}
			};

		class SpaceShipView {

			public :

			SpaceShipView ( glm::mat4 const & p_model, glm::mat4 const & p_view, ViewControlData * p_vcd ) :
			__model( p_model ),
			__view( p_view ),
			__vcd( p_vcd ) {

			}

			~SpaceShipView ( ) {

			}

			private :

				glm::mat4
				__model,
				__view;

				glm::mat3
				__loc;

				ViewControlData
				* __vcd;

			public :
		};

		enum MODE {

			OFFSCREEN,
			ONSCREEN
		};

		class FrameBuffer :
		public GLRRef {

			private:

				GLuint
				__id;

			public :

				FrameBuffer ( GLR & p_glr ) :
				GLRRef ( p_glr ),
				__id( 0 ) {

					glGenFramebuffers ( 1, & __id );
				}

				~ FrameBuffer ( ) {

					glDeleteFramebuffers ( 1, & __id );
				}

				std::vector< Str >
				outTextures,
				renderBuffers;

				bool
				fixedSize;

				GLsizei
				viewPortWidth,
				viewPortHeight;

				FrameBuffer
				& addOutTexture ( CStr & p_textureName ) {

					outTextures.push_back ( p_textureName );

					return * this;
				}

				FrameBuffer
				& addRenderBuffer ( CStr & p_colorRenderBufferName ) {

					renderBuffers.push_back ( p_colorRenderBufferName );

					return * this;
				}

				void
				bind ( ) {

					glBindFramebuffer ( GL_FRAMEBUFFER, __id );
				}

				GLuint
				id ( ) const {

					return __id;
				}

				void
				release ( ) {

					glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
				}

				void
				resize( GLsizei p_width = 0, GLsizei p_height = 0 ) {

					viewPortWidth = p_width;
					viewPortHeight = p_height;

					if( ! fixedSize ) {

						for( auto ot : outTextures ) {

							glr ( ).tx[ ot ]->resize( viewPortWidth, viewPortHeight );
						}
					}
				}

		};

		class Texture :
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
			GLsizei p_width = 32,
			GLsizei p_height = 32 ) :
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

						if( format == GL_DEPTH_COMPONENT ) {

							glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY );
							glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
							glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
						}

						glTexImage2D( target, level, internal_format, width, height, 0, format, type, nullptr );

						glBindTexture( GL_TEXTURE_2D, 0 );
			}

			Texture(
			CStr p_name, CStr p_filename ) :
			Named( p_name ),
			target( GL_TEXTURE_2D ),
			level( 0 ),
			internal_format( GL_RGBA32F ),
			min_filter( GL_NEAREST ),
			mag_filter( GL_NEAREST ),
			wrap_s( GL_CLAMP_TO_EDGE ),
			wrap_t( GL_CLAMP_TO_EDGE ),
			format( GL_RGBA ),
			type( GL_FLOAT ),
			width( 0 ),
			height( 0 ),
			id( 0 ) {

				QImage
				imgFromFile;

				if( ! imgFromFile.load( QString( p_filename.c_str( ) ) ) ) {

						std::cerr << "error loading " << p_filename << std::endl ;
						exit( 1 );
					}

					QImage
					GL_formatted_image;

					GL_formatted_image = QGLWidget::convertToGLFormat( imgFromFile );

					if( GL_formatted_image.isNull( ) ) {

							std::cerr << "error GL_formatted_image" << std::endl ;

							exit( 1 );
						}

						width  = GL_formatted_image.width( );
						height = GL_formatted_image.height( );

						glGenTextures( 1, &id );
						glBindTexture( target, id );

						glTexParameteri( target, GL_TEXTURE_MIN_FILTER, min_filter );
						glTexParameteri( target, GL_TEXTURE_MAG_FILTER, mag_filter );
						glTexParameteri( target, GL_TEXTURE_WRAP_S, wrap_s );
						glTexParameteri( target, GL_TEXTURE_WRAP_T, wrap_t );

						glTexImage2D( target, level, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, GL_formatted_image.bits( ) );

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

					//glTexStorage2D( target, 0, GL_RG32F, width, height );
					glTexImage2D( target, level, internal_format, width, height, 0, format, type, nullptr );

					glBindTexture( GL_TEXTURE_2D, 0 );
			}
		};

		class RenderBuffer :
		public Named {

			private:

			GLuint
			__id;

			GLenum
			__storageType;

			public :

			GLsizei
			width,
			height;

			public :

			RenderBuffer( CStr & p_name, GLenum const & p_storageType ) :
			Named( p_name ),
			__id( 0 ),
			__storageType( p_storageType ) {

				glGenRenderbuffers( 1, & __id );
			}

			~RenderBuffer( ) {

				glDeleteRenderbuffers( 1, & __id);
			}

			void
			bind( ) {

				glBindRenderbuffer( GL_RENDERBUFFER, __id );
			}

			GLuint
			id( ) const {

				return __id;
			}

			void
			release( ) {

				glBindFramebuffer( GL_RENDERBUFFER, 0 );
			}

			void
			resize( GLsizei p_width, GLsizei p_height ) {

				width = p_width;
				height = p_height;

					glBindRenderbuffer( GL_RENDERBUFFER, __id );

					glRenderbufferStorage( GL_RENDERBUFFER, __storageType, p_width, p_height );

					//glTexStorage2D( target, 0, GL_RG32F, width, height );
					//glTexImage2D( target, level, internal_format, width, height, 0, format, type, nullptr );

					glBindRenderbuffer( GL_RENDERBUFFER, 0 );
			}
		};

		class IndexArray :
		public Named {

			public :

			GLuint
			iaoId,
			iboId,
			stride;

			GLenum
			usage;

			class Object {

				public:

					Object( GLint p_offs, GLint p_size, GLenum p_mode ) :
					offs( p_offs ),
					size( p_size ),
					mode( p_mode ) {

					}

					GLint
					offs,
					size;

					GLenum
					mode;
			};

			std::vector< GLushort >
			arr;

			std::vector< Object >
			obj;

			public :

			IndexArray( CStr &p_name ) :
			Named( p_name ),
			iaoId( 0 ),
			iboId( 0 ),
			stride( 0 ),
			usage( GL_STATIC_DRAW ) {

				glGenVertexArrays( 1, &iaoId );
	//					glBindVertexArray( vaoId );

				glGenBuffers( 1, &iboId );
	//					glBindBuffer( GL_ARRAY_BUFFER, vboId );
	//					glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * arr.size( ), arr.data( ), GL_STATIC_DRAW );

	//					glBindVertexArray( 0 );
			}

			~IndexArray( ) {

			}

			IndexArray
			& operator << ( GLushort const & p_value ) {

				arr.push_back( p_value );

				return *this;
			}

			IndexArray
			& operator << ( Object const & p_object ) {

				obj.push_back( p_object );

					return *this;
			}


			IndexArray
				&setUsage( GLenum const & p_usage ) {

					usage = p_usage;

					return *this;
				}

				int
				indexCount( ) {

					return arr.size( );
				}

				void
				bind( ) {

					glBindVertexArray( iaoId );
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iboId );
				}

				void
				release( ) {

					glBindVertexArray( 0 );
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
				}
		};

		class VertexArray :
		public Named {

			public :

				GLuint
				vaoId,
				vboId,
				stride;

				GLenum
				usage;

				struct Attr {

					GLsizei offs;
					GLuint size;
				};

				class Object {

					public:

					Object( GLsizei p_offs, GLuint p_size, GLenum p_mode ) :
					offs( p_offs ),
					size( p_size ),
					mode( p_mode ) {

					}

					GLsizei
					offs;

					GLuint
					size;

					GLenum
					mode;
				};

				std::map< CStr, Attr >
				attr;

				std::vector< GLfloat >
				arr;

				std::vector< Object >
				obj;

			public :

				VertexArray( CStr &p_name ) :
				Named( p_name ),
				vaoId( 0 ),
				vboId( 0 ),
				stride( 0 ),
				usage( GL_STATIC_DRAW ) {

					glGenVertexArrays( 1, &vaoId );

					glGenBuffers( 1, &vboId );
				}

				~VertexArray( ) {

				}

				VertexArray
				&operator << ( GLfloat const & p_value ) {

					arr.push_back( p_value );

					return *this;
				}

				VertexArray
				&operator << ( Object const & p_object ) {

					obj.push_back( p_object );

					return *this;
				}

				VertexArray
				&operator << ( glm::vec2 const & p_value ) {

					arr.push_back( p_value.x );
					arr.push_back( p_value.y );

					return *this;
				}

				VertexArray
				&operator << ( glm::vec3 const & p_value ) {

					arr.push_back( p_value.r );
					arr.push_back( p_value.g );
						arr.push_back( p_value.b );

						return *this;
				}

				VertexArray
				&operator << ( glm::vec4 const & p_value ) {

					arr.push_back( p_value.r );
						arr.push_back( p_value.g );
						arr.push_back( p_value.b );
						arr.push_back( p_value.a );

						return *this;
				}

				VertexArray
					& attrib( std::string const & p_name, GLsizei const &p_offset, GLuint const &p_size ) {

						attr[ p_name ].offs = p_offset;
						attr[ p_name ].size = p_size;
						stride += p_size;

	//					vertexCount = arr.size( ) / stride;

						return *this;
					}

					VertexArray
					&setUsage( GLenum const & p_usage ) {

						usage = p_usage;

						return *this;
					}

					int
					vertexCount( ) {

						return arr.size( ) / stride;
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
				__infoLog( nullptr ) {

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

					glShaderSource( __id, 1, &cStr, nullptr );
					glCompileShader( __id );

					glGetShaderiv( __id, GL_COMPILE_STATUS, &__success );

						if( !__success ) {

							__infoLog = new GLchar[ 0x200 ];

							glGetShaderInfoLog( __id, 0x200, nullptr, __infoLog );

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

		class Shader :
		public Named {

			public :

				Shader( CStr &p_name, CStr &p_vertexText, CStr &p_fragmentText, ShaderCode::CreationMethod const &p_method ) :
				Named( p_name ),
				__vertexShader( new ShaderCode( GL_VERTEX_SHADER, p_vertexText, p_method ) ),
				__geometryShader( nullptr ),
				__fragmentShader( new ShaderCode( GL_FRAGMENT_SHADER, p_fragmentText, p_method ) ),
				__success( 0 ),
				__id( 0 ),
				__infoLog( nullptr ) {

					if( __vertexShader && __fragmentShader &&
					__vertexShader->ok( ) && __fragmentShader->ok( ) ) {

						__id = glCreateProgram( );
						glAttachShader( __id, __vertexShader->id( ) );
						glAttachShader( __id, __fragmentShader->id( ) );

						glLinkProgram( __id );

						glGetProgramiv( __id, GL_LINK_STATUS, &__success );

						if( !__success ) {

							__infoLog = new GLchar[ 0x200 ];

							glGetProgramInfoLog( __id, 0x200, nullptr, __infoLog );
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
				__geometryShader( nullptr ),
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

							glGetProgramInfoLog( __id, 0x200, nullptr, __infoLog );
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
				__infoLog( nullptr ) {

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

							glGetProgramInfoLog( __id, 0x200, nullptr, __infoLog );
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

								glGetProgramInfoLog( __id, 0x200, nullptr, __infoLog );
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
						* data;

					public:

						Uniform( TYPE const & p_ut = INT, SIZE const & p_us = SCALAR, void * p_data = nullptr ) :
						type( p_ut ),
						size( p_us ),
						data( p_data ) {

						}

						void
						set( TYPE const & p_ut, SIZE const & p_us, void* p_data ) {

							type = p_ut;
							size = p_us;
							data = p_data;
						}
				};

				std::map< CStr, Uniform >
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

					Shader
					& addUniform( CStr & p_name, TYPE p_ut, SIZE p_us, void * p_data ) {

						uniform[ p_name ] = Uniform( p_ut, p_us, p_data );

						return *this;
					}

					Shader
					& modifyUniform( CStr & p_name, TYPE p_ut, SIZE p_us, void * p_data ) {

						uniform[ p_name ].set( p_ut, p_us, p_data );

						return *this;
					}

					Shader
					& setUniformSamplerId( CStr & p_name, GLint p_samplerId ) {

						glUniform1i( glGetUniformLocation( __id, p_name.c_str( ) ), p_samplerId );

						return *this;
					}

					void
					sendUniforms( ) {

						for( auto u : uniform ) {

							GLint
							uLoc = glGetUniformLocation( __id, u.first.c_str( ) );

							if( u.second.size < 1 ) {

								switch( u.second.type ) {

									case INT : {

										glUniform1i( uLoc, ( static_cast< GLint * >( u.second.data ) )[ 0 ] );

										break;
									}

									case UINT : {

										glUniform1ui( uLoc, ( static_cast< GLuint * >( u.second.data ) )[ 0 ] );

										break;
									}

									case FLOAT : {

										glUniform1f( uLoc, ( static_cast< GLfloat * >( u.second.data ) )[ 0 ] );

										break;
									}

									case IVEC2 : {

										glUniform2i(
										uLoc,
										( static_cast< GLint * >( u.second.data ) )[ 0 ],
										( static_cast< GLint * >( u.second.data ) )[ 1 ] );

										break;
									}

									case IVEC3 : {

										glUniform3i(
										uLoc,
										( static_cast< GLint * >( u.second.data ) )[ 0 ],
										( static_cast< GLint * >( u.second.data ) )[ 1 ],
										( static_cast< GLint * >( u.second.data ) )[ 2 ] );

										break;
									}

									case IVEC4 : {

										glUniform4i(
										uLoc,
										( static_cast< GLint * >( u.second.data ) )[ 0 ],
										( static_cast< GLint * >( u.second.data ) )[ 1 ],
										( static_cast< GLint * >( u.second.data ) )[ 2 ],
										( static_cast< GLint * >( u.second.data ) )[ 3 ] );

										break;
									}

									case UVEC2 : {

										glUniform2ui(
										uLoc,
										( static_cast< GLuint * >( u.second.data ) )[ 0 ],
										( static_cast< GLuint * >( u.second.data ) )[ 1 ] );

										break;
									}

									case UVEC3 : {

										glUniform3ui(
										uLoc,
										( static_cast< GLuint * >( u.second.data ) )[ 0 ],
										( static_cast< GLuint * >( u.second.data ) )[ 1 ],
										( static_cast< GLuint * >( u.second.data ) )[ 2 ] );

										break;
									}

									case UVEC4 : {

										glUniform4ui(
										uLoc,
										( static_cast< GLuint * >( u.second.data ) )[ 0 ],
										( static_cast< GLuint * >( u.second.data ) )[ 1 ],
										( static_cast< GLuint * >( u.second.data ) )[ 2 ],
										( static_cast< GLuint * >( u.second.data ) )[ 3 ] );

										break;
									}

									case VEC2 : {

										glUniform2f(
										uLoc,
										( static_cast< GLfloat * >( u.second.data ) )[ 0 ],
										( static_cast< GLfloat * >( u.second.data ) )[ 1 ] );

										break;
									}

									case VEC3 : {

										glUniform3f(
										uLoc,
										( static_cast< GLfloat * >( u.second.data ) )[ 0 ],
										( static_cast< GLfloat * >( u.second.data ) )[ 1 ],
										( static_cast< GLfloat * >( u.second.data ) )[ 2 ] );

										break;
									}

									case VEC4 : {

										glUniform4f(
										uLoc,
										( static_cast< GLfloat * >( u.second.data ) )[ 0 ],
										( static_cast< GLfloat * >( u.second.data ) )[ 1 ],
										( static_cast< GLfloat * >( u.second.data ) )[ 2 ],
										( static_cast< GLfloat * >( u.second.data ) )[ 3 ] );

										break;
									}

									case MAT2 : {

										glUniformMatrix2fv( uLoc, 1, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT2x3 : {

										glUniformMatrix2x3fv( uLoc, 1, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT2x4 : {

										glUniformMatrix2x4fv( uLoc, 1, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT3x2 : {

										glUniformMatrix3x2fv( uLoc, 1, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT3 : {

										glUniformMatrix3fv( uLoc, 1, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT3x4 : {

										glUniformMatrix3x4fv( uLoc, 1, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT4x2 : {

										glUniformMatrix4x2fv( uLoc, 1, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT4x3 : {

										glUniformMatrix4x3fv( uLoc, 1, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT4 : {

										glUniformMatrix4fv( uLoc, 1, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}
								}
							}
							else {

								switch( u.second.type ) {

									case INT : {

										glUniform1iv( uLoc, u.second.size, static_cast< GLint * >( u.second.data ) );

										break;
									}

									case UINT : {

										glUniform1uiv( uLoc, u.second.size, static_cast< GLuint * >( u.second.data ) );

										break;
									}

									case FLOAT : {

										glUniform1fv( uLoc, u.second.size,static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case IVEC2 : {

										glUniform2iv( uLoc, u.second.size, static_cast< GLint * >( u.second.data ) );

										break;
									}

									case IVEC3 : {

										glUniform3iv( uLoc, u.second.size, static_cast< GLint * >( u.second.data ) );

										break;
									}

									case IVEC4 : {

										glUniform3iv( uLoc, u.second.size, static_cast< GLint * >( u.second.data ) );

										break;
									}

									case UVEC2 : {

										glUniform2uiv( uLoc, u.second.size, static_cast< GLuint * >( u.second.data ) );

										break;
									}

									case UVEC3 : {

										glUniform3uiv( uLoc, u.second.size, static_cast< GLuint * >( u.second.data ) );

										break;
									}

									case UVEC4 : {

										glUniform4uiv( uLoc, u.second.size, static_cast< GLuint * >( u.second.data ) );

										break;
									}

									case VEC2 : {

										glUniform2fv( uLoc, u.second.size, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case VEC3 : {

										glUniform3fv( uLoc, u.second.size, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case VEC4 : {

										glUniform4fv( uLoc, u.second.size, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT2 : {

										glUniformMatrix2fv( uLoc, u.second.size, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT2x3 : {

										glUniformMatrix2x3fv( uLoc, u.second.size, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT2x4 : {

										glUniformMatrix2x4fv( uLoc, u.second.size, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT3x2 : {

										glUniformMatrix3x2fv( uLoc, u.second.size, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT3 : {

										glUniformMatrix3fv( uLoc, u.second.size, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT3x4 : {

										glUniformMatrix3x4fv( uLoc, u.second.size, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT4x2 : {

										glUniformMatrix4x2fv( uLoc, u.second.size, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT4x3 : {

										glUniformMatrix4x3fv( uLoc, u.second.size, false, static_cast< GLfloat * >( u.second.data ) );

										break;
									}

									case MAT4 : {

										glUniformMatrix4fv( uLoc, u.second.size, false, static_cast< GLfloat * >( u.second.data ) );

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

		class Container :
		public Named {

			private :

				GLR
				* __glr;

			public :

				Container( CStr &p_name, GLR * p_glr ) :
				Named( p_name ),
				__glr( p_glr ) {

				}

				~Container( ) {

				}

			public:

				Str
				vertexArray;

				Str
				indexArray;

				std::vector< Str >
				inTextures;

				Str
				shader;

			public :

				Container
				& setVertexArray( CStr & p_vertexArrayName ) {

					vertexArray = p_vertexArrayName;

					return *this;
				}

				Container
				& setIndexArray( CStr & p_indexArrayName ) {

					indexArray = p_indexArrayName;

					return *this;
				}

				Container
				& addInTexture( CStr & p_textureName ) {

					inTextures.push_back( p_textureName );

					return *this;
				}

				Container
				& setShader( CStr & p_shaderName ) {

					shader = p_shaderName;

					return *this;
				}

				void
				build( ) {

					VertexArray
					*vaLoc = __glr->va[ vertexArray ];

					if( vaLoc ) {

						vaLoc->bind( );
					}

					IndexArray
					*iaLoc = __glr->ia[ indexArray ];

					if( iaLoc ) {

						iaLoc->bind( );
					}

					if( vaLoc ) {

						glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * vaLoc->arr.size( ), vaLoc->arr.data( ), vaLoc->usage );
					}

					if( iaLoc ) {

						glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( GLushort ) * iaLoc->arr.size( ), iaLoc->arr.data( ), iaLoc->usage );
					}

					Shader
					*shLoc = __glr->sh[ shader ];

					if( shLoc ) {

						shLoc->bind( );

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
					}

					for( std::size_t i = 0; i < inTextures.size( ); ++ i ) {

						glActiveTexture( GL_TEXTURE0 + i );

						Texture
						*tx = __glr->tx[ inTextures[ i ] ];

						tx->bind( );

						shLoc->setUniformSamplerId( tx->name( ).c_str( ), i );
					}

					for( std::size_t i = 0; i < inTextures.size( ); ++ i ) {

						__glr->tx[ inTextures[ i ] ]->release( );
					}

					if( shLoc ) {

						shLoc->release( );
					}

					if( iaLoc ) {

						iaLoc->release( );
					}

					if( vaLoc ) {

						vaLoc->release( );
					}
				}

				void
				run( ) {

					VertexArray
					* va = __glr->va[ vertexArray ];

					if( va ) {

						va->bind( );

						if( va->usage != GL_STATIC_DRAW ) {

							glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * va->arr.size( ), va->arr.data( ), va->usage );
						}
					}

					IndexArray
					*ia = __glr->ia[ indexArray ];

					if( ia ) {

						ia->bind( );

						if( ia->usage != GL_STATIC_DRAW ) {

							glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( GLushort ) * ia->arr.size( ), ia->arr.data( ), ia->usage );
						}
					}

					Shader
					*sh = __glr->sh[ shader ];

					if( sh ) {

						sh->bind( );

						sh->sendUniforms( );
					}

					for( GLuint i = 0; i < inTextures.size( ); ++ i ) {

						glActiveTexture( GL_TEXTURE0 + i );

						Texture
						*tx = __glr->tx[ inTextures[ i ] ];

						tx->bind( );
					}

					//addUniformInt( tx->name( ).c_str( ), i );

					if( va ) {

						for( auto i : va->obj ) {

							glDrawArrays( i.mode, i.offs, i.size );
						}
					}

					if( ia ) {

						for( auto i : ia->obj ) {

							glDrawElements( i.mode, i.size, GL_UNSIGNED_SHORT, ( GLvoid * ) ( i.offs * sizeof ( GLushort ) ) );
						}
					}

//					glDrawArrays( mode, 0, va->vertexCount );

					for( GLuint i = 0; i < inTextures.size( ); ++ i ) {

						__glr->tx[ inTextures[ i ] ]->release( );
					}

					if( sh ) sh->release( );

					if( va ) va->release( );

					if( ia ) ia->release( );
				}
		};

	public :

		FrameBuffer
		* fb;

		std::map< CStr, VertexArray * >
		va;

		std::map< CStr, IndexArray * >
		ia;

		std::map< CStr, Texture * >
		tx;

		std::map< CStr, Shader * >
		sh;

		std::map< CStr, Container * >
		co;

		VertexArray
		* currentVertexArray;

		IndexArray
		* currentIndexArray;

		Shader
		* currentShader;

		Container
		* currentContainer;

		MODE
		mode;

	public :

		GLR ( ) :
		fb ( nullptr ),
		mode ( ONSCREEN ) {

		}

		~GLR ( ) {

			for ( auto s : sh )

				delete s.second;

			for ( auto t : tx )

				delete t.second;

			for ( auto v : va )

				delete v.second;

			delete fb;

			for ( auto c : co )

				delete c.second;
		}

		VertexArray
		& vertices ( CStr p_name ) {

			if( va.find ( p_name ) == va.cend ( ) ) {

				va[ p_name ] = new VertexArray ( p_name );
			}

			currentVertexArray = va[ p_name ];

			return * currentVertexArray;
		}

		IndexArray
		& indices ( CStr & p_name ) {

			if( ia.find( p_name ) == ia.cend( ) ) {

				ia[ p_name ] = new IndexArray( p_name );
			}

			currentIndexArray = ia[ p_name ];

			return * currentIndexArray;
		}

		GLR
		& createOffScreen ( ) {

			if ( fb )

				return * this;

			fb = new FrameBuffer ( * this );

			return * this;
		}

		Texture
		& texture ( CStr & p_name, Texture * p_texture ) {

			tx[ p_name ] = p_texture;

			return * tx[ p_name ];
		}

		Shader
		& shader( CStr & p_name, CStr & p_vertexText, CStr & p_fragmentText, ShaderCode::CreationMethod const & p_method ) {

			if ( sh.find ( p_name ) == sh.cend ( ) ) {

				sh[ p_name ] = new Shader ( p_name, p_vertexText, p_fragmentText, p_method );
			}

			currentShader = sh[ p_name ];

			return * currentShader;
		}

		Shader
		& shader( CStr & p_name, CStr & p_vertexText, CStr & p_fragmentText, ShaderCode::CreationMethod const & p_method, std::vector < GLuint > const & p_locId, std::vector < Str > const & p_names ){

			if ( sh.find ( p_name ) == sh.cend ( ) ) {

				sh[ p_name ] = new Shader ( p_name, p_vertexText, p_fragmentText, p_method, p_locId, p_names );
			}

			currentShader = sh[ p_name ];

			return * currentShader;
		}

		Shader
		& shader( CStr & p_name, CStr & p_vertexText, CStr & p_geometryText, CStr & p_fragmentText, ShaderCode::CreationMethod const & p_method ) {

			if ( sh.find ( p_name ) == sh.cend ( ) ) {

				sh[ p_name ] = new Shader ( p_name, p_vertexText, p_geometryText, p_fragmentText, p_method );
			}

			currentShader = sh[ p_name ];

			return * currentShader;
		}

		Shader
		& shader ( CStr & p_name, CStr & p_vertexText, CStr & p_geometryText, CStr & p_fragmentText, ShaderCode::CreationMethod const & p_method, std::vector < GLuint > const & p_locId, std::vector< Str > const & p_names ) {

			if ( sh.find ( p_name ) == sh.cend ( ) ) {

				sh[ p_name ] = new Shader ( p_name, p_vertexText, p_geometryText, p_fragmentText, p_method, p_locId, p_names );
			}

			currentShader = sh[ p_name ];

			return * currentShader;
		}

		Shader
		& shader( CStr & p_name ) {

			currentShader = sh[ p_name ];

			return * currentShader;
		}

		Container
		& container ( CStr p_name ) {

			if ( co.find ( p_name ) == co.cend ( ) ) {

				co[ p_name ] = new Container ( p_name, this );
			}

			currentContainer = co[ p_name ];

			return * currentContainer;
		}

		void
		run ( std::initializer_list< CStr > const & p_containers ) {

			for ( auto c : p_containers ) {

				co[ c ]->run ( );
			}
		}

		void
		run ( std::vector < Str > const & p_containers  ) {

			for ( auto c : p_containers ) {

				co[ c ]->run ( );
			}
		}

		void
		screenoff ( ) {

			if ( mode == OFFSCREEN )

				return;

			if ( fb == nullptr )

				return;

			mode = OFFSCREEN;

			fb->bind( );

			GLuint
			colorAttachments = 0;

			for( auto t : fb->outTextures ) {

				Texture
				* txLoc = tx[ t ];

				txLoc->bind( );

				if( txLoc->format == GL_DEPTH_COMPONENT ) {

					glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, txLoc->id, 0 );
				}
				else if ( txLoc->format == GL_STENCIL_INDEX ) {

					glFramebufferTexture2D( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, txLoc->id, 0 );
				}
				else if ( txLoc->format == GL_DEPTH_STENCIL ) {

					glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, txLoc->id, 0 );
				}
				else {

					glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachments ++, GL_TEXTURE_2D, txLoc->id, 0 );
				}
			}

			if( 0 < colorAttachments ) {

				glDrawBuffers( colorAttachments, drawBuffers );
			}
			else {

				glDrawBuffer( GL_NONE );
			}
		}

		void
		screenon ( ) {

			if ( mode == ONSCREEN )

				return;

			mode = ONSCREEN;

			if ( fb == nullptr )

				return;

			fb->release( );

			for ( auto t : fb->outTextures ) {

				tx[ t ]->release( );
			}
		}
};

class GLProject :
public Named {

	public:

	explicit GLProject( CStr p_name ) :
	Named( p_name ),
	vcd( nullptr ) {

	}

	virtual
	~GLProject(  ) {

	}

	public:

	GLR
	glr;

	ViewControlData
	* vcd;

	void
	setViewControlData( ViewControlData * p_vcd ) {

		vcd = p_vcd;
	}

	virtual void
	init( ) = 0;

	virtual void
	resize( int p_width, int p_height ) = 0;

	void
	resizeViewport( int p_width, int p_height ) {

		if ( glr.fb != nullptr )

			glr.fb->resize( p_width, p_height );

	}

	virtual void
	paint( ) = 0;
};

#endif // GLR_HPP
