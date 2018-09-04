#include "widget.hpp"
#include <QApplication>
#include "glprojects/triangle2d/triangle2d.hpp"
#include "glprojects/quad3d/quad3d.hpp"
#include "glprojects/coordinateSystem/coordinatesystem.hpp"
#include "glprojects/cubewithtexture/cubewithtexture.hpp"
#include "glprojects/solarsystem/solarsystem.hpp"
#include "glprojects/zbuffertexture/zbuffertexture.hpp"
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
	* quad3D = new Quad3D( "Quad3D" );

	CoordinateSystem
	* coordinateSystem = new CoordinateSystem( "CoordinateSystem" );

	CubeWithTexture
	* cubeWithTexture = new CubeWithTexture( );

	SolarsSystem
	* solarsSystem = new SolarsSystem( "SolarsSystem" );

	ZBufferTexture
	* zBufferTexture = new ZBufferTexture( "ZBufferTexture" );

//	Cube
//	* cube = new Cube( "Cube" );

//	FunIn2D
//	* funIn2D = new FunIn2D( "FunIn2D" );

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


//	SphereWithTexture
//	* sphereWithTexture = new SphereWithTexture( );

	w.projects[ "Triangle2D" ] = triangle2D;
	w.projects[ "Quad3D" ] = quad3D;
	w.projects[ "CoordinateSystem" ] = coordinateSystem;
	w.projects[ "CubeWithTexture" ] = cubeWithTexture;
	w.projects[ "SolarsSystem" ] = solarsSystem;
	w.projects[ "ZBufferTexture" ] = zBufferTexture;
//	w.projects[ "Cube" ] = cube;
//	w.projects[ "FunIn2D" ] = funIn2D;
//	w.projects[ "LightedCheckerboardCave" ] = lightedCheckerboardCave;
//	w.projects[ "Sphere" ] = sphere;
//	w.projects[ "SphereDynamicalSurface" ] = sphereDynamicalSurface;
//	w.projects[ "RobertsLightTest2D" ] = robertsLightTest2D;
//	w.projects[ "BallAndLight" ] = ballAndLight;
//	w.projects[ "TextureTest" ] = textureTest;
//	w.projects[ "MultiSampling" ] = multiSampling;
//	w.projects[ "SomeText" ] = someText;
//	w.projects[ "YAS" ] = yetAnotherSphere;
//	w.projects[ "SphereWithTexture" ] = sphereWithTexture;

	w.currentProject = "ZBufferTexture";

	w.show( );

	return a.exec( );
}
