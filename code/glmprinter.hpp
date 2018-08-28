#ifndef GLMPRINTER_HPP
#define GLMPRINTER_HPP

#include <iostream>
#include "../../glm/glm/glm.hpp"

typedef glm::vec2   V2;
typedef glm::vec3   V3;
typedef glm::vec4   V4;

typedef glm::mat4   M4;
typedef glm::mat4x3 M43;
typedef glm::mat4x2 M42;

typedef glm::mat3x4 M34;
typedef glm::mat3   M3;
typedef glm::mat3x2 M32;

typedef glm::mat2x4 M24;
typedef glm::mat2x3 M23;
typedef glm::mat2   M2;

void
print( glm::vec2 const & p_vec );

void
print( glm::vec3 const & p_vec );

void
print( glm::vec4 const & p_vec );

void
print( glm::mat2 const & p_mat );

void
print( glm::mat3 const & p_mat );

void
print( glm::mat4 const & p_mat );

#endif // GLMPRINTER_HPP
