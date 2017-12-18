#pragma once

#define g_pObjectManager ObjectManager::GetInstance()

class ObjectManager
{
	SINGLETON( ObjectManager );
private:
	std::set<Object*> m_setObject;

public:
	void AddObject( Object* object );
	void RemoveObject( Object* object );
	void Destroy();
};

