#ifndef BALL_H
#define BALL_H

#include "OneSpriteMoveableObject.h"

// стрктура шара
struct Ball:
	OneSpriteMoveableObject
{
	// радиус шара
	float radius;
};

#endif
