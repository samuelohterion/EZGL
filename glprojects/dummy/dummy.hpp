#ifndef DUMMY_HPP
#define DUMMY_HPP
#include "code/glr.hpp"

class Dummy :
public GLProject {

	public:

		Dummy( CStr const & p_name = "Dummy", ViewControlData *p_vcd = nullptr );

	public:

		glm::mat4
		m,//odel
		v,//iew
		p,//projection
		mvp;//model-view-projection

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );

};
#endif // COLOREDTRIANGLE3D_HPP
