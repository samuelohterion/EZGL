#include "glmprinter.hpp"

void
print( glm::vec2 const & p_vec ) {

	std::cout << p_vec[ 0 ] << " " << p_vec[ 1 ];
}

void
print( glm::vec3 const & p_vec ) {

	std::cout << p_vec[ 0 ] << " " << p_vec[ 1 ] << " " << p_vec[ 2 ];
}

void
print( glm::vec4 const & p_vec ) {

	std::cout << p_vec[ 0 ] << " " << p_vec[ 1 ] << " " << p_vec[ 2 ] << " " << p_vec[ 3 ];
}

void
print( glm::mat2 const & p_mat ) {

	print( p_mat[ 0 ] );
	std::cout <<  std::endl;
	print( p_mat[ 1 ] );
}

void
print( glm::mat3 const & p_mat ) {

	print( p_mat[ 0 ] );
	std::cout <<  std::endl;
	print( p_mat[ 1 ] );
	std::cout <<  std::endl;
	print( p_mat[ 2 ] );
}

void
print( glm::mat4 const & p_mat ) {

	print( p_mat[ 0 ] );
	std::cout <<  std::endl;
	print( p_mat[ 1 ] );
	std::cout <<  std::endl;
	print( p_mat[ 2 ] );
	std::cout <<  std::endl;
	print( p_mat[ 3 ] );
}

