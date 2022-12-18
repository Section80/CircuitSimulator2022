#ifndef PARSE_H
#define PARSE_H

#include <map>
#include "DataMemory.h"

bool LoadInstruction(const char* path, std::map<int, int>* pMap, std::map<int, std::string>* pStringMap);
bool LoadData(const char* path, DataMemory* pDataMemory);

#endif
