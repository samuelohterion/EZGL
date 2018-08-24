#ifndef ROBERTSLIGHTTEST_HPP
#define ROBERTSLIGHTTEST_HPP

#include "code/glrenderer.hpp"
#include <QString>

class RobertsLightTest2D :
public GLProject {

	public:

		RobertsLightTest2D( CStr const & p_name = "RobertsLightTest2D", ViewControlData * p_vcd = nullptr );

		void
		init( );

		void
		resize( int p_width, int p_height );

		void
		paint( );
};

#endif // ROBERTSLIGHTTEST_HPP

