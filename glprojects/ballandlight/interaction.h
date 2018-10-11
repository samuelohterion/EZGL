#ifndef INTERACTION_H
#define INTERACTION_H

#include "../../../glm/glm/glm.hpp"
#include "../../../glm/glm/vec3.hpp" // glm::vec3
#include "../../../glm/glm/vec4.hpp" // glm::vec4
#include "../../../glm/glm/mat4x4.hpp" // glm::mat4
#include "../../../glm/glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "../../../glm/glm/gtc/type_ptr.hpp"

class Interaction
{
public:
	Interaction(glm::vec3 iVel, glm::vec3 iAcc, glm::vec3 iPos, glm::vec3 iG, float iRad );

	public:
	glm::vec3
			vel,
			acc,
			pos,
			g;

			float
			curTime,
			rad;
	public:
			glm::vec3
			Bounds(float time);

			void hitGround();
};

#endif // INTERACTION_H
