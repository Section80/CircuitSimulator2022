#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <string>
#include <assert.h>
#include <unordered_map>
#include "Parse.h"
#include "Instruction.h"

int strToRegisterIndex(std::string& str);
bool strToInt(std::string& str, int* pOut);


bool LoadInstructions(const char* path, std::map<int, int>* pMap)
{
	using namespace std;
	printf("Load Instructions \n");

	std::map<int, int>& map = *pMap;
	map.clear();
	
	// 코드 참고
	// https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
	// https://stackoverflow.com/questions/20572203/detect-when-at-the-end-of-a-stringstream

	// MIPS CONVERTER
	// https://www.eg.bucknell.edu/~csci320/mips_web/

	
	ifstream f(path, ifstream::in);
	assert(f.is_open());

	string line;

	int lastKey = 0x00400024 - 4;
	unordered_map<string, int> labelMap;	// key: label string, value: instruction address

	// 일단 처음부터 끝까지 읽으면서 라벨을 초기화한다. 
	while (getline(f, line))
	{
		stringstream ss(line);

		while (!ss.eof())
		{
			string word;
			ss >> word;

			if (word.size() == 0)
			{
				continue;
			}

			if (word[0] == '#') break;

			// check if it is Label
			if (word.back() == ':')
			{
				word.pop_back();
				labelMap[word] = lastKey + 4;
				break;
			}

			// to lowercase: https://stackoverflow.com/questions/313970/how-to-convert-an-instance-of-stdstring-to-lower-case
			// for (auto& c : word)
			// {
			// 	c = tolower(c);
			// }

			if (word.compare("and") == 0)
			{
				lastKey += 4;
			}
			else if (word.compare("or") == 0)
			{
				lastKey += 4;
			}
			else if (word.compare("add") == 0)
			{
				lastKey += 4;
			}
			else if (word.compare("addi") == 0)
			{
				lastKey += 4;
			}
			else if (word.compare("sub") == 0)
			{
				lastKey += 4;
			}
			else if (word.compare("slt") == 0)
			{
				lastKey += 4;
			}
			else if (word.compare("lw") == 0)
			{
				lastKey += 4;
			}
			else if (word.compare("sw") == 0)
			{
				lastKey += 4;
			}
			else if (word.compare("beq") == 0)
			{
				lastKey += 4;
			}
			else if (word.compare("j") == 0)
			{
				lastKey += 4;
			}
		}
	}

	// 라벨을 초기화했으니 명령어를 채운다. 
	f.clear();
	f.seekg(0);
	lastKey = 0x00400024 - 4;
	while (getline(f, line))
	{
		stringstream ss(line);

		while (!ss.eof())
		{
			string word;
			ss >> word;

			if (word.size() == 0)
			{
				continue;
			}

			if (word[0] == '#') break;

			// to lowercase: https://stackoverflow.com/questions/313970/how-to-convert-an-instance-of-stdstring-to-lower-case
			// for (auto& c : word)
			// {
			// 	c = tolower(c);
			// }

			if (word.compare("and") == 0)
			{
				string op1;
				string op2;
				string op3;
				ss >> op1;
				ss >> op2;
				ss >> op3;

				int rd = strToRegisterIndex(op1);
				int rs = strToRegisterIndex(op2);
				int rt = strToRegisterIndex(op3);

				Instruction i;
				i.SetOpcode(0).SetRS(rs).SetRT(rt).SetRD(rd).SetFunct(0x24);
				map[lastKey += 4] = i.Get();
			}
			else if (word.compare("or") == 0)
			{
				string op1;
				string op2;
				string op3;
				ss >> op1;
				ss >> op2;
				ss >> op3;

				int rd = strToRegisterIndex(op1);
				int rs = strToRegisterIndex(op2);
				int rt = strToRegisterIndex(op3);

				Instruction i;
				i.SetOpcode(0).SetRS(rs).SetRT(rt).SetRD(rd).SetFunct(0x25);
				map[lastKey += 4] = i.Get();
			}
			else if (word.compare("add") == 0)
			{
				string op1;
				string op2;
				string op3;
				ss >> op1;
				ss >> op2;
				ss >> op3;

				int rd = strToRegisterIndex(op1);
				int rs = strToRegisterIndex(op2);
				int rt = strToRegisterIndex(op3);

				Instruction i;
				i.SetOpcode(0).SetRS(rs).SetRT(rt).SetRD(rd).SetFunct(0x20);
				map[lastKey += 4] = i.Get();
			}
			else if (word.compare("addi") == 0)
			{
				string op1;
				string op2;
				string op3;
				ss >> op1 >> op2 >> op3;

				int rt = strToRegisterIndex(op1);
				int rs = strToRegisterIndex(op2);
				int imm = 0;
				strToInt(op3, &imm);

				Instruction i;
				i.SetOpcode(0x8).SetRT(rt).SetRS(rs).SetLow16(imm);
				map[lastKey += 4] = i.Get();
			}
			else if (word.compare("sub") == 0)
			{
				string op1;
				string op2;
				string op3;
				ss >> op1;
				ss >> op2;
				ss >> op3;

				int rd = strToRegisterIndex(op1);
				int rs = strToRegisterIndex(op2);
				int rt = strToRegisterIndex(op3);

				Instruction i;
				i.SetOpcode(0).SetRS(rs).SetRT(rt).SetRD(rd).SetFunct(0x22);
				map[lastKey += 4] = i.Get();
			}
			else if (word.compare("slt") == 0)
			{
				string op1;
				string op2;
				string op3;
				ss >> op1;
				ss >> op2;
				ss >> op3;

				int rd = strToRegisterIndex(op1);
				int rs = strToRegisterIndex(op2);
				int rt = strToRegisterIndex(op3);

				Instruction i;
				i.SetOpcode(0).SetRS(rs).SetRT(rt).SetRD(rd).SetFunct(0x2a);
				map[lastKey += 4] = i.Get();
			}
			else if (word.compare("lw") == 0)
			{
				// lw &4, 8($5)
				string op1;
				ss >> op1;

				int rt = strToRegisterIndex(op1);

				// "(" 앞까지는 imm에 해당한다. 
				string immStr;
				getline(ss, immStr, '(');

				int imm = 0;
				strToInt(immStr, &imm);

				// 여는 괄호 이후 부분은 "$rs)"에 해당한다. 
				string rsStr;
				getline(ss, rsStr, ')');
				int rs = strToRegisterIndex(rsStr);
				
				Instruction i;
				i.SetOpcode(0x23).SetRT(rt).SetRS(rs).SetLow16(imm);
				map[lastKey += 4] = i.Get();
			}
			else if (word.compare("sw") == 0)
			{
			// sw &4, 8($5)
			string op1;
			ss >> op1;

			int rt = strToRegisterIndex(op1);

			// "(" 앞까지는 imm에 해당한다. 
			string immStr;
			getline(ss, immStr, '(');

			int imm = 0;
			strToInt(immStr, &imm);

			// 여는 괄호 이후 부분은 "$rs)"에 해당한다. 
			string rsStr;
			getline(ss, rsStr, ')');	// "(" 앞까지만 읽는다. 
			int rs = strToRegisterIndex(rsStr);

			Instruction i;
			i.SetOpcode(0x2b).SetRT(rt).SetRS(rs).SetLow16(imm);
			map[lastKey += 4] = i.Get();
			}
			else if (word.compare("beq") == 0)
			{
				string op1;
				string op2;
				string op3;
				ss >> op1;
				ss >> op2;
				ss >> op3;

				int rs = strToRegisterIndex(op1);
				int rt = strToRegisterIndex(op2);

				int imm = 0;
				if (labelMap.count(op3) != 0) 
				{
					imm = labelMap[op3];
				}
				else
				{
					printf("[info]LoadInsturctions() label not found: %s", op3.c_str());
				}

				Instruction i;
				i.SetOpcode(4).SetRS(rs).SetRT(rt).SetLow16(imm);
				map[lastKey += 4] = i.Get();
			}
			else if (word.compare("j") == 0)
			{
				string op;
				ss >> op;
				int addr = 0;
				if (labelMap.count(op) != 0) {
					int pc = lastKey + 4;
					addr = labelMap[op] / 4;
					addr |= 0b00001111111111111111111111111111;
					addr = addr >> 2;
				}
				else
				{
					printf("[info]LoadInsturctions() label not found: %s", op.c_str());
				}

				Instruction i;
				i.SetOpcode(2).SetAddress(addr);
				map[lastKey += 4] = i.Get();
			}
		}
	}

	return true;
}

