#ifndef CONVERSION_H
#define CONVERSION_H

#include <stdint.h>
#include "Pin.h"

uint32_t ReadToUint32(InputPin& inputPin, int len);

bool ReadBitAt(uint32_t bits, int index);

void Uint32ToBoolArray(uint32_t bits, bool* pOut, int len = 32);

#endif
