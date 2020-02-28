#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>
#include <queue>

class Actor;
class Socrates;

const double PI = 3.14159265358979323846;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();


	//Returns a pointer to the player, if they are within a given range.
	Socrates* playerWithin(double x, double y, double range);

	//will deal damage to an object in the given range. If successful it returns true. Else returns false
	bool damageAt(int dmg, double x, double y);

	//will kill a bacteria-consumable in within a given range. If successful, returns true.
	bool eatAt(double x, double y, double range = SPRITE_WIDTH);
	
	// if there is a bacteria consumable within range, will return true
	// and set endx and endy to the consumable's location
	// else returns false
	bool nearestBacteriaConsumable(double x, double y, double range, double& endx, double& endy);
	
	//used to add an actor to the list of tracked actors
	void addActor(Actor* actor);

	bool canMoveTo(int x, int y);

	int dirTowards(double sourceX, double sourceY, double endX, double endY);

	~StudentWorld();

private:
	std::list<Actor*> m_actors;
	
	Socrates* soc;
	std::queue<Actor*> toBeAdded;

	Actor* damageableOverlap(double x, double y);
	Actor* bacteriaConsumableAround(double x, double y, double range = SPRITE_WIDTH);

	void spawnPointInRadius(int radius, double& x, double& y);
	//bool damageableCheck(Actor* actor);

	bool within(double x1, double y1, double x2, double y2, double range);
};

#endif // STUDENTWORLD_H_
