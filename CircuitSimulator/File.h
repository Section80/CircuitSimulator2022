#ifndef FILE_H
#define FILE_H

#include <vector>
#include "Circuit.h"

void SaveCircuitsToFile(const char* fileName, std::vector<Circuit*>& circuits);
void LoadCircuitsFromFile(const char* fileName, std::vector<Circuit*>* pCircuits);

#endif
