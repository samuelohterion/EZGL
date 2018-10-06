#include "widget.hpp"
#include <QApplication>
//#include "glprojects/triangle2d/triangle2d.hpp"
//#include "glprojects/quad3d/quad3d.hpp"
//#include "glprojects/coordinateSystem/coordinatesystem.hpp"
//#include "glprojects/cubewithtexture/cubewithtexture.hpp"
//#include "glprojects/solarsystem/solarsystem.hpp"
#include "glprojects/zbuffertexture/zbuffertexture.hpp"
#include "glprojects/dummy/dummy.hpp"
//#include "glprojects/cube/cube.hpp"
//#include "glprojects/funIn2D/funIn2D.hpp"
//#include "glprojects/lighttest/lighttest.hpp"
//#include "glprojects/sphere/sphere.hpp"
//#include "glprojects/spheredynamicsurface/spheredynamicsurface.hpp"
//#include "glprojects/robertslighttest2D/robertslighttest2D.hpp"
//#include "glprojects/ballandlight/ballandlight.hpp"
//#include "glprojects/texturetest/texturetest.hpp"
#include "glprojects/multisampling/multisampling.hpp"
#include "glprojects/mstest/mstest.hpp"
//#include "glprojects/sometext/sometext.hpp"
//#include "glprojects/yetanothersphere/yetanothersphere.hpp"
//#include "glprojects/space/space.hpp"
//#include "glprojects/sceneexample/sceneexample.hpp"
//#include "glprojects/spherewithtexture/spherewithtexture.hpp"
#include "glprojects/obj/obj.hpp"

int
main( int argc, char *argv[ ] ) {

	QApplication
	a( argc, argv );

	GLWidget
	w;

//	Triangle2D
//	* triangle2D = new Triangle2D( "Triangle2D" );

//	Quad3D
//	* quad3D = new Quad3D( "Quad3D" );

//	CoordinateSystem
//	* coordinateSystem = new CoordinateSystem( "CoordinateSystem" );

//	CubeWithTexture
//	* cubeWithTexture = new CubeWithTexture( );

//	SolarsSystem
//	* solarsSystem = new SolarsSystem( "SolarsSystem" );

	ZBufferTexture
	* zBufferTexture = new ZBufferTexture( "ZBufferTexture" );

	Dummy
	* dummy = new Dummy( );

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

	MultiSampling
	* multiSampling = new MultiSampling( "MultiSampling" );

	MSTest
	* mstest = new MSTest ( "MSTest" );

//	SomeText
//	* someText = new SomeText( );

//	YetAnotherSphere
//	* yetAnotherSphere = new YetAnotherSphere( "YAS " );

//	SphereWithTexture
//	* sphereWithTexture = new SphereWithTexture( );

//	Space
//	* space = new Space( );

//	SceneExample
//	* sceneExample = new SceneExample( "SceneExample" );

//	MTL
//	mtl;

//	mtl.fromFile ( "../obj/audi000tri.mtl" );

	ObjExample
	* objExample = new ObjExample ( );

	w.addGLProject( dummy );
//	w.addGLProject( triangle2D );
//	w.addGLProject( quad3D );
//	w.addGLProject( coordinateSystem );
//	w.addGLProject( cubeWithTexture );
//	w.addGLProject( solarsSystem );
	w.addGLProject( zBufferTexture );
//	w.addGLProject( yetAnotherSphere );
//	w.projects[ "Cube" ] = cube;
//	w.projects[ "FunIn2D" ] = funIn2D;
//	w.projects[ "LightedCheckerboardCave" ] = lightedCheckerboardCave;
//	w.projects[ "Sphere" ] = sphere;
//	w.addGLProject( sphereDynamicalSurface );
//	w.projects[ "RobertsLightTest2D" ] = robertsLightTest2D;
//	w.projects[ "BallAndLight" ] = ballAndLight;
//	w.projects[ "TextureTest" ] = textureTest;
	w.projects[ "MultiSampling" ] = multiSampling;
//	w.projects[ "SomeText" ] = someText;
//	w.projects[ "YAS" ] = yetAnotherSphere;
//	w.projects[ "Space" ] = space;
//	w.projects[ "SphereWithTexture" ] = sphereWithTexture;
//	w.projects[ "SceneExample" ] = sceneExample;

	//w.addGLProject( objExample );
	w.addGLProject( mstest );

	w.slotSetGLProject( "MSTest" );

	w.show( );

	return a.exec( );
}
