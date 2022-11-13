#pragma once

#include <vector>
#include "Circuit.h"

class InteractionManager
{
public:
	static void Update(std::vector<Circuit*>* pCircuits);

private:
	static void HandleCreate();
	static void HandleDelete(std::vector<Circuit*>* pCircuits);
};

