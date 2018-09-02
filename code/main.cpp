#include "widget.hpp"
#include <QApplication>
#include "glprojects/triangle2d/triangle2d.hpp"
#include "glprojects/quad3d/quad3d.hpp"
#include "glprojects/coordinateSystem/coordinatesystem.hpp"
//#include "glprojects/cube/cube.hpp"
//#include "glprojects/funIn2D/funIn2D.hpp"
//#include "glprojects/lighttest/lighttest.hpp"
//#include "glprojects/sphere/sphere.hpp"
//#include "glprojects/spheredynamicsurface/spheredynamicsurface.hpp"
//#include "glprojects/robertslighttest2D/robertslighttest2D.hpp"
//#include "glprojects/ballandlight/ballandlight.hpp"
//#include "glprojects/texturetest/texturetest.hpp"
//#include "glprojects/multisampling/multisampling.hpp"
//#include "glprojects/sometext/sometext.hpp"
//#include "glprojects/yetanothersphere/yetanothersphere.hpp"
//#include "glprojects/cubewithtexture/cubewithtexture.hpp"
//#include "glprojects/spherewithtexture/spherewithtexture.hpp"

int
main( int argc, char *argv[ ] ) {

	QApplication
	a( argc, argv );

	GLWidget
	w;

	Triangle2D
	* triangle2D = new Triangle2D( "Triangle2D" );

	Quad3D
	* quadD = new Quad3D( "Quad3D" );

	CoordinateSystem
	* coordinateSystem = new CoordinateSystem( "CoordinateSystem" );

//	FunIn2D
//	* funIn2D = new FunIn2D( "FunIn2D" );

//	Cube
//	* cube = new Cube( "Cube" );

//	LightedCheckerboardCave
//	* lightedCheckerboardCave = new LightedCheckerboardCave( "LightedCheckerboardCave" );

//	Sphere
//	* sphere = new Sphere( "Sphere" );

//	SphereDynamicalSurface
//	* sphereDynamicalSurface = new SphereDynamicalSurface( "SphereDynamicalSurface" );

//	RobertsLightTest2D
//	* robertsLightTest2D = new RobertsLightTest2D( "RobertsLightTest2D" );

//	BallAndLight
//	* ballAndLight = new BallAndLight( "BallAndLight" );

//	TextureTest
//	* textureTest = new TextureTest( "TextureTest" );

//	MultiSampling
//	* multiSampling = new MultiSampling( "MultiSampling" );

//	SomeText
//	* someText = new SomeText( );

//	YetAnotherSphere
//	* yetAnotherSphere = new YetAnotherSphere( "YAS " );

//	CubeWithTexture
//	* cubeWithTexture = new CubeWithTexture( );

//	SphereWithTexture
//	* sphereWithTexture = new SphereWithTexture( );

	w.projects[ "SimpleTriangle" ] = triangle2D;
	w.projects[ "SimpleColoredTriangle" ] = simpleColoredTriangle;
	w.projects[ "CoordinateSystem" ] = coordinateSystem;
//	w.projects[ "FunIn2D" ] = funIn2D;
//	w.projects[ "LightedCheckerboardCave" ] = lightedCheckerboardCave;
//	w.projects[ "Sphere" ] = sphere;
//	w.projects[ "SphereDynamicalSurface" ] = sphereDynamicalSurface;
//	w.projects[ "RobertsLightTest2D" ] = robertsLightTest2D;
//	w.projects[ "BallAndLight" ] = ballAndLight;
//	w.projects[ "Cube" ] = cube;
//	w.projects[ "TextureTest" ] = textureTest;
//	w.projects[ "MultiSampling" ] = multiSampling;
//	w.projects[ "SomeText" ] = someText;
//	w.projects[ "YAS" ] = yetAnotherSphere;
//	w.projects[ "CubeWithTexture" ] = cubeWithTexture;
//	w.projects[ "SphereWithTexture" ] = sphereWithTexture;

	w.currentProject = "SimpleTriangle";

	w.show( );

	return a.exec( );
}
