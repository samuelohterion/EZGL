#ifndef FBTEST_HPP
#define FBTEST_HPP
#include "code/glr.hpp"

class FBTest :
public GLProject {

	public:

		FBTest( CStr const & p_name = "FBTest" );

	public:

		glm::mat4
		m,//odel
		v,//iew
		p,//projection
		vp,
		mvp;//model-view-projection

		GLR::CameraCenterView
		ccv;

		glm::vec4
		color;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );

};
#endif // FBTEST_HPP
