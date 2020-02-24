#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <iostream>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* world, int imageID, double startX, double startY, int hitPts,
	Direction dir, int depth, bool isDamageable, bool alivity, double size) :
	GraphObject(imageID, startX, startY, dir, depth, size) {

	hp = hitPts;
	initialHP = hp;
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

int Actor::getHP() {
	return hp;
}

bool Actor::Damageable() {
	return damageable;
}
bool Actor::eatenByBacteria() {
	return false;
}

int Actor::getInitialHP() {
	return initialHP;
}

void Actor::setHP(int newHP) {
	hp = newHP;
	if (hp <= 0)
		die();
}

bool Actor::blocksVictory() {
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
bool Food::eatenByBacteria() {
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
		case KEY_PRESS_SPACE:
			//getWorld()->addActor(new RestoreHealthGoodie(getWorld(), 0));
			//getWorld()->addActor(new Ecoli(getWorld(), VIEW_WIDTH / 2, VIEW_HEIGHT / 2));
			if (getSprays() > 0) {
				double x,y;
				getPositionInThisDirection(getDirection(), SPRITE_WIDTH, x, y);
				Spray* sp = new Spray(getWorld(), x, y, getDirection());
			
				getWorld()->addActor(sp);
				m_sprays--;
				getWorld()->playSound(SOUND_PLAYER_SPRAY);

			}
			break;
		case KEY_PRESS_ENTER:
			if (getFlames() > 0) {
				for (int flameIdx = 0; flameIdx < 16; flameIdx++) {
					double x, y;
					getPositionInThisDirection(getDirection() + 22 * flameIdx, SPRITE_WIDTH, x, y);
					Flame* flame = new Flame(getWorld(), x, y, getDirection() + 22 * flameIdx);
					getWorld()->addActor(flame);
				}
				m_flames--;
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				
			}
			break;
		}
	}
	else {
		if (m_sprays < SOCRATES_MAX_SPRAYS)
			m_sprays++;
	}
}

int Socrates::getSprays() {
	return m_sprays;
}

int Socrates::getFlames() {
	return m_flames;
}

void Socrates::setFlames(int numFlames) {
	m_flames = numFlames;
}


Projectile::Projectile(int lifespan, StudentWorld* world, int imageID, double startX, double startY,
	Direction dir, int depth, double size) :
	Actor(world, imageID, startX, startY, 0, dir, depth, false, true, size) {

	m_lifespan = lifespan;
}

void Projectile::doSomething() {
	if (!isAlive()) {
		return;
	}
	
	
	Actor* target = getWorld()->damageableOverlap(this);
	if (target) {
		collision(target); 
		die();
		return;
	}
	moveForward(SPRITE_WIDTH);
	m_lifespan -= SPRITE_WIDTH;
	
	if (m_lifespan < 0)
		die();
}

Flame::Flame(StudentWorld* world, double startX, double startY, Direction dir) : Projectile(32, world, IID_FLAME, startX, startY, dir, 1){

}
void Flame::collision(Actor* target) {
	target->takeDmg(5);
}

Spray::Spray(StudentWorld* world, double startX, double startY, Direction dir) : Projectile(112, world, IID_SPRAY, startX, startY, dir, 1) {

}

void Spray::collision(Actor* target) {
	target->takeDmg(2);
}





Goodie::Goodie(int lifespan, StudentWorld* world, int imageID, int startPos,
	Direction dir, int depth, double size) : Actor(world, imageID, 
		VIEW_WIDTH / 2 + VIEW_RADIUS * cos(startPos * PI / 180),
		VIEW_HEIGHT / 2 + VIEW_RADIUS * sin(startPos * PI / 180),
		0, dir, depth, true, true, size) {
	m_lifespan = lifespan;
}

void Goodie::doSomething() {
	if (!isAlive())
		return;

	Socrates* soc = getWorld()->playerWithin(getX(), getY(), SPRITE_WIDTH);
	if (soc) {
		onPickup(soc);
		return;
	}
	m_lifespan--;
	if (m_lifespan <= 0)
		die();
}

RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* world, int pos) : 
	Goodie(std::max(randInt(0, 300 - 10 * world->getLevel() - 1), 50),
		world, IID_RESTORE_HEALTH_GOODIE, pos, 0, 1){
}

