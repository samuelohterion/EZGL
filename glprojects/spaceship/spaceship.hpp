#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include "../../code/glr.hpp"

class SpaceShip :
public GLProject {

	public:

		SpaceShip( CStr const & p_name = "SpaceShip", ViewControlData * p_vcd = nullptr );

	public:

		glm::mat4
		model,
		view,
		projection,
		loc;

		GLfloat
		mass,
		density;

		glm::vec3
		angularDensities,
		angularMomentums,
		positions,
		velocities,
		accelerations,
		angles,
		momentsOfIntertia,
		torques;

		glm::vec3
		lightP,
		lightC;

		GLfloat
		dtime,
		tm,
		days;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );
};

#endif // SPACESHIP_HPP
