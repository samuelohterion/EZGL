#include "space.hpp"
#include "../../code/glmprinter.hpp"

SpaceShip:: SpaceShip ( CStr const & p_name , ViewControlData * p_vcd ) :
GLProject ( p_name, p_vcd ),
model( M4( 1 ) ),
view( glm::lookAt( V3 ( 0., 1., +0. ), V3 ( 0., 10., 0. ), V3 ( 0., 1., 0. ) ) ),
loc( M4( 1 ) ),
mass( 1 ),
density( 0 ),
angularDensities ( 0 ),
positions ( 0 ),
velocities ( 0 ),
accelerations ( 0 ),
angles ( 0 ),
momentsOfIntertia ( 0 ),
torques ( 0 ) {

}

SpaceShip:: ~ SpaceShip( ) {

}

void
SpaceShip:: init ( ) {

	// frame buffer
	{
	}

	// vertex arrays
	{
	}

	// index arrays
	{
	}

	// shaders
	{
	}

	// textures
	{
	}

	// containers
	{
	}
}

void
SpaceShip:: paint ( ) {

}

void
SpaceShip:: resize ( int p_width, int p_height ) {

}

Space::Space ( const CStr & p_name, ViewControlData *p_vcd ) :
GLProject ( p_name, p_vcd ) {

	spaceShip = new SpaceShip( p_name + "-001" );
}

Space::~ Space ( ) {

	delete spaceShip;
}

void
Space::init( ) {

	// frame buffer
	{
	}

	// vertex arrays
	{
	}

	// index arrays
	{
	}

	// shaders
	{
	}

	// textures
	{
	}

	// containers
	{
	}

	glClearColor( .01f, .02f, .03f, 1. );

	projection = model = glm::mat4( 1. );

	view = spaceShip->view;

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
}

void
Space::paint( ) {

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void
Space::resize( int p_width, int p_height ) {

	// get aspect ratio
	float
	ratio = ( 1.f * p_width / p_height );

	// create a projection matrix
	projection = glm::perspective( 30.f * 3.14159f / 180.f, ratio, 1.0f, 300.f );
}
