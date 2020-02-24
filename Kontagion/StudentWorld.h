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
	Socrates* playerWithin(double x, double y, double range);
	Actor* damageableOverlap(Actor* act);
	Actor* bacteriaConsumableOverlap(Actor* act);
	Actor* nearestBacteriaConsumable(int x, int y, double& dist);
	void addActor(Actor* actor);

	bool canMoveTo(int x, int y);

	int dirTowards(double sourceX, double sourceY, double endX, double endY);

private:
	std::list<Actor*> m_actors;
	
	Socrates* soc;
	std::queue<Actor*> toBeAdded;

	void spawnPointInRadius(int radius, double& x, double& y);
	Actor* overlap(int x, int y, bool (*propertyCheck)(Actor*), double range);
	//bool damageableCheck(Actor* actor);

};

#endif // STUDENTWORLD_H_
