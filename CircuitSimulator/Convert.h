#ifndef CONVERSION_H
#define CONVERSION_H

#include <stdint.h>
#include "Pin.h"

uint32_t ReadToUint32(InputPin& inputPin, int len);
uint32_t ReadToUint32(InputPin& inputPin, int offset, int len);

bool ReadBitAt(uint32_t bits, int index);

void Uint32ToBoolArray(uint32_t bits, bool* pOut, int len = 32);

void Copy(InputPin& inputPin, bool* pOut, int len);

int EvaluateBoolBuffer(bool* pBuffer, int len);

#endif
