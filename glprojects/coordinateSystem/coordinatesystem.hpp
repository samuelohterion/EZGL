#ifndef COORDINATESYSTEM_HPP
#define COORDINATESYSTEM_HPP
#include "code/glr.hpp"

class CoordinateSystem :
public GLProject {

	public:

		CoordinateSystem( CStr const & p_name = "CoordinateSystem" );

	public:

		glm::mat4
		modelView,
		model,
		view,
		projection;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );

};
#endif // COORDINATESYSTEM_HPP
