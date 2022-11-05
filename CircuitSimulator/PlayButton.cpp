#include <imgui.h>
#include "PlayButton.h"

PlayButton::PlayButton()
	: m_isStarted(false)
{}

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
			printf("======== Start ======== \n");
			m_isStarted = true;
		}
	}
}

