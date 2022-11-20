#ifndef PARSE_H
#define PARSE_H

#include <map>


bool LoadInstructions(const char* path, std::map<int, int>* pMap);
void LoadDatas(const char* path, std::map<int, int>* pMap);

#endif
