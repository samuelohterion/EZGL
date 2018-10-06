#ifndef MULTISAMPLING_HPP
#define MULTISAMPLING_HPP

#include "../../code/glr.hpp"

class MultiSampling :
public GLProject {

	public:

		MultiSampling( CStr & p_name = "MultiSampling" );

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
		faceIndex,
		sampleIndex,
		maxSamples;

		glm::vec2
		dPixPos;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );
};

#endif // MULTISAMPLING_HPP
