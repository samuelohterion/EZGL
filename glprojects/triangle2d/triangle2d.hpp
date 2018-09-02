#ifndef TRIANGLE2D_HPP
#define TRIANGLE2D_HPP
#include "code/glr.hpp"

class Triangle2D :
public GLProject {

	public:

		Triangle2D( CStr const & p_name = "Triangle2D", ViewControlData *p_vcd = nullptr );

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );

};
#endif // SIMPLETRIANGLE_HPP
