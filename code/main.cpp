#include "myglproject.hpp"
#include "widget.hpp"
#include <QApplication>

int
main( int argc, char *argv[ ] ) {

	QApplication
	a( argc, argv );

	GLWidget
	w;

	MyGLProject
	* glp = new MyGLProject( "p1" );

	w.projects[ "p1" ] = glp;
	w.currentProject = "p1";

	w.show( );

	return a.exec( );
}
