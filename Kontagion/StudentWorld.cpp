#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
	
	soc = new Socrates(this);

	list<Actor*> takenSpaces;


	int numDirt = max(180 - 20 * getLevel(), 20);
	for (int i = 0; i < numDirt; i++) {
		int DIRT_RAD = 120;
		
		double x, y;

		do {
			int r = sqrt(randInt(0, DIRT_RAD * DIRT_RAD));
			double theta = randInt(0, 360) * PI / 180;
			x = r * cos(theta);
			y = r * sin(theta);

			list<Actor*>::iterator it = takenSpaces.begin();
			while (it != takenSpaces.end()) {
				int shiftX = x + VIEW_WIDTH / 2 - (*it)->getX();
				int shiftY = y + VIEW_HEIGHT / 2 - (*it)->getY();
				

				if (shiftX * shiftX + shiftY * shiftY < 4 * SPRITE_RADIUS * SPRITE_RADIUS) {
					x = DIRT_RAD;
					y = DIRT_RAD;
					break;
				}
				it++;
			}
		}
		while (x * x + y * y > DIRT_RAD* DIRT_RAD);
		
		Dirt* d = new Dirt(x + VIEW_WIDTH / 2, y + VIEW_HEIGHT / 2);
		m_actors.push_back(d);
		//takenSpaces.push_back(d); // this was for testing if the system worked
	}
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	soc->doSomething();

	list<Actor*>::iterator it = m_actors.begin();
	while (it != m_actors.end()) {
		if (!(*it)->isAlive()) {
			delete* it;
			it = m_actors.erase(it);
		}
		else {
			(*it)->doSomething();
			it++;
		}
		
	}

	if (!soc->isAlive()) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	list<Actor*>::iterator it = m_actors.begin();
	while (it != m_actors.end()) {
		delete *it;
		it++;
	}

	delete soc;
}


bool StudentWorld::playerWithin(double x, double y, double range) {
	return (pow((soc->getX() - x), 2)) + pow((soc->getY() - y), 2) <= pow(range, 2);
}

Actor* StudentWorld::damageableAround(double x, double y, double range) {
	list<Actor*>::iterator it = m_actors.begin();
	while (it != m_actors.end()) {
		if ((pow(((*it)->getX() - x), 2)) + pow(((*it)->getY() - y), 2) <= pow(range, 2))
			return *it;
		it++;
	}
}