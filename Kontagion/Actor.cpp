#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* world, int imageID, double startX, double startY, int hitPts,
	Direction dir, int depth, bool isDamageable, bool alivity, double size) :
	GraphObject(imageID, startX, startY, dir, depth, size) {

	hp = hitPts;
	alive = alivity;
	damageable = isDamageable;
	m_world = world;
}

Actor::~Actor() {
	
}

void Actor::takeDmg(int dmg) {
	if (damageable) {
		hp -= dmg;
		if (hp <= 0) {
			alive = false;
		}
	}
}

StudentWorld* Actor::getWorld() {
	return m_world;
}

bool Actor::isAlive() {
	return alive;
}

bool Actor::blocksPassage() {
	return false;
}