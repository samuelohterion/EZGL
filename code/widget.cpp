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

	viewControlData.dMouse = glm::vec2(
		p_mouseEvent->pos( ).x( ) - viewControlData.mousex,
		height( ) - 1 - p_mouseEvent->pos( ).y( ) - viewControlData.mousey );

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
	viewControlData.dMouse = glm::vec2( 0, 0 );

	viewControlData.buttons = p_mouseEvent->buttons( );

	p_mouseEvent->accept( );

	if( !timer.isActive( ) )

		updateGL( );
}

void
GLWidget::mouseReleaseEvent( QMouseEvent *p_mouseEvent ) {

	viewControlData.mousex = p_mouseEvent->pos( ).x( );
	viewControlData.mousey = height( ) - 1 - p_mouseEvent->pos( ).y( );
	viewControlData.dMouse = glm::vec2( 0, 0 );

	viewControlData.buttons = p_mouseEvent->buttons( );

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

	viewControlData.dMouse = glm::vec2( 0, 0 );

	viewControlData.buttons = 0;
	viewControlData.time    = 0;

	glViewport( 0, 0, viewControlData.width, viewControlData.height );

	if( projects.contains( currentProject ) ) {

		projects[ currentProject ]->setViewControlData( & viewControlData );
		projects[ currentProject ]->init( );
	}
}

void
GLWidget::resizeGL( int p_width, int p_height ) {

	viewControlData.width = p_width;
	viewControlData.height = p_height;

	viewControlData.aspect = glm::vec2( p_width, p_height ) / float( p_width < p_height ? p_height : p_width );

	viewControlData.mousex = viewControlData.width >> 1;
	viewControlData.mousey = viewControlData.height >> 1;

	if( projects.contains( currentProject ) ) {

		projects[ currentProject ]->resizeViewport( p_width, p_height );
		projects[ currentProject ]->resize( p_width, p_height );
	}
}

void
GLWidget::paintGL( ) {

	if( projects.contains( currentProject ) ) {

		projects[ currentProject ]->paint( );
	}

	viewControlData.dMouse = glm::vec2( 0.f, 0.f );

	viewControlData.ticks = 0;
}

void GLWidget::addGLProject( GLProject * p_glProject ) {

	projects[ p_glProject->name( ).c_str( ) ] = p_glProject;
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

void GLWidget::slotSetGLProject( QString const & p_name ) {

	currentProject = p_name;

	this->setWindowTitle( "EZGL - " + currentProject );
}
