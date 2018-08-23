#include "interaction.h"

Interaction::Interaction(glm::vec3 iVel, glm::vec3 iAcc, glm::vec3 iPos, glm::vec3 iG, float iRad )
{
	this->vel = iVel;
	this->acc = iAcc;
	this->pos = iPos;
	this->g = iG;
	this->curTime = 0;
	this->rad = iRad;
}

glm::vec3
Interaction::Bounds(float time){

	float dt = time-curTime;

	hitGround();
	glm::vec3 dV = dt*g;
	vel +=dV;
	pos += vel*dt;
	curTime = time;
	return pos;
}

void
Interaction::hitGround(){

	if(pos.y < (-1+rad) && vel.y < 0){
		vel.y *=-1;
		pos.y += (-1+rad)-pos.y;
	}
}
