#ifndef LIGHTTEST_HPP
#define LIGHTTEST_HPP

#include "../../code/glrenderer.hpp"

class LightTest :
public GLProject {

	public:

		LightTest( CStr const & p_name = "FunWithTextures", ViewControlData * p_vcd = nullptr );

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
