#include "Actor.h"
#include "Dirt.h"


Dirt::Dirt(StudentWorld* world, double startX, double startY): Actor(world, IID_DIRT, startX, startY, 0, 0, 1){

}
void Dirt::doSomething() {

}

bool Dirt::blocksPassage() {
	return true;
}