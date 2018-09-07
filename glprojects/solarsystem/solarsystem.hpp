#ifndef SOLARSYSTEM_HPP
#define SOLARSYSTEM_HPP

#include "../../code/glr.hpp"

class SolarsSystem :
public GLProject {

	public:

		SolarsSystem( CStr const & p_name = "SolarsSystem", ViewControlData * p_vcd = nullptr );

	public:

		glm::mat4
		model,
		view,
		projection;

		glm::vec3
		lightP,
		lightC;

		GLfloat
		days;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );
};

#endif // SOLARSYSTEM_HPP
