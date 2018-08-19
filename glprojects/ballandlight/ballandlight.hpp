#ifndef BALLANDLIGHT_HPP
#define BALLANDLIGHT_HPP
#include "code/glrenderer.hpp"

class BallAndLight :
public GLProject {

	public:

		BallAndLight( CStr const & p_name, ViewControlData *p_vcd = nullptr );

	public:

		glm::mat4
		m,v,p,
		mv;

		glm::mat3
		nrm;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );
};
#endif // BALLANDLIGHT_HPP
