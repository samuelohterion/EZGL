#ifndef CUBE_HPP
#define CUBE_HPP

#include "code/glr.hpp"

class Cube :
public GLProject {

	public:

		Cube( CStr const & p_name = "Cube", ViewControlData * p_vcd = nullptr );

	public:

		// model view projection matrix
		glm::mat4
		projection,
		model,
		view;

		GLfloat
		angle;

		glm::vec4
		lightModelSpace[ 7 ],
		lightCameraSpace[ 7 ];

		glm::vec3
		lightColors[ 7 ];

		bool
		textureBuilt;

		int
		textureWidth,
		textureHeight;

		void
		init( );

		void
		resize( int p_width, int p_height );

		void
		paint( );
};

#endif // CUBE_HPP
