#include "Actor.h"
#include "Socrates.h"
#include "StudentWorld.h"
#include <cmath>

Socrates::Socrates(StudentWorld* world): Actor(world, IID_PLAYER, 0, 128, 100, 0){
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