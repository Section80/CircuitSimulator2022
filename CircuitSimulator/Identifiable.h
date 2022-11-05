#ifndef IDENTIFIABLE_H
#define IDENTIFIABLE_H

#include <unordered_map>

class Identifiable abstract {
public:
	Identifiable(unsigned int id);
	// id를 새로 발급받는다. 
	Identifiable();
	virtual ~Identifiable();
	
	inline unsigned int GetId() { return m_id; }

public:
	static unsigned int GetNewId() { return ++s_lastId; }

private:
	unsigned int m_id;

	static unsigned int s_lastId;
};

#endif
