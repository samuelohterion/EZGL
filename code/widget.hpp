#ifndef GL2DWIDGET_HPP
#define GL2DWIDGET_HPP

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QEvent>
#include <QTimer>
#include "../../glm/glm/glm.hpp"
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <map>
#include "clock.hpp"
#include "glrenderer.hpp"

namespace Ui {

	class Widget;
}

class GLWidget :
public QGLWidget {

		Q_OBJECT

	public:

		explicit
		GLWidget( QWidget *parent = nullptr );

		~GLWidget( );

	private:

		Ui::Widget
		*ui;

		QTimer
		timer;

		Clock
		clock;

	public:

//		GLRenderer
//		glr;

		QMap< QString, GLProject * >
		projects;

		QString
		currentProject;

		ViewControlData
		viewControlData;

	public:

		void
		keyPressEvent( QKeyEvent *p_keyEvent );

		void
		keyReleaseEvent( QKeyEvent *p_keyEvent );

		void
		mouseMoveEvent( QMouseEvent *p_mouseEvent );

		void
		mousePressEvent( QMouseEvent *p_mouseEvent );

		void
		mouseReleaseEvent( QMouseEvent *p_mouseEvent );

		void
		wheelEvent( QWheelEvent *p_wheelEvent );

	public:

		void
		initializeGL( );

		void
		resizeGL( int p_width, int p_height );

		void
		paintGL( );

	public:

		void
		updateShaderUniforms( );

		void
		toggleFullscreen( );

	public slots:

		void
		slotStartTimer( );

		void
		slotTimerFun( );

		void
		slotStopTimer( );
};

#endif // WIDGET_HPP
