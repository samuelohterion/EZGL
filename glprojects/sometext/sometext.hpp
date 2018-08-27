#ifndef INTERACTION_HPP
#define INTERACTION_HPP

#include "../../code/glrenderer.hpp"

class SomeText :
public GLProject {

	public:

		SomeText( CStr const & p_name = "SomeText", ViewControlData * p_vcd = nullptr );

	public:

		glm::mat4
		model,
		view,
		projection;

		glm::vec2
		letter;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );
};

#endif // INTERACTION_HPP
