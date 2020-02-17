#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

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


Dirt::Dirt(StudentWorld* world, double startX, double startY) : Actor(world, IID_DIRT, startX, startY, 0, 0, 1) {

}
void Dirt::doSomething() {

}

bool Dirt::blocksPassage() {
	return true;
}


Socrates::Socrates(StudentWorld* world) : Actor(world, IID_PLAYER, 0, 128, 100, 0) {
	m_sprays = 20;
	m_flames = 5;
	m_pos = 180;
}

void Socrates::doSomething() {
	if (!isAlive())
		return;

	int ch;
	if (getWorld()->getKey(ch)) {
		switch (ch) {
		case KEY_PRESS_LEFT:
			//counterclockwise
			m_pos += 10;
		case KEY_PRESS_RIGHT:
			m_pos -= 5;
			m_pos %= 360;
			double pi = 3.14159265358979323846;
			moveTo(VIEW_RADIUS * cos(m_pos * pi / 180) + VIEW_WIDTH / 2,
				VIEW_RADIUS * sin(m_pos * pi / 180) + VIEW_HEIGHT / 2);
			setDirection(m_pos + 180);
			break;
		}
	}
}