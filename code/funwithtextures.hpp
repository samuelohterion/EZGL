#ifndef FUNWITHTEXTURES_HPP
#define FUNWITHTEXTURES_HPP

#include <glrenderer.hpp>

class FunWithTextures :
public GLProject {

	FunWithTextures( CStr const & p_name = "FunWithTextures", ViewControlData * p_vcd = nullptr ) :
	GLProject( p_name, p_vcd ) {

	}

	void
	init( ) {
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

		glr.tx[ "TX1" ]->resize( vcd->width, vcd->height );
		glr.tx[ "TX2" ]->resize( vcd->width, vcd->height );
	}

	void
	paint( ) {

		glDisable( GL_DEPTH_TEST );
		glEnable( GL_CULL_FACE );

		glClear( GL_COLOR_BUFFER_BIT );

		glr.run( { "PRG1", "PRG2", "PRG3" } );
	}
};

#endif // FUNWITHTEXTURES_HPP
