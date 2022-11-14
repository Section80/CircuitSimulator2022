#ifndef START_BUTTON_H
#define START_BUTTON_H

#include "Identifiable.h"

class PlayButton final : public Identifiable
{
public:
	PlayButton();

	void Render();

	inline void Play() { m_isStarted = true; }
	inline void Pause() { m_isStarted = false; }

	inline bool IsPlaying() { return m_isStarted; }

public:
	static PlayButton* Instance;

private:
	bool m_isStarted;
};

#endif
