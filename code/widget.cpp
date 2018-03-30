#include "widget.hpp"
#include "ui_widget.h"

GL2DWidget::GL2DWidget( QWidget *parent ) :
QGLWidget( parent ),
ui( new Ui::Widget ) {

	ui->setupUi( this );

	QGLFormat
	glFormat;
	glFormat.setVersion( 3, 3 );
	glFormat.setProfile( QGLFormat::CoreProfile); // Requires >=Qt-4.8.0
//	glFormat.setSampleBuffers( true );
//	glFormat.setDoubleBuffer( true );

	this->setFormat( glFormat );

	clock.start( );

	connect( &timer, SIGNAL( timeout( ) ), this, SLOT( slotTimerFun( ) ) );

	viewControlVars.zoom = 1.;

	timer.start( 1000. );
}

GL2DWidget::~GL2DWidget( ){

	delete ui;
}

void
GL2DWidget::keyPressEvent( QKeyEvent *p_keyEvent ) {

	p_keyEvent->accept( );

	if( !timer.isActive( ) )

		updateGL( );
}

void
GL2DWidget::keyReleaseEvent( QKeyEvent *p_keyEvent ) {

	if( p_keyEvent->key( ) == Qt::Key_F )

		toggleFullscreen( );

	if( p_keyEvent->key( ) == Qt::Key_Escape || p_keyEvent->key( ) == Qt::Key_Q )

		close( );

	p_keyEvent->accept( );

	if( !timer.isActive( ) )

		updateGL( );
}

void
GL2DWidget::mouseMoveEvent( QMouseEvent *p_mouseEvent ) {

	if( p_mouseEvent->buttons( ) & Qt::LeftButton ) {

//		viewControlVars.mouse = glm::vec2(
//			p_mouseEvent->pos( ).x( ),
//			height( ) - 1 - p_mouseEvent->pos( ).y( ) );

//		s1Vars.cntr.x += viewControlVars.zoom * ( 2. * ( viewControlVars.mouse.x / viewControlVars.dims.x ) - 1. );
//		s1Vars.cntr.y += viewControlVars.zoom * ( 2. * ( viewControlVars.mouse.y / viewControlVars.dims.y ) - 1. );
	}

	p_mouseEvent->accept( );

	if( !timer.isActive( ) )

		updateGL( );
}

void
GL2DWidget::mousePressEvent( QMouseEvent *p_mouseEvent ) {

	if( p_mouseEvent->buttons( ) & Qt::LeftButton ) {

		viewControlVars.dmouse = viewControlVars.mouse;

		viewControlVars.mouse = glm::vec2(
			p_mouseEvent->pos( ).x( ),
			height( ) - 1 - p_mouseEvent->pos( ).y( ) );

		viewControlVars.dmouse = viewControlVars.mouse - viewControlVars.dmouse;

		s1Vars.cntr.x -= 2 * viewControlVars.dmouse.x / viewControlVars.dims.x * viewControlVars.zoom;
		s1Vars.cntr.y -= 2 * viewControlVars.dmouse.y / viewControlVars.dims.y * viewControlVars.zoom;
	}

	p_mouseEvent->accept( );

	if( !timer.isActive( ) )

		updateGL( );
}

void
GL2DWidget::mouseReleaseEvent( QMouseEvent *p_mouseEvent ) {

	p_mouseEvent->accept( );

	viewControlVars.mouse = glm::vec2( .5 * viewControlVars.dims.x, .5 * viewControlVars.dims.y );

	if( !timer.isActive( ) )

		updateGL( );
}

void
GL2DWidget::wheelEvent( QWheelEvent *p_wheelEvent ) {

	viewControlVars.zoom *= ( 1. - p_wheelEvent->angleDelta( ).ry( ) / 6000. );

	std::cout << viewControlVars.zoom << std::endl;

	p_wheelEvent->accept( );

	if( !timer.isActive( ) )

		updateGL( );
}

