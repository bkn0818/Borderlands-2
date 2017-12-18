#pragma once

#define g_pTimeManager TimeManager::GetInstance()

class TimeManager
{
	SINGLETON( TimeManager )
public:
	void Update();
	float GetDeltaTime();

private:
	DWORD lastUpdateTime;
	DWORD deltaTime;

};


