#ifndef GL2DWIDGET_HPP
#define GL2DWIDGET_HPP

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QTimer>
#include "clock.hpp"
#include "shaderprogram.hpp"
#include <glm/glm.hpp>
#include <GL/gl.h>
#include <GLES3/gl3.h>

namespace Ui {

	class Widget;
}

class GL2DWidget :
public QGLWidget {

		Q_OBJECT

	public:

		explicit
		GL2DWidget( QWidget *parent = 0 );

		~GL2DWidget( );

	private:

		Ui::Widget
		*ui;

		QTimer
		timer;

		Clock
		clock;

	public:

		std::vector< float >
		vertices;

		ShaderProgram
		*shader;

		GLuint
		vao,
        vbo;

        struct Vars {

			glm::vec2
			dims;

			glm::vec2
			mouse;

			float
            time;

        } vars;

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

	public:

		void
		initializeGL( );

		void
		resizeGL( int p_width, int p_height );

		void
		paintGL( );

	public:

		void
		createShaders( );

		void
		createBufferObjects( );

		bool
		createUniformBufferObjects( );

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
