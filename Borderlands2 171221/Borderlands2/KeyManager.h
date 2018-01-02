#pragma once
#include <bitset>

#define KEYMAX 256
#define g_pKeyManager KeyManager::GetInstance()

using namespace std;

class KeyManager
{
	SINGLETON(KeyManager);

private:
	bitset<KEYMAX> keyUp;
	bitset<KEYMAX> keyDown;

public:
	HRESULT Init();
	void	Release();

	bool	IsOnceKeyDown(int key);
	bool	IsOnceKeyUp(int key);
	bool	IsStayKeyDown(int key);
	bool	IsToggleKey(int key);

	bitset<KEYMAX> GetKeyUp() { return keyUp; };
	bitset<KEYMAX> GetKeyDown() { return keyDown; };

	void SetKeyDown(int key, bool state) { keyDown.set(key, state); };
	void SetKeyUp(int key, bool state) { keyUp.set(key, state); };
};

