#ifndef SPHEREDYNAMICSURFACE_HPP
#define SPHEREDYNAMICSURFACE_HPP
#include "code/glr.hpp"

class SphereDynamicalSurface :
public GLProject {

	public:

		SphereDynamicalSurface( CStr const & p_name = "SphereDynamicalSurface", ViewControlData *p_vcd = nullptr );

	public:

		glm::mat4
		m,v,p,
		mvp;

		glm::mat3
		nrm;

		std::vector< glm::vec3 >
		vel,
		acc;

	public:

		void
		init( );

		void
		paint( );

		void
		resize( int p_width, int p_height );

		void
		simulate( );
	};
void
print( glm::vec4 const & p_vec );

void
print( glm::mat4 const & p_mat );
#endif // SPHEREDYNAMICSURFACE_HPP
