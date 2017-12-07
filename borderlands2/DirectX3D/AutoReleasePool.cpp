#include "stdafx.h"
#include "AutoReleasePool.h"


AutoReleasePool::AutoReleasePool()
{
}


AutoReleasePool::~AutoReleasePool()
{
}

void AutoReleasePool::AddObject(Object * obj)
{
	objectList.push_back(obj);
}

void AutoReleasePool::Drain()
{
	for each(auto p in objectList)
	{
		SAFE_RELEASE(p);
		objectList.clear();
	}
}
