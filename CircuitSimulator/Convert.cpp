#include "stdafx.h"
#include <assert.h>
#include "Convert.h"

#pragma region BitMask
#define MASK_ALL_ZERO	0b00000000000000000000000000000000
#define MASK_0			0b00000000000000000000000000000001
#define MASK_1			0b00000000000000000000000000000010
#define MASK_2			0b00000000000000000000000000000100
#define MASK_3			0b00000000000000000000000000001000
#define MASK_4			0b00000000000000000000000000010000
#define MASK_5			0b00000000000000000000000000100000
#define MASK_6			0b00000000000000000000000001000000
#define MASK_7			0b00000000000000000000000010000000
#define MASK_8			0b00000000000000000000000100000000
#define MASK_9			0b00000000000000000000001000000000
#define MASK_10			0b00000000000000000000010000000000
#define MASK_11			0b00000000000000000000100000000000
#define MASK_12			0b00000000000000000001000000000000
#define MASK_13			0b00000000000000000010000000000000
#define MASK_14			0b00000000000000000100000000000000
#define MASK_15			0b00000000000000001000000000000000
#define MASK_16			0b00000000000000010000000000000000
#define MASK_17			0b00000000000000100000000000000000
#define MASK_18			0b00000000000001000000000000000000
#define MASK_19			0b00000000000010000000000000000000
#define MASK_20			0b00000000000100000000000000000000
#define MASK_21			0b00000000001000000000000000000000
#define MASK_22			0b00000000010000000000000000000000
#define MASK_23			0b00000000100000000000000000000000
#define MASK_24			0b00000001000000000000000000000000
#define MASK_25			0b00000010000000000000000000000000
#define MASK_26			0b00000100000000000000000000000000
#define MASK_27			0b00001000000000000000000000000000
#define MASK_28			0b00010000000000000000000000000000
#define MASK_29			0b00100000000000000000000000000000
#define MASK_30			0b01000000000000000000000000000000
#define MASK_31			0b10000000000000000000000000000000
#define MASK_ALL_ONE	0b11111111111111111111111111111111
#pragma endregion

#define READ_BIT_AT(bits, idx) ( (bits) & MASK_##idx )

uint32_t ReadToUint32(InputPin& inputPin, int len)
{
	uint32_t ret = 0;
	uint32_t mask = MASK_0;

	for (int i = 0; i < len; i++)
	{
		bool bVal = inputPin.ReadAt(i);
		if (bVal)
		{
			ret |= mask;
		}

		mask = mask << 1;
	}

	return ret;
}

bool ReadBitAt(uint32_t bits, int index)
{
	switch (index)
	{
	case 0:
		return READ_BIT_AT(bits, 0);
	case 1:
		return READ_BIT_AT(bits, 1);
	case 2:
		return READ_BIT_AT(bits, 2);
	case 3:
		return READ_BIT_AT(bits, 3);
	case 4:
		return READ_BIT_AT(bits, 4);
	case 5:
		return READ_BIT_AT(bits, 5);
	case 6:
		return READ_BIT_AT(bits, 6);
	case 7:
		return READ_BIT_AT(bits, 7);
	case 8:
		return READ_BIT_AT(bits, 8);
	case 9:
		return READ_BIT_AT(bits, 9);
	case 10:
		return READ_BIT_AT(bits, 10);
	case 11:
		return READ_BIT_AT(bits, 11);
	case 12:
		return READ_BIT_AT(bits, 12);
	case 13:
		return READ_BIT_AT(bits, 13);
	case 14:
		return READ_BIT_AT(bits, 14);
	case 15:
		return READ_BIT_AT(bits, 15);
	case 16:
		return READ_BIT_AT(bits, 16);
	case 17:
		return READ_BIT_AT(bits, 17);
	case 18:
		return READ_BIT_AT(bits, 18);
	case 19:
		return READ_BIT_AT(bits, 19);
	case 20:
		return READ_BIT_AT(bits, 20);
	case 21:
		return READ_BIT_AT(bits, 21);
	case 22:
		return READ_BIT_AT(bits, 22);
	case 23:
		return READ_BIT_AT(bits, 23);
	case 24:
		return READ_BIT_AT(bits, 24);
	case 25:
		return READ_BIT_AT(bits, 25);
	case 26:
		return READ_BIT_AT(bits, 26);
	case 27:
		return READ_BIT_AT(bits, 27);
	case 28:
		return READ_BIT_AT(bits, 28);
	case 29:
		return READ_BIT_AT(bits, 29);
	case 30:
		return READ_BIT_AT(bits, 30);
	case 31:
		return READ_BIT_AT(bits, 31);
	default:
		assert(0);
	}

	return false;
}

void Uint32ToBoolArray(uint32_t bits, bool* pOut, int len)
{
	for (int i = 0; i < len; i++)
	{
		pOut[i] = ReadBitAt(bits, i);
	}
}

void Copy(InputPin& inputPin, bool* pOut, int len)
{
	uint32_t val =ReadToUint32(inputPin, len);
	Uint32ToBoolArray(val, pOut, len);
}
