#ifndef TEXTURETEST_HPP
#define TEXTURETEST_HPP

#include "../../code/glrenderer.hpp"

class TextureTest :
public GLProject {

	public:

		TextureTest( CStr const & p_name = "TextureTest", ViewControlData * p_vcd = nullptr );

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

		int
		faceIndex;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );
};

#endif // TEXTURETEST_HPP
