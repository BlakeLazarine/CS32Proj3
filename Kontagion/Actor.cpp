#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <iostream>

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

void Actor::die() {
	alive = false;
}

void Actor::takeDmg(int dmg) {
	if (damageable) {
		hp -= dmg;
		if (hp <= 0) {
			die();
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

bool Actor::blocksPlacement() {
	return false;
}



Dirt::Dirt(double startX, double startY) : Actor(nullptr, IID_DIRT, startX, startY, 0, 0, 1) {
	//nullptr is passed as world, since Dirt never needs to access the studentworld
}
void Dirt::doSomething() {

}

bool Dirt::blocksPassage() {
	return true;
}



Food::Food(double startX, double startY) : Actor(nullptr, IID_FOOD, startX, startY, 0, 90, 1, false) {

}

bool Food::blocksPlacement() {
	return true;
}

void Food::doSomething() {

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
	;
	if (getWorld()->getKey(ch)) {
		switch (ch) {
		case KEY_PRESS_LEFT:
			//counterclockwise
			m_pos += 10;
		case KEY_PRESS_RIGHT:
			m_pos -= 5;
			m_pos %= 360;
			
			moveTo(VIEW_RADIUS * cos(m_pos * PI / 180) + VIEW_WIDTH / 2,
				VIEW_RADIUS * sin(m_pos * PI / 180) + VIEW_HEIGHT / 2);
			setDirection(m_pos + 180);
			break;
		}
	}
}


Projectile::Projectile(int dmg, int lifespan, StudentWorld* world, int imageID, double startX, double startY,
	Direction dir, int depth, double size) :
	Actor(world, imageID, startX, startY, 0, dir, depth, false, true, size) {
	m_dmg = dmg;
	m_lifespan = lifespan;
}

void Projectile::doSomething() {
	if (!isAlive())
		return;
	Actor* target = getWorld()->damageableAround(getX(), getY(), SPRITE_RADIUS * 2);
	if (target) {
		target->takeDmg(m_dmg);
		die();
		return;
	}
	moveAngle(getDirection(), SPRITE_RADIUS * 2);
	m_lifespan -= SPRITE_RADIUS * 2;
	if (m_lifespan <= 0)
		die();
}

Flame::Flame(StudentWorld* world, double startX, double startY, Direction dir) : Projectile(5, 32, world, IID_FLAME, startX, startY, dir, 1){

}

Spray::Spray(StudentWorld* world, double startX, double startY, Direction dir) : Projectile(2, 112, world, IID_SPRAY, startX, startY, dir, 1) {

}