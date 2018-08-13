#ifndef FUNIN2D_HPP
#define FUNIN2D_HPP

#include "code/glrenderer.hpp"
#include <QString>

class FunIn2D :
public GLProject {

	public:

		FunIn2D( CStr const & p_name, ViewControlData * p_vcd = nullptr );

		void
		init( );

		void
		resize( int p_width, int p_height );

		void
		paint( );
};

#endif // FUNIN2D_HPP
