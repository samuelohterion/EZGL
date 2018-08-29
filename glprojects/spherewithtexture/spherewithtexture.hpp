#ifndef SPHEREWITHTEXTURE_HPP
#define SPHEREWITHTEXTURE_HPP

#include "../../code/glrenderer.hpp"

class SphereWithTexture :
public GLProject {

	public:

		SphereWithTexture( CStr const & p_name = "SphereWithTexture", ViewControlData * p_vcd = nullptr );

	public:

		glm::mat4
		model,
		view,
		projection;

		glm::vec3
		lightP,
		lightC;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );
};

#endif // YETANOTHERSPHERE_HPP
