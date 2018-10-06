#ifndef MSTEST_HPP
#define MSTEST_HPP
#include "code/glr.hpp"

class MSTest :
public GLProject {

	public:

		MSTest( CStr const & p_name = "MSTest", ViewControlData *p_vcd = nullptr );

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
#endif // MSTEST_HPP
