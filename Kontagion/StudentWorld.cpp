#include "StudentWorld.h"
#include "GameConstants.h"
#include <algorithm>
#include <string>
#include "Dirt.h"
#include "Socrates.h"
#include <iostream>
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
		
		int x = DIRT_RAD;
		int y = DIRT_RAD;
		

		while (x * x + y * y > DIRT_RAD* DIRT_RAD) {
			x = randInt(-DIRT_RAD, DIRT_RAD);
			y = randInt(-DIRT_RAD, DIRT_RAD);			

			list<Actor*>::iterator it = takenSpaces.begin();
			while (it != takenSpaces.end()) {
				int shiftX = x + VIEW_WIDTH / 2 - (*it)->getX();
				int shiftY = y + VIEW_HEIGHT / 2 - (*it)->getY();
				

				if (shiftX * shiftX + shiftY * shiftY < 4 * SPRITE_RADIUS * SPRITE_RADIUS) {
					int x = DIRT_RAD;
					int y = DIRT_RAD;
					break;
				}
				it++;
			}
		}
		
		Dirt* d = new Dirt(this, x + VIEW_WIDTH / 2, y + VIEW_HEIGHT / 2);
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
