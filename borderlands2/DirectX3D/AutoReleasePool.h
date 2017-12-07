#pragma once

#define g_pAutoReleasePool AutoReleasePool::GetInstance()

class Object;

class AutoReleasePool
{
	SINGLETON(AutoReleasePool)

private:
	std::vector<Object*> objectList;

public:

	void AddObject(Object* obj);
	void Drain();
};

