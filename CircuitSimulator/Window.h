#ifndef WINDOW_H
#define WINDOW_H

namespace glfw_imgui
{
	struct Config
	{
		int Width;
		int Height;
		bool IsFixed;
		const char* Title;

		bool (*OnStart)();
		void (*OnUpdate)(double dt);
		bool (*OnEnd)();
	};

	bool LauchWindow(Config& config);
}

#endif WINDOW_H
