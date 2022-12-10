#ifndef DATA_MEMORY_H
#define DATA_MEMORY_H

#include <string>
#include <map>

class DataMemory
{
public:
	DataMemory();

	int SetAscii(int address, std::string& ascii);
	int SetAsciiz(int address, std::string& ascii);

	int SetSpace(int address);

	int SetByte(int address, char val);
	int SetHalf(int address, short val);
	int SetWord(int address, int val);

	int SetFloat(int address, float val);
	int SetDouble(int address, double val);

	void SetLastChangd(int address);

	void Clear();

	int ReadWord(int address);

	void RenderInspector();

private:
	// key: address
	std::map<int, char> m_map;
	int m_lastChanged;
};

#endif