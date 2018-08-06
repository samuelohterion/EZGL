#ifndef MYGLPROJECT_HPP
#define MYGLPROJECT_HPP

#include "glrenderer.hpp"
#include <QString>

class MyGLProject :
public GLProject {

	public:

		MyGLProject( CStr const & p_name, ViewControlData * p_vcd = nullptr );

		void
		init( );

		void
		resize( int p_width, int p_height );

		void
		paint( );
};

#endif // MYGLPROJECT_HPP
