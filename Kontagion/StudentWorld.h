#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>

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
	bool playerWithin(double x, double y, double range);
	Actor* damageableAround(double x, double y, double range);

private:
	std::list<Actor*> m_actors;

	Socrates* soc;

};

#endif // STUDENTWORLD_H_
