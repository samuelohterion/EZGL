#include "widget.hpp"
#include "ui_widget.h"

GL2DWidget::GL2DWidget( QWidget *parent ) :
QGLWidget( parent ),
ui( new Ui::Widget ),
vao( 0 ),
vbo( 0 ) {

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

	vars.zoom = 1.;

	timer.start( 1000. );
}

GL2DWidget::~GL2DWidget( ){

	glDeleteBuffers( 1, &vbo );
	glDeleteVertexArrays( 1, &vao );

	delete shader;

	delete ui;
}

void
GL2DWidget::keyPressEvent( QKeyEvent *p_keyEvent ) {

}

void
GL2DWidget::keyReleaseEvent( QKeyEvent *p_keyEvent ) {

	if( p_keyEvent->key( ) == Qt::Key_F )

		toggleFullscreen( );

	if( p_keyEvent->key( ) == Qt::Key_Escape || p_keyEvent->key( ) == Qt::Key_Q )

		close( );
}

void
GL2DWidget::mouseMoveEvent( QMouseEvent *p_mouseEvent ) {

	if( p_mouseEvent->buttons( ) | Qt::LeftButton ) {

<<<<<<< HEAD
        vars.mouse = glm::vec2(
=======
		vars.mouse = glm::vec2(
>>>>>>> 207fb90fa53f4bf1f925c10a5eac51f7e54e16ab
			p_mouseEvent->pos( ).x( ),
			height( ) - 1 - p_mouseEvent->pos( ).y( ) );
	}
}

void
GL2DWidget::mousePressEvent( QMouseEvent *p_mouseEvent ) {

	if( p_mouseEvent->buttons( ) | Qt::LeftButton ) {

<<<<<<< HEAD
        vars.mouse = glm::vec2(
=======
		vars.mouse = glm::vec2(
>>>>>>> 207fb90fa53f4bf1f925c10a5eac51f7e54e16ab
			p_mouseEvent->pos( ).x( ),
			height( ) - 1 - p_mouseEvent->pos( ).y( ) );
	}
}

void
GL2DWidget::mouseReleaseEvent( QMouseEvent *p_mouseEvent ) {

//	if( p_mouseEvent->buttons() | Qt::LeftButton ) {

//		mousex = width( ) / 2.;
//		mousey = height( ) / 2;
//	}
}

void
GL2DWidget::wheelEvent( QWheelEvent *p_wheelEvent ) {

	vars.zoom *= ( 1. - p_wheelEvent->angleDelta( ).ry( ) / 12000. );

	std::cout << vars.zoom << std::endl;
}

void
GL2DWidget::initializeGL( ) {

	vars.dims = glm::vec2(
		this->width( ),
		this->height( ) );

//	fvars.rdims.x = 1. / fvars.dims.x;
//	fvars.rdims.y = 1. / fvars.dims.y;

	vars.mouse.x = vars.dims.x / 2.;
	vars.mouse.y = vars.dims.y / 2.;

	glViewport( 0, 0, vars.dims.x, vars.dims.y );

	createShaders( );

	createBufferObjects( );

	glClearColor( 0., 0., 0., 1. );

}

void
GL2DWidget::resizeGL( int p_width, int p_height ) {

	vars.dims = glm::vec2(
		p_width,
		p_height );

//	fvars.rdims.x = 1. / fvars.dims.x;
//	fvars.rdims.y = 1. / fvars.dims.y;

	vars.mouse.x = vars.dims.x / 2.;
	vars.mouse.y = vars.dims.y / 2.;

	glViewport( 0, 0, vars.dims.x, vars.dims.y );
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

	shader->bind( );

	glBindVertexArray( vao );

	shader->setUniformVec2F( "dims", &vars.dims[ 0 ] );
	shader->setUniformVec2F( "mouse", &vars.mouse[ 0 ] );
	shader->setUniformFloat( "time", vars.time );
	shader->setUniformFloat( "zoom", vars.zoom );
	shader->setUniformInt( "buttons", vars.buttons );

	//std::cout << vars.zoom << std::endl;

	glDrawArrays( GL_TRIANGLES, 0, 6 * 2 );
//	glLineWidth( 10 + 5 * sin( time * .00123 ) );
//	glDrawArrays( GL_LINES, 0, 3 * 2 * 6 );

	glBindVertexArray( 0 );

	glBindBuffer( GL_UNIFORM_BUFFER, 0 );

	shader->release( );
}

void
GL2DWidget::createShaders( ) {

	shader = new ShaderProgram( "../../shaders/m_vert.glsl", "../../shaders/m_frag.glsl", Shader::FROM_FILE );
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

	vars.time = 1e-6 * clock.elapsedMicros( );

	updateGL( );
}

void
GL2DWidget::slotStopTimer( ) {

	timer.stop( );
}