void RestoreHealthGoodie::onPickup(Socrates* soc) {
	getWorld()->increaseScore(250);
	die();
	getWorld()->playSound(SOUND_GOT_GOODIE);
	soc->setHP(soc->getInitialHP());
}


FlamethrowerGoodie::FlamethrowerGoodie(StudentWorld* world, int pos) :
	Goodie(std::max(randInt(0, 300 - 10 * world->getLevel() - 1), 50),
		world, IID_FLAME_THROWER_GOODIE, pos, 0, 1) {

}

void FlamethrowerGoodie::onPickup(Socrates* soc) {
	getWorld()->increaseScore(300);
	die();
	getWorld()->playSound(SOUND_GOT_GOODIE);
	soc->setFlames(soc->getFlames() + 5);
}


ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, int pos) :
	Goodie(std::max(randInt(0, 300 - 10 * world->getLevel() - 1), 50),
		world, IID_EXTRA_LIFE_GOODIE, pos, 0, 1) {

}

void ExtraLifeGoodie::onPickup(Socrates* soc) {
	getWorld()->increaseScore(500);
	die();
	getWorld()->playSound(SOUND_GOT_GOODIE);
	getWorld()->incLives();
}


Fungus::Fungus(StudentWorld* world, int pos) :
	Goodie(std::max(randInt(0, 300 - 10 * world->getLevel() - 1), 50),
		world, IID_FUNGUS, pos, 0, 1) {

}

void Fungus::onPickup(Socrates* soc) {
	getWorld()->increaseScore(-50);
	die();
	soc->takeDmg(20);
}




Bacterium::Bacterium(int hurtSound, int dieSound, int strength, StudentWorld* world, int imageID,
	double startX, double startY, int hitPts,
	Direction dir, int depth, double size) : 
	Actor(world, imageID, startX, startY, hitPts, dir, depth, true, true, size) {
	m_strength = strength;
	m_dieSound = dieSound;
	m_hurtSound = hurtSound;
	foodEaten = 0;
}

bool Bacterium::blocksVictory() {
	return true;
}

void Bacterium::doSomething() {
	if (!isAlive())
		return;
	bool movedEarly = earlyMove();
	Socrates* player = getWorld()->playerWithin(getX(), getY(), SPRITE_WIDTH);
	if (player)
		hitPlayer(player);
	else {
		if (foodEaten == 3) {

			int newx = getX();
			if (getX() < VIEW_WIDTH / 2)
				newx += SPRITE_WIDTH / 2;
			else if (getX() > VIEW_WIDTH / 2)
				newx -= SPRITE_WIDTH / 2;

			int newy = getY();
			if (getY() < VIEW_HEIGHT / 2)
				newy += SPRITE_WIDTH / 2;
			else if (getY() > VIEW_HEIGHT / 2)
				newy -= SPRITE_WIDTH / 2;

			split(newx, newy);		

			foodEaten = 0;
		}
		else {
			Actor* food = getWorld()->bacteriaConsumableOverlap(this);
			if (food) {
				foodEaten++;
				food->die();
			}
		}
	}
	if (!movedEarly)
		move();
}

bool Bacterium::earlyMove() {
	return false;
}

void Bacterium::hitPlayer(Socrates* soc) {
	soc->takeDmg(m_strength);
}

void Bacterium::takeDmg(int dmg) {
	
	Actor::takeDmg(dmg);
	if (getHP() <= 0) {
		getWorld()->playSound(m_dieSound);
		getWorld()->increaseScore(100);
		if (randInt(0, 1))
			getWorld()->addActor(new Food(getX(), getY()));
	}
	else {
		getWorld()->playSound(m_hurtSound);
	}
}


Salmonella::Salmonella(StudentWorld* world, double startX, double startY, int hitPts, int strength) : 
	Bacterium(SOUND_SALMONELLA_HURT, SOUND_SALMONELLA_DIE, strength, world, IID_SALMONELLA, startX, startY, hitPts, 90) {
	m_movementPlanDist = 0;

}

void Salmonella::split(int newx, int newy) {
	getWorld()->addActor(new Salmonella(getWorld(), newx, newy));
}

