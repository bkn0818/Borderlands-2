#include "stdafx.h"
#include "TimeManager.h"


TimeManager::TimeManager()
{
	deltaTime = 0;
	lastUpdateTime = GetTickCount();
}


TimeManager::~TimeManager()
{
}

void TimeManager::Update()
{
	DWORD curUpdateTime = GetTickCount();
	deltaTime = curUpdateTime - lastUpdateTime;
	lastUpdateTime = curUpdateTime;
}

float TimeManager::GetDeltaTime()
{
	return deltaTime / 2000.0f;
}
