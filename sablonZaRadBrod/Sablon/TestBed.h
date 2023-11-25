#pragma once
#include "Ship.h"
double randomNumber();
void checkGear(Ship* ship);
void overtake(Ship* ship);
void someoneExitsShip(Ship* ship);
void turn(Ship* ship);
void runSimulation(Ship* ship);
Ship getShip();
void speedUp(Ship* ship);
void slowDown(Ship* ship);
void startSimulation(Ship* ship);
void endSimulation(Ship* ship);

