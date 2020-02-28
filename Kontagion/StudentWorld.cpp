#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>
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
	
	oss.fill('0');
	if(getScore() >= 0)
		oss << "Score: " << setw(6) << getScore() << "  ";
	else
		oss << "Score: -" << setw(5) << abs(getScore()) << "  ";

	oss << "Level: " << getLevel() << "  "
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
	while (!toBeAdded.empty()) {
		delete toBeAdded.front();
		toBeAdded.pop();
	}

	delete soc;
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

Socrates* StudentWorld::playerWithin(double x, double y, double range) {
	if ((pow((soc->getX() - x), 2)) + pow((soc->getY() - y), 2) <= pow(range, 2))
		return soc;
	return nullptr;
}


Actor* StudentWorld::damageableOverlap(double x, double y) {
	list<Actor*>::iterator it = m_actors.begin();
	while (it != m_actors.end()) {

		if ((*it)->isAlive() && (*it)->Damageable() && within(x, y, (*it)->getX(), (*it)->getY(), SPRITE_WIDTH)) {

			return *it;
		}
		it++;
	}
	return nullptr;
}

void StudentWorld::addActor(Actor* actor) {
	//toBeAdded.push(actor);
	toBeAdded.push(actor);
}

bool StudentWorld::damageAt(int dmg, double x, double y) {
	Actor* target = damageableOverlap(x, y);
	if (target) {
		target->takeDmg(dmg);
		return true;
	}
	return false;
}

bool StudentWorld::eatAt(double x, double y, double range) {
	Actor* target = bacteriaConsumableAround(x, y, range);
	if (target) {
		target->die();
		return true;
	}
	return false;
}

Actor* StudentWorld::bacteriaConsumableAround(double x, double y, double range) {
	list<Actor*>::iterator it = m_actors.begin();
	while (it != m_actors.end()) {

		if ((*it)->isAlive() && (*it)->eatenByBacteria() && within(x, y, (*it)->getX(), (*it)->getY(), range)) {

			return *it;
		}
		it++;
	}
	return nullptr;
}

bool StudentWorld::canMoveTo(int x, int y) {
	if (pow(x - VIEW_WIDTH / 2, 2) + pow(y - VIEW_HEIGHT / 2, 2) >= pow(VIEW_RADIUS, 2))
		return false;
	list<Actor*>::iterator it = m_actors.begin();
	
	while (it != m_actors.end()) {
		if ((*it)->isAlive() && (*it)->blocksPassage() && within(x, y, (*it)->getX(), (*it)->getY(), SPRITE_WIDTH / 2)) {
			return false;
		}
		it++;
	}
	
	return true;
}


bool StudentWorld::within(double x1, double y1, double x2, double y2, double range) {
	return (pow((x1 - x2), 2)) + pow((y1 - y2), 2) <= pow(range, 2);
}

bool StudentWorld::nearestBacteriaConsumable(double x, double y, double range, double& endx, double& endy) {
	list<Actor*>::iterator it = m_actors.begin();
	double shortestDist = -1;

	Actor* nearest = nullptr;
	while (it != m_actors.end()) {

		int thisDist = sqrt((pow(((*it)->getX() - x), 2)) + pow(((*it)->getY() - y), 2));
		if ((*it)->isAlive() && thisDist < range && (*it)->eatenByBacteria() && (thisDist < shortestDist || shortestDist == -1)) {
			shortestDist = thisDist;
			nearest = (*it);
		}
		it++;
	}
	if (nearest) {
		endx = nearest->getX();
		endy = nearest->getY();
	}
	return shortestDist != -1;
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