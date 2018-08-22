#ifndef BAllANDLIGHT_HPP
#define BAllANDLIGHT_HPP
#include "code/glrenderer.hpp"
#include "code/interaction.h"

class BallAndLight :
public GLProject {

	public:

		BallAndLight( CStr const & p_name, ViewControlData *p_vcd = nullptr );

	public:

		glm::mat4
		m,v,p,
		mv;

	glm::vec3
	light;

	glm::mat3
		nrm;

	Interaction*
	ballintr;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );
};
#endif
