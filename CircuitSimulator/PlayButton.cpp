#include "stdafx.h"
#include <imgui.h>
#include <assert.h>
#include "PlayButton.h"


PlayButton* PlayButton::Instance = nullptr;

PlayButton::PlayButton()
	: m_isStarted(false)
{
	assert(Instance == nullptr);
	Instance = this;
}

void PlayButton::Render()
{
	if (m_isStarted)
	{
		if (ImGui::Button("Pause"))
		{
			printf("======== Pause ======== \n");
			m_isStarted = false;
		}
	}
	else
	{
		if (ImGui::Button("Start"))
		{
			printf("======== Play ======== \n");
			m_isStarted = true;
		}
	}
}

