#ifndef DIRT_H
#define DIRT_H

#include "Actor.h"

class Dirt : public Actor {
public:
	Dirt(StudentWorld* world, double startX, double startY);

	void doSomething();
	bool blocksPassage();
};

#endif // !DIRT_H