void LoadDatas(const char* path, std::map<int, int>* pMap)
{
	printf("Load Datas \n");
}

int strToRegisterIndex(std::string& str)
{
	if (str[0] != '$')
	{
		return -1;
	}

	// remove $
	str = str.substr(1);

	// remove ,
	str.erase(remove(str.begin(), str.end(), ','), str.end());

	// check name first
	if (!str.compare("zero"))
	{
		return 0;
	}
	else if (!str.compare("at"))
	{
		return 1;
	}
	else if (!str.compare("v0"))
	{
		return 2;
	}
	else if (!str.compare("v1"))
	{
		return 3;
	}
	else if (!str.compare("a0"))
	{
		return 4;
	}
	else if (!str.compare("a1"))
	{
		return 5;
	}
	else if (!str.compare("a2"))
	{
		return 6;
	}
	else if (!str.compare("a3"))
	{
		return 7;
	}

	else if (!str.compare("t0"))
	{
		return 8;
	}
	else if (!str.compare("t1"))
	{
		return 9;
	}
	else if (!str.compare("t2"))
	{
		return 10;
	}
	else if (!str.compare("t3"))
	{
		return 11;
	}
	else if (!str.compare("t4"))
	{
		return 12;
	}
	else if (!str.compare("t5"))
	{
		return 13;
	}
	else if (!str.compare("t6"))
	{
		return 14;
	}
	else if (!str.compare("t7"))
	{
		return 15;
	}

	else if (!str.compare("s0"))
	{
		return 16;
	}
	else if (!str.compare("s1"))
	{
		return 17;
	}
	else if (!str.compare("s2"))
	{
		return 18;
	}
	else if (!str.compare("s3"))
	{
		return 19;
	}
	else if (!str.compare("s4"))
	{
		return 20;
	}
	else if (!str.compare("s5"))
	{
		return 21;
	}
	else if (!str.compare("s6"))
	{
		return 22;
	}
	else if (!str.compare("s7"))
	{
		return 23;
	}

	else if (!str.compare("t8"))
	{
		return 24;
	}
	else if (!str.compare("t9"))
	{
		return 25;
	}

	else if (!str.compare("k0"))
	{
		return 26;
	}
	else if (!str.compare("k1"))
	{
		return 27;
	}

	else if (!str.compare("gp"))
	{
	return 28;
	}
	else if (!str.compare("sp"))
	{
	return 29;
	}
	else if (!str.compare("fp"))
	{
	return 30;
	}
	else if (!str.compare("ra"))
	{
	return 31;
	}

	// number
	char* p = nullptr;
	long val = strtol(str.c_str(), &p, 10);
	if (*p)
	{
		return -1;
	}
	else
	{
		return (int)val;
	}

	return -1;
}

bool strToInt(std::string& str, int* pOut)
{
	assert(pOut != nullptr);

	char* p = nullptr;
	long val = strtol(str.c_str(), &p, 10);
	if (*p)
	{
		return false;
	}
	else
	{
		*pOut = (int)val;
		return true;
	}
	return false;
}
