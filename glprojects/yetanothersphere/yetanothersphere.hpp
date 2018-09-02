#ifndef YETANOTHERSPHERE_HPP
#define YETANOTHERSPHERE_HPP

#include "../../code/glr.hpp"

class YetAnotherSphere :
public GLProject {

	public:

		YetAnotherSphere( CStr const & p_name = "YetAnotherSphere", ViewControlData * p_vcd = nullptr );

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

#endif // YETANOTHERSPHERE_HPP
