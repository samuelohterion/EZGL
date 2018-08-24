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
#include "glprojects/texturetest/texturetest.hpp"
#include "widget.hpp"
#include <QApplication>

int
main( int argc, char *argv[ ] ) {

	QApplication
	a( argc, argv );

	GLWidget
	w;

	FunIn2D
	* funIn2D = new FunIn2D( "FunIn2D" );

	Cube
	* cube = new Cube( "Cube" );

	LightedCheckerboardCave
	* lightedCheckerboardCave = new LightedCheckerboardCave( "LightedCheckerboardCave" );

	CoordinateSystem
	* coordinateSystem = new CoordinateSystem( "CoordinateSystem" );

	SimpleTriangle
	* simpleTriangle = new SimpleTriangle( "SimpleTriangle" );

	Sphere
	* sphere = new Sphere( "Sphere" );

	SimpleColoredTriangle
	* simpleColoredTriangle = new SimpleColoredTriangle( "SimpleColoredTriangle" );

	SphereDynamicalSurface
	* sphereDynamicalSurface = new SphereDynamicalSurface( "SphereDynamicalSurface" );

	RobertsLightTest2D
	* robertsLightTest2D = new RobertsLightTest2D( "RobertsLightTest2D" );

	BallAndLight
	* ballAndLight = new BallAndLight( "BallAndLight" );

	TextureTest
	* textureTest = new TextureTest( "TextureTest" );

	w.projects[ "FunIn2D" ] = funIn2D;
	w.projects[ "LightedCheckerboardCave" ] = lightedCheckerboardCave;
	w.projects[ "CoordinateSystem" ] = coordinateSystem;
	w.projects[ "SimpleTriangle" ] = simpleTriangle;
	w.projects[ "Sphere" ] = sphere;
	w.projects[ "SimpleColoredTriangle" ] = simpleColoredTriangle;
	w.projects[ "SphereDynamicalSurface" ] = sphereDynamicalSurface;
	w.projects[ "RobertsLightTest2D" ] = robertsLightTest2D;
	w.projects[ "BallAndLight" ] = ballAndLight;
	w.projects[ "Cube" ] = cube;
	w.projects[ "TextureTest" ] = textureTest;

	w.currentProject = "TextureTest";
	w.show( );

	return a.exec( );
}
