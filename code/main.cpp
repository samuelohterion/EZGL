#include "glprojects/cube/cube.hpp"
#include "glprojects/funIn2D/funIn2D.hpp"
#include "glprojects/lighttest/lighttest.hpp"
#include "glprojects/coordinateSystem/coordinatesystem.hpp"
#include "glprojects/simpleTriangle/simpletriangle.hpp"
#include "glprojects/sphere/sphere.hpp"
#include "glprojects/spheredynamicsurface/spheredynamicsurface.hpp"
#include "glprojects/simplecoloredtriangle/simplecoloredtriangle.hpp"
#include "glprojects/robertslighttest2D/robertslighttest2D.hpp"
#include "glprojects/ballandlight/ballandlight.hpp"
#include "widget.hpp"
#include <QApplication>

int
main( int argc, char *argv[ ] ) {

	QApplication
	a( argc, argv );

	GLWidget
	w;

	FunIn2D
	* glp = new FunIn2D( "FunIn2D" );

	Cube
	* fwt = new Cube( "Cube" );

	LightTest
	* lt = new LightTest( "LightTest" );

	CoordinateSystem
	* cs = new CoordinateSystem( "CoordinateSystem" );

	SimpleTriangle
	* st = new SimpleTriangle( "SimpleTriangle" );

	Sphere
	* sp = new Sphere( "Sphere" );

	SimpleColoredTriangle
	* sct = new SimpleColoredTriangle( "SimpleColoredTriangle" );

	SphereDynamicalSurface
	* sds = new SphereDynamicalSurface( "SphereDynamicalSurface" );

	RobertsLightTest2D
	* rlt = new RobertsLightTest2D( "RobertsLightTest2D" );

	BallAndLight
	* bal = new BallAndLight( "BallAndLight" );

	w.projects[ "FunIn2D" ] = glp;
	w.projects[ "LightTest" ] = lt;
	w.projects[ "CoordinateSystem" ] = cs;
	w.projects[ "SimpleTriangle" ] = st;
	w.projects[ "Sphere" ] = sp;
	w.projects[ "SimpleColoredTriangle" ] = sct;
	w.projects[ "SphereDynamicalSurface" ] = sds;
	w.projects[ "RobertsLightTest2D" ] = rlt;
	w.projects[ "BallAndLight" ] = bal;
	w.projects[ "Cube" ] = fwt;

	w.currentProject = "BallAndLight";

	w.show( );

	return a.exec( );
}
