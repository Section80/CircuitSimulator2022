#ifndef SPAWN_CIRCUIT_H
#define SPAWN_CIRCUIT_H

#include <vector>
#include "CircuitType.h"
class Circuit;

// void SpawnTestSRLatch(std::vector<Circuit*> pCircuits);

void SpawnTestRegisterFile(float x, float y, std::vector<Circuit*>* pCircuits);

void SpawnSimple1(float x, float y, std::vector<Circuit*>* pCircuits);

Circuit* SpawnCircuit(ECircuitType type, float x, float y);

#endif