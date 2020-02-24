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
	virtual bool eatenByBacteria();
	virtual bool blocksVictory();

	bool Damageable();

	void die();
	bool isAlive();
	int getHP();
	void setHP(int newHP);
	int getInitialHP();
	StudentWorld* getWorld();
private:
	int hp;
	int initialHP;
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
	bool eatenByBacteria();
	
};

class Socrates : public Actor {
public:
	Socrates(StudentWorld* world);

	void doSomething();
	int getSprays();
	int getFlames();
	void setFlames(int numFlames);
private:
	int m_sprays;
	int m_flames;
	int m_pos;
	const int SOCRATES_MAX_SPRAYS = 20;
};



class Projectile : public Actor {
public:
	Projectile(int lifespan, StudentWorld* world, int imageID, double startX, double startY,
		Direction dir = 0, int depth = 0, double size = 1.0);
		//Actor(world, imageID, startX, startY, 0, dir, depth, false, true, size)
	void doSomething();
	virtual void collision(Actor* target) = 0;
private:
	int m_dmg, m_lifespan;
};

class Flame : public Projectile {
public:
	Flame(StudentWorld* world, double startX, double startY, Direction dir);
	void collision(Actor* target);
};

class Spray : public Projectile {
public:
	Spray(StudentWorld* world, double startX, double startY, Direction dir);
	void collision(Actor* target);
};

class Bacterium : public Actor {
public:
	Bacterium(int hurtSound, int dieSound, int strength, StudentWorld* world, int imageID, double startX, double startY, int hitPts,
		Direction dir = 0, int depth = 0, double size = 1.0);
	void doSomething();

	virtual void move() = 0;
	virtual void hitPlayer(Socrates* soc);
	virtual void split(int newx, int newy) = 0;
	virtual bool earlyMove();
	bool blocksVictory();
	void takeDmg(int dmg);

private:
	int foodEaten;
	int m_strength;

	int m_hurtSound, m_dieSound;
};


class Salmonella : public Bacterium {
public:
	Salmonella(StudentWorld* world, double startX, double startY, int hitPts = 4, int strength = 1);
	
	
	void move();
	virtual void split(int newx, int newy);

private:
	int m_movementPlanDist;
};

class AggressiveSalmonella : public Salmonella {
public:
	AggressiveSalmonella(StudentWorld* world, double startX, double startY);
	bool earlyMove();
	void split(int newx, int newy);
};

class Ecoli : public Bacterium {
public:
	Ecoli(StudentWorld* world, double startX, double startY);
	void move();
	void split(int newx, int newy);
};



class Goodie : public Actor {
public:
	Goodie(int lifespan, StudentWorld* world, int imageID, int startPos,
		Direction dir = 0, int depth = 0, double size = 1.0);
	void doSomething();
	virtual void onPickup(Socrates* soc) = 0;

private:
	int m_lifespan;
};


class RestoreHealthGoodie : public Goodie {
public:
	RestoreHealthGoodie(StudentWorld* world, int pos);
	void onPickup(Socrates* soc);
};


class FlamethrowerGoodie : public Goodie {
public:
	FlamethrowerGoodie(StudentWorld* world, int pos);
	void onPickup(Socrates* soc);
};


class ExtraLifeGoodie : public Goodie {
public:
	ExtraLifeGoodie(StudentWorld* world, int pos);
	void onPickup(Socrates* soc);
};


class Fungus : public Goodie {
public:
	Fungus(StudentWorld* world, int pos);
	void onPickup(Socrates* soc);
};

class Pit : public Actor {
public:
	Pit(StudentWorld* world, double x, double y);
	void doSomething();
	bool blocksPlacement();
	bool blocksVictory();
private:
	int reg, agg, ecoli;
};


#endif