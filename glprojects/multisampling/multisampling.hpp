#ifndef MULTISAMPLING_HPP
#define MULTISAMPLING_HPP

#include "../../code/glrenderer.hpp"

class MultiSampling :
public GLProject {

	public:

		MultiSampling( CStr const & p_name = "MultiSampling", ViewControlData * p_vcd = nullptr );

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
