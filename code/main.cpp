#include "myglproject.hpp"
#include "funwithtextures.hpp"
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

	FunWithTextures
	* fwt = new FunWithTextures( "p2" );

	w.projects[ "p1" ] = glp;
	w.projects[ "p2" ] = fwt;

	w.currentProject = "p2";

	w.show( );

	return a.exec( );
}
