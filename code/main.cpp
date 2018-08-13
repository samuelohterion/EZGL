#include "glprojects/cube/cube.hpp"
#include "glprojects/funIn2D/funIn2D.hpp"
#include "glprojects/lighttest/lighttest.hpp"
#include "widget.hpp"
#include <QApplication>

int
main( int argc, char *argv[ ] ) {

	QApplication
	a( argc, argv );

	GLWidget
	w;

	FunIn2D
	* glp = new FunIn2D( "p1" );

	Cube
	* fwt = new Cube( "p2" );

	LightTest
	* lt = new LightTest( "p3" );

	w.projects[ "p1" ] = glp;
	w.projects[ "p2" ] = fwt;
	w.projects[ "p3" ] = lt;

	w.currentProject = "p3";

	w.show( );

	return a.exec( );
}
