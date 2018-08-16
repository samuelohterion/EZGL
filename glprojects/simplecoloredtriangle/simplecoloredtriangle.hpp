#ifndef SIMPLECOLOREDTRIANGLE_HPP
#define SIMPLECOLOREDTRIANGLE_HPP
#include "code/glrenderer.hpp"

class SimpleColoredTriangle :
public GLProject {

	public:

		SimpleColoredTriangle( CStr const & p_name, ViewControlData *p_vcd = nullptr );

	public:

		glm::mat4
		m,v,p,
		mvp;


	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );

};
#endif // SIMPLECOLOREDTRIANGLE_HPP