void
GL2DWidget::initializeGL( ) {

	viewControlVars.dims = glm::vec2( width( ), height( ) );

	viewControlVars.mouse.x = viewControlVars.dims.x / 2.;
	viewControlVars.mouse.y = viewControlVars.dims.y / 2.;
	viewControlVars.dmouse.x = 0.;
	viewControlVars.dmouse.y = 0.;

	viewControlVars.buttons = 0;
	viewControlVars.zoom = 100.;

	glViewport( 0, 0, viewControlVars.dims.x, viewControlVars.dims.y );

	glr.shader( "S1", "../../shaders/stage1.vsh", "../../shaders/stage1.fsh", GLRenderer::ShaderCode::FROM_FILE );
	glr.shader( "S1" ).addUniform( "ratio", GLRenderer::Shader::VEC2,  GLRenderer::Shader::SCALAR, &s1Vars.ratio );
	glr.shader( "S1" ).addUniform( "cntr", GLRenderer::Shader::VEC2,  GLRenderer::Shader::SCALAR, &s1Vars.cntr );
	glr.shader( "S1" ).addUniform( "time", GLRenderer::Shader::FLOAT,  GLRenderer::Shader::SCALAR, &s1Vars.time );
	glr.shader( "S1" ).addUniform( "zoom", GLRenderer::Shader::FLOAT, GLRenderer::Shader::SCALAR, &s1Vars.zoom );


	glr.shader( "S2", "../../shaders/stage2.vsh", "../../shaders/stage2.fsh", GLRenderer::ShaderCode::FROM_FILE );
	glr.shader( "S2" ).addUniform( "dims", GLRenderer::Shader::VEC2,  GLRenderer::Shader::SCALAR, &s2Vars.dims );
	glr.shader( "S2" ).addUniform( "time", GLRenderer::Shader::FLOAT,  GLRenderer::Shader::SCALAR, &s2Vars.time );

	glr.shader( "S3", "../../shaders/stage3.vsh", "../../shaders/stage3.fsh", GLRenderer::ShaderCode::FROM_FILE );
	glr.shader( "S3" ).addUniform( "dimsRec", GLRenderer::Shader::VEC2,  GLRenderer::Shader::SCALAR, &s3Vars.dimsRec );
//	glr.sh[ "S3" ]->addUniform( "time", GLRenderer::Shader::FLOAT,  GLRenderer::Shader::SCALAR, &s2Vars.time );

	glr.frameBuffer( "FB1" );

	glr.addTexture(
		"TX1",
		new GLRenderer::Texture(
			"TX1",
			GL_TEXTURE_2D,
			0,
			GL_R32I,
			GL_NEAREST, GL_NEAREST,
			GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
			GL_RED_INTEGER, GL_INT,
			viewControlVars.dims.x, viewControlVars.dims.y
		)
	);

	glr.addTexture(
		"TX2",
		new GLRenderer::Texture(
			"TX2",
			GL_TEXTURE_2D,
			0,
			GL_RGBA32F,
			GL_NEAREST, GL_NEAREST,
			GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
			GL_RGBA, GL_FLOAT,
			viewControlVars.dims.x, viewControlVars.dims.y
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

	glClearColor( 01., 0., 0., 1. );
}

void
GL2DWidget::resizeGL( int p_width, int p_height ) {

	viewControlVars.dims = glm::vec2(
		p_width,
		p_height );

	viewControlVars.mouse.x = viewControlVars.dims.x / 2.;
	viewControlVars.mouse.y = viewControlVars.dims.y / 2.;

	glViewport( 0, 0, viewControlVars.dims.x, viewControlVars.dims.y );

	glr.tx[ "TX1" ]->resize( viewControlVars.dims.x, viewControlVars.dims.y );
	glr.tx[ "TX2" ]->resize( viewControlVars.dims.x, viewControlVars.dims.y );
}

void
GL2DWidget::paintGL( ) {

	updateShaderUniforms( );

	glDisable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT );

	glr.run( { "PRG1", "PRG2", "PRG3" } );
}

void
GL2DWidget::updateShaderUniforms( ) {

	viewControlVars.ratio =
		viewControlVars.dims.x < viewControlVars.dims.y
			? glm::vec2( 1., viewControlVars.dims.y / viewControlVars.dims.x )
			: glm::vec2( viewControlVars.dims.x / viewControlVars.dims.y, 1. );

	s1Vars.dims = viewControlVars.dims;
	s1Vars.ratio = viewControlVars.ratio;
	s1Vars.zoom = viewControlVars.zoom;
	s1Vars.time = viewControlVars.time;

	s2Vars.dims = viewControlVars.dims;
	s2Vars.time = viewControlVars.time;

	s3Vars.dimsRec.x = 1. / viewControlVars.dims.x;
	s3Vars.dimsRec.y = 1. / viewControlVars.dims.y;
}

void
GL2DWidget::toggleFullscreen( ) {

	setWindowState( windowState( ) ^ Qt::WindowFullScreen );
}

void
GL2DWidget::slotStartTimer( ) {

	timer.start( );
}

void
GL2DWidget::slotTimerFun( ) {

	timer.setInterval( 1000. / 60. );

	viewControlVars.time = 1e-6 * clock.elapsedMicros( );

	updateGL( );
}

void
GL2DWidget::slotStopTimer( ) {

	timer.stop( );
}
