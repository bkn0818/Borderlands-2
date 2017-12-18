#include "stdafx.h"
#include "KeyManager.h"


KeyManager::KeyManager()
{
}


KeyManager::~KeyManager()
{
}

HRESULT KeyManager::Init()
{
	for (int i = 0; i < KEYMAX; ++i)
	{
		GetKeyUp().set(i, false);
		GetKeyUp().set(i, false);
	}

	return S_OK;
}

void KeyManager::Release()
{
}

bool KeyManager::IsOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!GetKeyDown()[key])
		{
			SetKeyDown(key, true);
			return true;
		}
	}
	else SetKeyDown(key, false);

	return false;
}

bool KeyManager::IsOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) SetKeyUp(key, true);
	else
	{
		if (GetKeyUp()[key])
		{
			SetKeyUp(key, false);
			return true;
		}
	}
	return false;
}

bool KeyManager::IsStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

bool KeyManager::IsToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}
