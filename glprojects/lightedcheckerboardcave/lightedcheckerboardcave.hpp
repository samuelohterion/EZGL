#ifndef LIGHTTEST_HPP
#define LIGHTTEST_HPP

#include "../../code/glr.hpp"

class LightedCheckerboardCave :
public GLProject {

	public:

		LightedCheckerboardCave( CStr const & p_name = "LightedCheckerboardCave" );

	public:

		glm::mat4
		model,
		view,
		projection;

		glm::vec3
		light1InModelSpacePosition,
		light2InCameraSpacePosition,
		light1InModelSpaceColor,
		light2InCameraSpaceColor;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );
};

#endif // LIGHTTEST_HPP
