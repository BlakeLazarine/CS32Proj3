#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject{
public:
	Actor::Actor(StudentWorld* world, int imageID, double startX, double startY, int hitPts, 
		Direction dir = 0, int depth = 0, bool isDamageable = true, bool alivity = true, double size = 1.0);
	virtual ~Actor();

	virtual void doSomething() = 0;
	virtual void takeDmg(int dmg);
	virtual bool blocksPassage();

	bool isAlive();
	StudentWorld* getWorld();
private:
	int hp;
	bool alive;
	bool damageable;
	StudentWorld* m_world;
};






#endif // ACTOR_H_
