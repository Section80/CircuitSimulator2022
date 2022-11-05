#ifndef START_BUTTON_H
#define START_BUTTON_H

#include "Identifiable.h"

class PlayButton final : public Identifiable
{
public:
	PlayButton();

	void Render();

	inline bool IsStarted() { return m_isStarted; }

private:
	bool m_isStarted;
};

#endif
