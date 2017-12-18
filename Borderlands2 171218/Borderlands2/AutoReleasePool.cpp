#include "StdAfx.h"
#include "AutoReleasePool.h"


AutoReleasePool::AutoReleasePool( void )
{
}


AutoReleasePool::~AutoReleasePool( void )
{
}

void AutoReleasePool::AddObject( Object* object )
{
	objectList.push_back( object );
}

void AutoReleasePool::Drain()
{
	for each( auto p in objectList )
		SAFE_RELEASE( p );
	objectList.clear();
}
