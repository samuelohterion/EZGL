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
		view;

		GLfloat
		angle;

		glm::vec4
		lightModelSpace[ 6 ],
		lightCameraSpace[ 6 ];


		glm::vec3
		lightColors[ 6 ];

		void
		init( );

		void
		resize( int p_width, int p_height );

		void
		paint( );
};

#endif // CUBE_HPP
