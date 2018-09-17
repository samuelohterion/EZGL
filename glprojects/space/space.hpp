#ifndef SPACE_HPP
#define SPACE_HPP

#include "../../code/glr.hpp"
#include "../../code/glmprinter.hpp"

class SpaceShip :
public GLProject {

	public :

		SpaceShip ( const CStr & p_name, ViewControlData * p_vcd = nullptr );

		~ SpaceShip ( );

	public :

		M4
		model,
		view,
		loc;

		GLfloat
		mass,
		density;

		V3
		angularDensities,
		angularMomentums,
		positions,
		velocities,
		accelerations,
		angles,
		momentsOfIntertia,
		torques;

	public :

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );
};

class Space :
public GLProject {

	public:

		Space ( CStr const & p_name = "SpaceShip", ViewControlData * p_vcd = nullptr );

		~ Space ( );

	public:

		M4
		model,
		view,
		projection;

		SpaceShip
		* spaceShip;

	public:

		void
		init ( );

		void
		paint ( );

		void
		resize ( int p_width, int p_height );
};

#endif // SPACE_HPP