void Salmonella::move() {
	if (m_movementPlanDist > 0) {
		m_movementPlanDist--;
		double x, y;
		getPositionInThisDirection(getDirection(), 3, x, y);
		if (getWorld()->canMoveTo(x, y)) {
			moveTo(x,y);
		}
		else {
			setDirection(randInt(0, 359));
			m_movementPlanDist = 10;
		}
	}
	else {
		double dist = VIEW_HEIGHT;
		Actor* nearestFood = getWorld()->nearestBacteriaConsumable(getX(), getY(), dist);
		
		if (dist > 128 || !nearestFood) {
			setDirection(randInt(0, 359));
			m_movementPlanDist = 10;
		}
		else {
			
			Direction newDir = getWorld()->dirTowards(getX(), getY(), 
				nearestFood->getX(), nearestFood->getY());
			double x, y;
			getPositionInThisDirection(newDir, 3, x, y);
			if (getWorld()->canMoveTo(x, y)) {
				moveTo(x,y);
				setDirection(newDir);
			}
			else {
				setDirection(randInt(0, 359));
				m_movementPlanDist = 10;
				
			}
			

		}
	}

}

AggressiveSalmonella::AggressiveSalmonella(StudentWorld* world, double startX, double startY) : 
	Salmonella(world, startX, startY, 10, 2){

}

bool AggressiveSalmonella::earlyMove() {
	Socrates* soc = getWorld()->playerWithin(getX(), getY(), 72);
	if (soc) {
		Direction newDir = getWorld()->dirTowards(getX(), getY(), soc->getX(), soc->getY());
		double x, y;
		getPositionInThisDirection(newDir, 3, x, y);
		if (getWorld()->canMoveTo(x, y)) {
			moveTo(x,y);
			setDirection(newDir);
		}
		return true;
	}
	return false;

}


void AggressiveSalmonella::split(int newx, int newy) {
	getWorld()->addActor(new AggressiveSalmonella(getWorld(), newx, newy));
}


Ecoli::Ecoli(StudentWorld* world, double startX, double startY) :
	Bacterium(SOUND_ECOLI_HURT, SOUND_ECOLI_DIE, 4, world, IID_ECOLI, startX, startY, 5, 90) {

}

void Ecoli::split(int newx, int newy) {
	getWorld()->addActor(new Ecoli(getWorld(), newx, newy));
}

void Ecoli::move() {
	Socrates* soc = getWorld()->playerWithin(getX(), getY(), 256);
	if (soc) {
		Direction newDir = getWorld()->dirTowards(getX(), getY(), soc->getX(), soc->getY());
		
		for (int attempt = 0; attempt < 10; attempt++) {
			double x, y;
			getPositionInThisDirection(newDir + 10 * attempt, 2, x, y);
			if (getWorld()->canMoveTo(x, y)) {
				moveTo(x, y);
				setDirection(newDir + 10 * attempt);
				return;
			}
		}
		
	}
}

Pit::Pit(StudentWorld* world, double x, double y) :Actor(world, IID_PIT, x, y, 0, 0, 1, false, true){
	reg = 5;
	agg = 3;
	ecoli = 2;
	
}
bool Pit::blocksPlacement() {
	return true;
}

void Pit::doSomething() {
	if (reg + agg + ecoli == 0) {
		//inform StudentWorld that this guy is done
		die();
		return;
	}
	if (randInt(1, 50) == 1) {
		while (true) {
			int toSpawn = randInt(0, 2);
			switch (toSpawn) {
			case 0:
				if (reg > 0) {
					reg--;
					getWorld()->addActor(new Salmonella(getWorld(), getX(), getY()));
					break;
				} 
				continue;
			case 1:
				if (agg > 0) {
					agg--;
					getWorld()->addActor(new AggressiveSalmonella(getWorld(), getX(), getY()));
					break;
				}
				continue;
			case 2:
				if (ecoli > 0) {
					ecoli--;
					getWorld()->addActor(new Ecoli(getWorld(), getX(), getY()));
					break;
				}
				continue;
			}
			break;
		}
		
	}
}

bool Pit::blocksVictory() {
	return true;
}