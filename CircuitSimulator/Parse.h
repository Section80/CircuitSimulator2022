#ifndef PARSE_H
#define PARSE_H

#include <map>


bool LoadInstruction(const char* path, std::map<int, int>* pMap);
void LoadData(const char* path, std::map<int, int>* pMap);

#endif
