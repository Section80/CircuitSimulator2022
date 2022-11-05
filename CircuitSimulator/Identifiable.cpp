#include "stdafx.h"
#include <stdio.h>
#include "Identifiable.h"

unsigned int Identifiable::s_lastId = 0;

Identifiable::Identifiable(unsigned int id)
{
	if (s_lastId == -1)
	{
		s_lastId += 1;
	}

	m_id = ++s_lastId;
}

Identifiable::Identifiable()
	: m_id(++s_lastId)
{
	// printf("[Info]id created: %u \n", GetId());
}

Identifiable::~Identifiable()
{
	// printf("[Info]id deleted: %u \n", GetId());
}