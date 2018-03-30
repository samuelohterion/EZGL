#ifndef GL2DWIDGET_HPP
#define GL2DWIDGET_HPP

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QEvent>
#include <QTimer>
#include "../../../includes/glm/glm/glm.hpp"
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <map>
#include "clock.hpp"
#include "glrenderer.hpp"

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

/*
 * std::vector< GLfloat >
		vertices;

		ShaderProgram
		*shader1,
		*shader2;

		GLuint
		vao,
		vbo,
		fbo,
		rbo,
		tbo;

		std::vector< GLfloat >
		tex;
*/
		GLRenderer
		glr;

		struct ViewControlVars {

			glm::vec2
			dims,
			ratio,
			mouse,
			dmouse;

			GLfloat
			time,
			zoom;

			GLuint
			buttons;

		} viewControlVars;

		struct Shader1Vars {

			glm::vec2
			dims,
			ratio,
			cntr;

			float
			zoom,
			time;
		} s1Vars;

		struct Shader2Vars {

			glm::vec2
			dims;

			float
			time;
		} s2Vars;

		struct Shader3Vars {

			glm::vec2
			dimsRec;
		} s3Vars;


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
