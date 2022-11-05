#pragma once
class InteractionManager
{
public:
	static void Update();

private:
	static void HandleCreate();
	static void HandleDelete();
};

