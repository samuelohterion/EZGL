#ifndef ZBUFFERTEXTURE_HPP
#define ZBUFFERTEXTURE_HPP
#include "code/glr.hpp"

class ZBufferTexture :
public GLProject {

	public:

		ZBufferTexture( CStr const & p_name = "ZBufferTexture" );

	public:

		glm::mat4
		m,//odel
		v,//iew
		p,//projection
		mvp;//model-view-projection

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );

};
#endif // ZBUFFERTEXTURE_HPP
