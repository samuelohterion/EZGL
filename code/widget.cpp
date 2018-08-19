#include "widget.hpp"
#include "ui_widget.h"

GLWidget::GLWidget( QWidget *parent ) :
QGLWidget( parent ),
ui( new Ui::Widget ) {

	ui->setupUi( this );

	QGLFormat
	glFormat;
	glFormat.setVersion( 3, 3 );
	glFormat.setProfile( QGLFormat::CoreProfile); // Requires >=Qt-4.8.0
	glFormat.setSampleBuffers( true );
	glFormat.setDoubleBuffer( true );

	this->setFormat( glFormat );

	clock.start( );

	connect( &timer, SIGNAL( timeout( ) ), this, SLOT( slotTimerFun( ) ) );

	timer.start( 1000. );
}

GLWidget::~GLWidget( ){

	for( auto & p : projects )

		delete p;

	delete ui;
}

void
GLWidget::keyPressEvent( QKeyEvent *p_keyEvent ) {

	p_keyEvent->accept( );

	if( !timer.isActive( ) )

		updateGL( );
}

void
GLWidget::keyReleaseEvent( QKeyEvent *p_keyEvent ) {

	if( p_keyEvent->key( ) == Qt::Key_F )

		toggleFullscreen( );

	if( p_keyEvent->key( ) == Qt::Key_Escape || p_keyEvent->key( ) == Qt::Key_Q )

		close( );

	p_keyEvent->accept( );

	if( !timer.isActive( ) )

		updateGL( );
}

void
GLWidget::mouseMoveEvent( QMouseEvent *p_mouseEvent ) {

	viewControlData.mousex = p_mouseEvent->pos( ).x( );
	viewControlData.mousey = height( ) - 1 - p_mouseEvent->pos( ).y( );

	p_mouseEvent->accept( );

	if( !timer.isActive( ) )

		updateGL( );
}

void
GLWidget::mousePressEvent( QMouseEvent *p_mouseEvent ) {

	viewControlData.mousex = p_mouseEvent->pos( ).x( );
	viewControlData.mousey = height( ) - 1 - p_mouseEvent->pos( ).y( );

	p_mouseEvent->accept( );

	if( !timer.isActive( ) )

		updateGL( );
}

void
GLWidget::mouseReleaseEvent( QMouseEvent *p_mouseEvent ) {

	viewControlData.mousex = p_mouseEvent->pos( ).x( );
	viewControlData.mousey = height( ) - 1 - p_mouseEvent->pos( ).y( );

	p_mouseEvent->accept( );

	if( !timer.isActive( ) )

		updateGL( );
}

void
GLWidget::wheelEvent( QWheelEvent *p_wheelEvent ) {

	viewControlData.ticks = p_wheelEvent->delta( );

	p_wheelEvent->accept( );

	if( !timer.isActive( ) )

		updateGL( );
}

void
GLWidget::initializeGL( ) {

	viewControlData.width = width( );
	viewControlData.height = height( );

	viewControlData.mousex = viewControlData.width >> 1;
	viewControlData.mousey = viewControlData.height >> 1;
	viewControlData.ticks = 0;

	viewControlData.buttons = 0;
	viewControlData.time    = 0;

	glViewport( 0, 0, viewControlData.width, viewControlData.height );
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
			viewControlVars.dims.x, viewControlVars.dims.y
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
*/

	if( projects.contains( currentProject ) ) {

		projects[ currentProject ]->setViewControlData( & viewControlData );
		projects[ currentProject ]->init( );
	}
/*
	glr.shader( "Fun1", "../shaders/fun1.vsh", "../shaders/fun1.fsh", GLRenderer::ShaderCode::FROM_FILE );

//	glm::vec2
//	ratio = viewControlData.ratio,
//	dims  = viewControlData.dims;

//	GLfloat
//	time  = viewControlData.time;

	glr.shader( "Fun1" ).addUniform( "width", GLRenderer::Shader::INT,  GLRenderer::Shader::SCALAR, & viewControlData.width);
	glr.shader( "Fun1" ).addUniform( "height", GLRenderer::Shader::INT,  GLRenderer::Shader::SCALAR, & viewControlData.height);
	glr.shader( "Fun1" ).addUniform( "time", GLRenderer::Shader::FLOAT,  GLRenderer::Shader::SCALAR, & viewControlData.time );

	glr.vertices( "VA" )
	//	    x      y
		<< -1. << -1.
		<< +1. << -1.
		<< -1. << +1.
		<< +1. << +1.;

	glr.vertices( "VA" ).addAttrib( "verts", 2, 0 );

	glr.program( "FUN" ).setShader( "Fun1" );
	glr.program( "FUN" ).setVertexArray( "VA" );
	glr.program( "FUN" ).build( );
*/
//	glClearColor( 0., 0., 0., 1. );
}

void
GLWidget::resizeGL( int p_width, int p_height ) {

	viewControlData.width = p_width;
	viewControlData.height = p_height;

	viewControlData.mousex = viewControlData.width >> 1;
	viewControlData.mousey = viewControlData.height >> 1;



	if( projects.contains( currentProject ) ) {

		projects[ currentProject ]->resizeViewport( p_width, p_height );
		projects[ currentProject ]->resize( p_width, p_height );
	}
//	glr.tx[ "TX1" ]->resize( viewControlVars.dims.x, viewControlVars.dims.y );
//	glr.tx[ "TX2" ]->resize( viewControlVars.dims.x, viewControlVars.dims.y );
}

void
GLWidget::paintGL( ) {

	if( projects.contains( currentProject ) ) {

		projects[ currentProject ]->paint( );
	}
}

void
GLWidget::toggleFullscreen( ) {

	setWindowState( windowState( ) ^ Qt::WindowFullScreen );
}

void
GLWidget::slotStartTimer( ) {

	timer.start( );
}

void
GLWidget::slotTimerFun( ) {

	timer.setInterval( 1000. / 60. );

	viewControlData.time = 1e-6f * clock.elapsedMicros( );

	updateGL( );
}

void
GLWidget::slotStopTimer( ) {

	timer.stop( );
}
