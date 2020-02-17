#ifndef SOCRATES_H
#define SOCRATES_H

#include "Actor.h"

class Socrates : public Actor {
public:
	Socrates(StudentWorld* world);

	void doSomething();
private:
	int m_sprays;
	int m_flames;
	int m_pos;
};

#endif // !SOCRATES_H

