#include "stdafx.h"
#include <stdio.h>
#include <sstream>
#include "Instruction.h"

int main()
{

	//        0b0000001111122222333334444455555
	int val = 0b00000000001000100001100000000000;

	Instruction i;
	i.SetRS(1).SetRT(2).SetRD(3);

	// printf("val:%d \n", val);
	// printf("i: %d \n", i.Get());

	// std::string str("11");
	// printf("%ld", str.size());


	std::string str;
	// GetBinaryString(32, &str);
	printf("%s", str.c_str());

	return 0;
}
