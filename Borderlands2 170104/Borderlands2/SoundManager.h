#pragma once
#include "inc/fmod.hpp"

#pragma comment (lib, "lib/fmodex_vc.lib")
using namespace FMOD;
using namespace std;

#define SOUNDBUFFER			50
#define EXTRACHANNELBUFFER	5
#define TOTALSOUNDBUFFER	(SOUNDBUFFER + EXTRACHANNELBUFFER)
#define g_pSoundManager		SoundManager::GetInstance()

class SoundManager
{
	SINGLETON(SoundManager);

private:
	typedef map<string, Sound**>				arrSounds;
	typedef map<string, Sound**>::iterator		arrSoundsIter;
	typedef map<string, Channel**>				arrChannels;
	typedef map<string, Channel**>::iterator	arrChannelsIter;

private:
	System*		system;
	Sound**		sound;
	Channel**	channel;

	arrSounds	mTotalSounds;

public:
	HRESULT Init();
	void Release();
	void Update();

	void AddSound(string keyName, string soundName, bool bgm, bool loop);
	void Play(string keyName, float volume = 1.0f);
	void Stop(string keyName);
	void Pause(string keyName);
	void Resume(string keyName);

	bool IsPlaySound(string keyName);
	bool IsPauseSound(string keyName);
};

