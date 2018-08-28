#ifndef YETANOTHERSPHERE_HPP
#define YETANOTHERSPHERE_HPP

#include "../../code/glrenderer.hpp"

class YetAnotherSphere :
public GLProject {

	public:

		YetAnotherSphere( CStr const & p_name = "SomeText", ViewControlData * p_vcd = nullptr );

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
