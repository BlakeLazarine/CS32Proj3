#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor: public GraphObject{
public:
	Actor(StudentWorld* world, int imageID, double startX, double startY, int hitPts, 
		Direction dir = 0, int depth = 0, bool isDamageable = true, bool alivity = true, double size = 1.0);
	virtual ~Actor();

	virtual void doSomething() = 0;
	virtual void takeDmg(int dmg);
	virtual bool blocksPassage();
	virtual bool blocksPlacement();

	void die();
	bool isAlive();
	StudentWorld* getWorld();
private:
	int hp;
	bool alive;
	bool damageable;
	StudentWorld* m_world;
};


class Dirt : public Actor {
public:
	Dirt(double startX, double startY);

	void doSomething();
	bool blocksPassage();
};

class Food : public Actor {
public:
	Food(double startX, double startY);
	
	void doSomething();
	bool blocksPlacement();
};

class Socrates : public Actor {
public:
	Socrates(StudentWorld* world);

	void doSomething();
private:
	int m_sprays;
	int m_flames;
	int m_pos;
};



class Projectile : public Actor {
public:
	Projectile(int dmg, int lifespan, StudentWorld* world, int imageID, double startX, double startY,
		Direction dir = 0, int depth = 0, double size = 1.0);
		//Actor(world, imageID, startX, startY, 0, dir, depth, false, true, size)
	void doSomething();
private:
	int m_dmg, m_lifespan;
};

class Flame : public Projectile {
public:
	Flame(StudentWorld* world, double startX, double startY, Direction dir);
};

class Spray : public Projectile {
public:
	Spray(StudentWorld* world, double startX, double startY, Direction dir);
};

#endif
