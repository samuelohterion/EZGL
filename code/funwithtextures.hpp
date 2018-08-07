#ifndef FUNWITHTEXTURES_HPP
#define FUNWITHTEXTURES_HPP

#include <glrenderer.hpp>
#include "../../glm/glm/glm.hpp"
#include "../../glm/glm/gtc/matrix_transform.hpp"
#include "../../glm/glm/gtc/type_ptr.hpp"

class FunWithTextures :
public GLProject {

	public:

		FunWithTextures( CStr const & p_name = "FunWithTextures", ViewControlData * p_vcd = nullptr ) :
		GLProject( p_name, p_vcd ) {

		}

	public:

		// model view projection matrix
		glm::mat4
		projection,
		model,
		view,
		mvp,
		norm;

		GLfloat
		angle;

		void
		init( ) {

			glClearColor( 0., 0., 0., 1. );

			angle = vcd->time;

			// Create perspective projection matrix
			glm::mat4
			id( 1. );

			glm::vec2
			wh( vcd->width, vcd->height );

			projection = glm::perspective( 45.0f, wh.x / wh.y, 1.0f, 100.f );

			// Apply model view transformations
			model = id;
			view = glm::translate( id, glm::vec3( 0.f, 0.f, -4.f ) );
			view = glm::rotate( view, angle, glm::vec3( 0.f, 1.f, 0.f ) );

			mvp = projection * view * model;  // Calculate final MVP matrix
			norm = glm::transpose( glm::inverse( view * model ) );

			glr.shader( "TRIANGULAR-SHADER", "../shaders/mousecontrolled.vsh", "../shaders/phong.fsh", GLRenderer::ShaderCode::FROM_FILE );
			glr.shader( "TRIANGULAR-SHADER" ).addUniform( "mvp", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & mvp );
			glr.shader( "TRIANGULAR-SHADER" ).addUniform( "norm", GLRenderer::Shader::MAT4, GLRenderer::Shader::SCALAR, & norm );

			glr.vertices( "TRIANGULAR-VERTICES" )
			//      veritces            norms             colors
			//      x      y      z     nx    ny    nz    r     g     b
				<< -1. << -1. << +1. << 0. << 0. << 1. << 1. << 0. << 0.
				<< +1. << -1. << +1. << 0. << 0. << 1. << 0. << 1. << 0.
				<< +0. << +1. << +0. << 0. << 0. << 1. << 0. << 0. << 1.

				<< +1. << -1. << +1. << 1. << 0. << 0. << 0. << 1. << 0.
				<< +1. << -1. << -1. << 1. << 0. << 0. << 1. << 0. << 0.
				<< +0. << +1. << +0. << 1. << 0. << 0. << 0. << 0. << 1.

				<< +1. << -1. << -1. << 1. << 0. << 0. << 1. << 0. << 0.
				<< -1. << -1. << -1. << 1. << 0. << 0. << 0. << 1. << 0.
				<< +0. << +1. << +0. << 1. << 0. << 0. << 0. << 0. << 1.

				<< -1. << -1. << +1. << 1. << 0. << 0. << 1. << 0. << 0.
				<< -1. << -1. << -1. << 1. << 0. << 0. << 0. << 1. << 0.
				<< +0. << +1. << +0. << 1. << 0. << 0. << 0. << 0. << 1.;

			glr.vertices( "TRIANGULAR-VERTICES" ).addAttrib( "vertices", 3, 0 );
			glr.vertices( "TRIANGULAR-VERTICES" ).addAttrib( "norms",    3, 3 );
			glr.vertices( "TRIANGULAR-VERTICES" ).addAttrib( "colors",   3, 6 );

			glr.program( "TRIANGULAR-PROGRAM-1" ).setShader( "TRIANGULAR-SHADER" );
			glr.program( "TRIANGULAR-PROGRAM-1" ).setVertexArray( "TRIANGULAR-VERTICES" );
			glr.program( "TRIANGULAR-PROGRAM-1" ).build( );
	/*
			glr.shader( "S1", "../shaders/stage1.vsh", "../shaders/stage1.fsh", GLRenderer::ShaderCode::FROM_FILE );
			glr.shader( "S1" ).addUniform( "ratio", GLRenderer::Shader::VEC2,  GLRenderer::Shader::SCALAR, &s1Vars.ratio );
			glr.shader( "S1" ).addUniform( "cntr", GLRenderer::Shader::VEC2,  GLRenderer::Shader::SCALAR, &s1Vars.cntr );
			glr.shader( "S1" ).addUniform( "time", GLRenderer::Shader::FLOAT,  GLRenderer::Shader::SCALAR, &s1Vars.time );
			glr.shader( "S1" ).addUniform( "zoom", GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, &s1Vars.zoom );


			glr.shader( "S2", "../shaders/stage2.vsh", "../shaders/stage2.fsh", GLRenderer::ShaderCode::FROM_FILE );
			glr.shader( "S2" ).addUniform( "dims", GLRenderer::Shader::VEC2,  GLRenderer::Shader::SCALAR, &s2Vars.dims );
			glr.shader( "S2" ).addUniform( "time", GLRenderer::Shader::FLOAT,  GLRenderer::Shader::SCALAR, &s2Vars.time );

			glr.shader( "S3", "../shaders/stage3.vsh", "../shaders/stage3.fsh", GLRenderer::ShaderCode::FROM_FILE );
			glr.shader( "S3" ).addUniform( "dimsRec", GLRenderer::Shader::VEC2,  GLRenderer::Shader::SCALAR, &s3Vars.dimsRec );
		//	glr.sh[ "S3" ]->addUniform( "time", GLRenderer::Shader::FLOAT,  GLRenderer::Shader::SCALAR, &s2Vars.time );

			glr.frameBuffer( "FB1" );

			glr.texture(
				"TX1",
				new GLRenderer::Texture(
					"TX1",
					GL_TEXTURE_2D,
					0,
					GL_R32I,
					GL_NEAREST, GL_NEAREST,
					GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
					GL_RED_INTEGER, GL_INT,
					vcd->width, vcd->height
				)
			);

			glr.texture(
				"TX2",
				new GLRenderer::Texture(
					"TX2",
					GL_TEXTURE_2D,
					0,
					GL_RGBA32F,
					GL_NEAREST, GL_NEAREST,
					GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
					GL_RGBA, GL_FLOAT,
					vcd->width, vcd->height
				)
			);

			glr.vertices( "VA1" )
			//	    x      y
				<< -1. << -1.
				<< +1. << -1.
				<< -1. << +1.
				<< +1. << +1.;

			glr.vertices( "VA1" ).addAttrib( "verts", 2, 0 );

			glr.vertices( "VA2" )
			//	    x      y     s     t
				<< -1. << -1. << 0. << 0.
				<< +1. << -1. << 1. << 0.
				<< -1. << +1. << 0. << 1.
				<< +1. << +1. << 1. << 1.;

			glr.vertices( "VA2" ).addAttrib( "verts", 2, 0 );
			glr.vertices( "VA2" ).addAttrib( "coords", 2, 2 );

			glr.program( "PRG1" ).setFrameBuffer( "FB1" );
			glr.program( "PRG1" ).setShader( "S1" );
			glr.program( "PRG1" ).addOutTexture( "TX1" );
			glr.program( "PRG1" ).setVertexArray( "VA1" );
			glr.program( "PRG1" ).build( );

			glr.program( "PRG2" ).setFrameBuffer( "FB1" );
			glr.program( "PRG2" ).setShader( "S2" );
			glr.program( "PRG2" ).addInTexture( "TX1" );
			glr.program( "PRG2" ).addOutTexture( "TX2" );
			glr.program( "PRG2" ).setVertexArray( "VA2" );
			glr.program( "PRG2" ).build( );

			glr.program( "PRG3" ).setShader( "S3" );
			glr.program( "PRG3" ).addInTexture( "TX2" );
			glr.program( "PRG3" ).setVertexArray( "VA2" );
			glr.program( "PRG3" ).build( );
	*/	}

		void
		resize( int p_width, int p_height ) {

//			glr.tx[ "TX1" ]->resize( vcd->width, vcd->height );
//			glr.tx[ "TX2" ]->resize( vcd->width, vcd->height );
			glm::mat4
			id( 1. );

			glm::vec2
			wh( vcd->width, vcd->height );

			projection = glm::perspective( 45.0f, wh.x / wh.y, 1.0f, 100.f );

			// Apply model view transformations
//			model = id;
//			view = glm::translate( id, glm::vec3( 0.f, 0.f, -4.f ) );
//			view = glm::rotate( view, angle, glm::vec3( 0.f, 1.f, 1.f ) );

			mvp = projection * view * model;  // Calculate final MVP matrix
		}

		void
		paint( ) {

			angle = vcd->time;

			glm::mat4
			id( 1. );

//			glm::vec2
//			wh( vcd->width, vcd->height );

//			projection = glm::perspective( 45.0f, wh.x / wh.y, 1.0f, 100.f );

			// Apply model view transformations
//			model = id;
			view = glm::translate( id, glm::vec3( 0.f, 0.f, -4.f ) );
			view = glm::rotate( view, angle, glm::vec3( 0.f, 1.f, 0.f ) );

			mvp = projection * view * model;  // Calculate final MVP matrix

			glEnable( GL_DEPTH_TEST );
			glEnable( GL_CULL_FACE );

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//		glr.run( { "PRG1", "PRG2", "PRG3" } );
			glr.run( { "TRIANGULAR-PROGRAM-1" } );
		}
};

#endif // FUNWITHTEXTURES_HPP
