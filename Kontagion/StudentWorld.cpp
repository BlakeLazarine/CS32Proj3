#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <cmath>
#include <sstream>
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

void StudentWorld::spawnPointInRadius(int radius, double& x, double& y) {
	do {
		int r = sqrt(randInt(0, radius * radius));
		double theta = randInt(0, 360-1) * PI / 180;
		x = r * cos(theta);
		y = r * sin(theta);

		list<Actor*>::iterator it = m_actors.begin();
		while (it != m_actors.end()) {
			if ((*it)->blocksPlacement()) {
				int shiftX = x + VIEW_WIDTH / 2 - (*it)->getX();
				int shiftY = y + VIEW_HEIGHT / 2 - (*it)->getY();


				if (shiftX * shiftX + shiftY * shiftY < 4 * SPRITE_RADIUS * SPRITE_RADIUS) {
					x = radius;
					y = radius;
					break;
				}
			}

			it++;
		}
	} while (x * x + y * y > radius* radius);
}


int StudentWorld::init()
{
	
	soc = new Socrates(this);

	int numPits = getLevel();
	for (int i = 0; i < numPits; i++) {
		int PIT_RAD = 120;

		double x, y;

		spawnPointInRadius(PIT_RAD, x, y);

		Pit* p = new Pit(this, x + VIEW_WIDTH / 2, y + VIEW_HEIGHT / 2);
		m_actors.push_back(p);
	}


	int numFood = min(5 * getLevel(), 25);
	for (int i = 0; i < numFood; i++) {
		int FOOD_RAD = 120;

		double x, y;

		spawnPointInRadius(FOOD_RAD, x, y);

		Food* f = new Food(x + VIEW_WIDTH / 2, y + VIEW_HEIGHT / 2);
		m_actors.push_back(f);
	}

	



	int numDirt = max(180 - 20 * getLevel(), 20);
	for (int i = 0; i < numDirt; i++) {
		int DIRT_RAD = 120;
		
		double x, y;

		spawnPointInRadius(DIRT_RAD, x, y);
		
		Dirt* d = new Dirt(x + VIEW_WIDTH / 2, y + VIEW_HEIGHT / 2);
		m_actors.push_back(d);
	}
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	soc->doSomething();

	list<Actor*>::iterator it = m_actors.begin();

	int blockingWin = 0;
	while (it != m_actors.end()) {
		if ((*it)->blocksVictory())
			blockingWin++;
		(*it)->doSomething();
		it++;
		if (!soc->isAlive()) {
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
			
	}
	
	if (blockingWin == 0)
		return GWSTATUS_FINISHED_LEVEL;

	it = m_actors.begin();
	while (it != m_actors.end()) {
		if (!(*it)->isAlive()) {
			delete* it;
			it = m_actors.erase(it);
		}
		else {
			it++;
		}
	}

	while (!toBeAdded.empty()) {
		m_actors.push_back(toBeAdded.front());
		toBeAdded.pop();
	}

	int chanceFungus = max(510 - getLevel() * 10, 200);
	if (randInt(0, chanceFungus - 1) == 0) {
		addActor(new Fungus(this, randInt(0, 360 - 1)));
	}
	
	int chanceGoodie = max(510 - getLevel() * 10, 250);
	if (randInt(0, chanceGoodie - 1) == 0) {
		int typeGoodie = randInt(0, 9);
		switch (typeGoodie) {
		case 0:
			addActor(new ExtraLifeGoodie(this, randInt(0, 360 - 1)));
			break;
		case 1:
		case 2:
		case 3:
			addActor(new FlamethrowerGoodie(this, randInt(0, 360 - 1)));
			break;
		default:
			addActor(new RestoreHealthGoodie(this, randInt(0, 360 - 1)));
			break;
		}

	}

	
	ostringstream oss;
	oss << "Score: " << getScore() << "  "
		<< "Level: " << getLevel() << "  "
		<< "Lives: " << getLives() << "  "
		<< "Health: " << soc->getHP() << "  "
		<< "Sprays: " << soc->getSprays() << "  "
		<< "Flames: " << soc->getFlames();
	setGameStatText(oss.str());
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	list<Actor*>::iterator it = m_actors.begin();
	while (it != m_actors.end()) {
		delete *it;
		it = m_actors.erase(it);
	}

	delete soc;
}


Socrates* StudentWorld::playerWithin(double x, double y, double range) {
	if ((pow((soc->getX() - x), 2)) + pow((soc->getY() - y), 2) <= pow(range, 2))
		return soc;
	return nullptr;
}


bool damageableCheck(Actor* actor) {
	return actor->Damageable();
}

Actor* StudentWorld::damageableOverlap(Actor* act) {
	return overlap(act->getX(), act->getY(), &damageableCheck, SPRITE_WIDTH);
}
/*
Actor* StudentWorld::damageableAround(this, double range) {
	list<Actor*>::iterator it = m_actors.begin();
	
	while (it != m_actors.end()) {
		
		//if ((*it)->Damageable() && sqrt((pow(((*it)->getX() - x), 2)) + pow((*it)->getY() - y, 2)) <= 2 * range)
			//std::cout << sqrt((pow(((*it)->getX() - x), 2)) + pow((*it)->getY() - y, 2)) << std::endl;

		if ((*it)->Damageable() && sqrt((pow(((*it)->getX() - x), 2)) + pow((*it)->getY() - y, 2)) <= range)
			return *it;
		it++;

	}
}*/

void StudentWorld::addActor(Actor* actor) {
	toBeAdded.push(actor);
}


bool bacteriaConsumableCheck(Actor* actor) {
	return actor->eatenByBacteria();
}
Actor* StudentWorld::bacteriaConsumableOverlap(Actor* act) {
	return overlap(act->getX(), act->getY(), &bacteriaConsumableCheck, SPRITE_WIDTH);
}

bool blockPassageCheck(Actor* actor) {
	return actor->blocksPassage();
}

bool StudentWorld::canMoveTo(int x, int y) {
	if (pow(x - VIEW_WIDTH / 2, 2) + pow(y - VIEW_HEIGHT / 2, 2) >= pow(VIEW_RADIUS, 2))
		return false;
	return !overlap(x, y, &blockPassageCheck, SPRITE_WIDTH / 2);
}


Actor* StudentWorld::overlap(int x, int y, bool (*propertyCheck)(Actor*), double range) {
	list<Actor*>::iterator it = m_actors.begin();
	while (it != m_actors.end()) {

		if ((*it)->isAlive() && propertyCheck(*it) && (pow(((*it)->getX() - x), 2)) + pow(((*it)->getY() - y), 2) <= pow(range, 2)) {
			
			return *it;
		}
		it++;
	}
}


Actor* StudentWorld::nearestBacteriaConsumable(int x, int y, double& shortestDist) {
	list<Actor*>::iterator it = m_actors.begin();
	Actor* nearest = nullptr;
	
	while (it != m_actors.end()) {

		int thisDist = sqrt((pow(((*it)->getX() - x), 2)) + pow(((*it)->getY() - y), 2));
		if ((*it)->isAlive() && (*it)->eatenByBacteria() && thisDist < shortestDist) {
			shortestDist = thisDist;
			nearest = *it;
		}
		it++;
	}
	return nearest;
}

int StudentWorld::dirTowards(double sourceX, double sourceY, double endX, double endY) {
	int newDir;
	if (sourceX == endX) {
		newDir = -90;
		if (endY > sourceY)
			newDir = 90;
	}
	else {
		newDir = atan((endY - sourceY) / (endX - sourceX)) * 180 / PI;
		if (endX < sourceX)
			newDir += 180;
	}
	return newDir;
}