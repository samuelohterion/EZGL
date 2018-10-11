#include "widget.hpp"
#include <QApplication>
#include "glprojects/dummy/dummy.hpp"
#include "glprojects/triangle2d/triangle2d.hpp"
#include "glprojects/quad3d/quad3d.hpp"
#include "glprojects/coordinateSystem/coordinatesystem.hpp"
#include "glprojects/cubewithtexture/cubewithtexture.hpp"
#include "glprojects/solarsystem/solarsystem.hpp"
#include "glprojects/zbuffertexture/zbuffertexture.hpp"
#include "glprojects/cube/cube.hpp"
#include "glprojects/funIn2D/funIn2D.hpp"
#include "glprojects/lightedcheckerboardcave/lightedcheckerboardcave.hpp"
#include "glprojects/sphere/sphere.hpp"
#include "glprojects/spherewithtexture/spherewithtexture.hpp"
#include "glprojects/sometext/sometext.hpp"
#include "glprojects/texturetest/texturetest.hpp"
#include "glprojects/obj/obj.hpp"
//#include "glprojects/yetanothersphere/yetanothersphere.hpp"
//#include "glprojects/spheredynamicsurface/spheredynamicsurface.hpp"
//#include "glprojects/robertslighttest2D/robertslighttest2D.hpp"
//#include "glprojects/ballandlight/ballandlight.hpp"
//#include "glprojects/multisampling/multisampling.hpp"
//#include "glprojects/mstest/mstest.hpp"
//#include "glprojects/space/space.hpp"
//#include "glprojects/sceneexample/sceneexample.hpp"
//#include "glprojects/spherewithtexture/spherewithtexture.hpp"
#include "glprojects/fbtest/fbtest.hpp"

int
main( int argc, char *argv[ ] ) {

	QApplication
	a( argc, argv );

	GLWidget
	w;

	Dummy
	* dummy = new Dummy ( );

	Triangle2D
	* triangle2D = new Triangle2D( "Triangle2D" );

	Quad3D
	* quad3D = new Quad3D( "Quad3D" );

	CoordinateSystem
	* coordinateSystem = new CoordinateSystem( "CoordinateSystem" );

	CubeWithTexture
	* cubeWithTexture = new CubeWithTexture ( );

	SolarsSystem
	* solarsSystem = new SolarsSystem;

	ZBufferTexture
	* zBufferTexture = new ZBufferTexture ( );

	Cube
	* cube = new Cube ( "Cube" );

	FunIn2D
	* funIn2D = new FunIn2D( "FunIn2D" );

	LightedCheckerboardCave
	* lightedCheckerboardCave = new LightedCheckerboardCave( );

	Sphere
	* sphere = new Sphere( "Sphere" );

	SphereWithTexture
	* sphereWithTexture = new SphereWithTexture( );

	SomeText
	* someText = new SomeText( );

//	YetAnotherSphere
//	* yetAnotherSphere = new YetAnotherSphere( "YAS" );

//	SphereDynamicalSurface
//	* sphereDynamicalSurface = new SphereDynamicalSurface( "SphereDynamicalSurface" );

//	RobertsLightTest2D
//	* robertsLightTest2D = new RobertsLightTest2D( "RobertsLightTest2D" );

//	BallAndLight
//	* ballAndLight = new BallAndLight( "BallAndLight" );

	TextureTest
	* textureTest = new TextureTest;

//	MultiSampling
//	* multiSampling = new MultiSampling( "MultiSampling" );

//	MSTest
//	* mstest = new MSTest ( "MSTest" );


//	Space
//	* space = new Space( );

//	SceneExample
//	* sceneExample = new SceneExample( "SceneExample" );

//	MTL
//	mtl;

//	mtl.fromFile ( "../obj/audi000tri.mtl" );

	ObjExample
	* objExample = new ObjExample;

	FBTest
	* fbTest = new FBTest ( );

	w.addGLProject ( dummy );
	w.addGLProject ( triangle2D );
	w.addGLProject ( quad3D );
	w.addGLProject ( coordinateSystem );
	w.addGLProject ( cubeWithTexture );
	w.addGLProject ( solarsSystem );
	w.addGLProject ( zBufferTexture );
	w.addGLProject ( cube );
	w.addGLProject ( funIn2D );
	w.addGLProject ( lightedCheckerboardCave );
	w.addGLProject ( sphere );
	w.addGLProject ( sphereWithTexture );
	w.addGLProject ( someText );
	w.addGLProject ( textureTest );
	w.addGLProject ( objExample );
//	w.addGLProject( ballAndLight );
//	w.addGLProject( yetAnotherSphere );
//	w.addGLProject( sphereDynamicalSurface );
//	w.projects[ "RobertsLightTest2D" ] = robertsLightTest2D;
//	w.projects[ "MultiSampling" ] = multiSampling;

//	w.projects[ "YAS" ] = yetAnotherSphere;
//	w.projects[ "Space" ] = space;
//	w.projects[ "SceneExample" ] = sceneExample;
//	w.addGLProject( mstest );

	w.addGLProject ( fbTest );

	w.slotSetGLProject ( "ObjExample" );

	w.show( );

	return a.exec( );
}
