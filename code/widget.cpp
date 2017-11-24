#include "widget.hpp"
#include "ui_widget.h"

GL2DWidget::GL2DWidget( QWidget *parent ) :
QGLWidget( parent ),
ui( new Ui::Widget ) {

	ui->setupUi( this );

	QGLFormat
	glFormat;
	glFormat.setVersion( 3, 3 );
	glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
	glFormat.setSampleBuffers( true );
//	glFormat.setDoubleBuffer( true );

	this->setFormat( glFormat );

	clock.start( );

	connect( &timer, SIGNAL( timeout( ) ), this, SLOT( slotTimerFun( ) ) );

	timer.start( 1000. );
}

GL2DWidget::~GL2DWidget( ){

	glDeleteBuffers( 1, &vbo );
	glDeleteVertexArrays( 1, &vao );

	delete shader;

	delete ui;
}

void
GL2DWidget::keyReleaseEvent( QKeyEvent *p_keyEvent ) {

	if( p_keyEvent->key( ) == 'F' )

		toggleFullscreen( );
}

void GL2DWidget::mouseMoveEvent( QMouseEvent *p_mouseEvent ) {

	if( p_mouseEvent->buttons() | Qt::LeftButton ) {

		mouse = glm::vec2(
			p_mouseEvent->pos( ).x( ),
			height( ) - 1 - p_mouseEvent->pos( ).y( ) );
	}
}

void GL2DWidget::mousePressEvent( QMouseEvent *p_mouseEvent ) {

	if( p_mouseEvent->buttons() | Qt::LeftButton ) {

		mouse = glm::vec2(
			p_mouseEvent->pos( ).x( ),
			height( ) - 1 - p_mouseEvent->pos( ).y( ) );
	}
}

void GL2DWidget::mouseReleaseEvent( QMouseEvent *p_mouseEvent ) {

//	if( p_mouseEvent->buttons() | Qt::LeftButton ) {

//		mousex = width( ) / 2.;
//		mousey = height( ) / 2;
//	}
}



void
GL2DWidget::initializeGL( ) {

	createShaders( );

	createBufferObjects( );

	glClearColor( 0., 0., 0., 1. );

	dims = glm::vec2(
		this->width( ),
		this->height( ) );

	rdims.x = 1. / dims.x;
	rdims.y = 1. / dims.y;

	mouse.x = dims.x / 2.;
	mouse.y = dims.y / 2.;

	glViewport( 0, 0, dims.x, dims.y );
}

void
GL2DWidget::resizeGL( int p_width, int p_height ) {

	dims = glm::vec2(
		p_width,
		p_height );

	rdims.x = 1. / dims.x;
	rdims.y = 1. / dims.y;

	mouse.x = dims.x / 2.;
	mouse.y = dims.y / 2.;

	glViewport( 0, 0, p_width, p_height );
}

void
GL2DWidget::paintGL( ) {

	glDisable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClear( GL_COLOR_BUFFER_BIT );

	//glBindBuffer( GL_ARRAY_BUFFER, vbo );

	//glmatrices.slotSetModel( glm::rotate( glmatrices.model, .02f, glm::vec3( 1.1, .3, .3 ) ) );
	//view = glm::translate( view, glm::vec3( 0., 0., -.05 * sin( .01 * time ) ) );

	//glmatrices.pvm = proj * view * model;

	glBindVertexArray( vao );

	shader->bind( );

	float
	time = 1e-6 * clock.elapsedMicros( );

	shader->setUniformFloat( "time", time );
	shader->setUniformVec2F( "dims", &dims[ 0 ] );
	shader->setUniformVec2F( "rdims", &rdims[ 0 ] );
	shader->setUniformVec2F( "mouse", &mouse[ 0 ] );
	shader->setUniformVec3F( "mousebuttons", &mousebuttons[ 0 ] );


	glDrawArrays( GL_TRIANGLES, 0, 6 * 2 );
//	glLineWidth( 10 + 5 * sin( time * .00123 ) );
//	glDrawArrays( GL_LINES, 0, 3 * 2 * 6 );

	shader->release( );
	glBindVertexArray( 0 );
}

void
GL2DWidget::createShaders( ) {

	shader = new ShaderProgram( "../../shaders/vert.glsl", "../../shaders/frag.glsl", Shader::FROM_FILE );
}

void
GL2DWidget::createBufferObjects( ) {

	vertices = {

		-1, -1,
		+1, -1,
		+1, +1,

		+1, +1,
		-1, +1,
		-1, -1
	};

	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * vertices.size( ), &vertices[ 0 ], GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0 * sizeof( GLfloat ), ( void* ) ( 0 * sizeof( GLfloat ) ) );
	glEnableVertexAttribArray( 0 );

	glBindBuffer( GL_VERTEX_ARRAY, 0 );
	glBindVertexArray( 0 );
	glDisableVertexAttribArray( 0 );
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

	updateGL( );
}

void
GL2DWidget::slotStopTimer( ) {

	timer.stop( );
}



