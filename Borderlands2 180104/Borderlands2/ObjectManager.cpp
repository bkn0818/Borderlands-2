#include "StdAfx.h"
#include "ObjectManager.h"


ObjectManager::ObjectManager( void )
{
}

ObjectManager::~ObjectManager( void )
{
}

void ObjectManager::AddObject( Object* object )
{
	m_setObject.insert( object );
}

void ObjectManager::RemoveObject( Object* object )
{
	m_setObject.erase( object );
}

void ObjectManager::Destroy()
{
	assert( m_setObject.empty() && "생성된 객체 중 삭제되지 않은 객체가 있습니다." );
}
