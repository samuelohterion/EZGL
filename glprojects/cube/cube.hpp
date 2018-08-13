#ifndef CUBE_HPP
#define CUBE_HPP

#include "code/glrenderer.hpp"

class Cube :
public GLProject {

	public:

		Cube( CStr const & p_name = "FunWithTextures", ViewControlData * p_vcd = nullptr );

	public:

		// model view projection matrix
		glm::mat4
		projection,
		model,
		view,
		mvp,
		norm;

		GLfloat
		angle;

		glm::vec3
		light;

		void
		init( );

		void
		resize( int p_width, int p_height );

		void
		paint( );
};

#endif // CUBE_HPP
