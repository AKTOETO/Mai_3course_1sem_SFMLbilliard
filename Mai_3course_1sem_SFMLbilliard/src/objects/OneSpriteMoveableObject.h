#ifndef ONESPRITEMOVEABLEOBJECT_H
#define ONESPRITEMOVEABLEOBJECT_H

#include "MoveableObject.h"

struct OneSpriteMoveableObject:
	MoveableObject
{
	// ��������, ������������ �� ������
	std::shared_ptr<Sprite> m_sprite;
};

#endif // !ONESPRITEMOVEABLEOBJECT_H
