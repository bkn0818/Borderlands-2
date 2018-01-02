#include "stdafx.h"
#include "SoundManager.h"


SoundManager::SoundManager()
{
}


SoundManager::~SoundManager()
{
}

HRESULT SoundManager::Init()
{
	System_Create(&system);

	system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	sound = new Sound*[TOTALSOUNDBUFFER];
	channel = new Channel*[TOTALSOUNDBUFFER];

	memset(sound, 0, sizeof(Sound*) * (TOTALSOUNDBUFFER));
	memset(channel, 0, sizeof(Channel*) * (TOTALSOUNDBUFFER));

	return S_OK;
}

void SoundManager::Release()
{
	if (channel || sound)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; ++i)
		{
			if (channel) channel[i]->stop();
			if (sound) sound[i]->release();
		}
	}

	SAFE_DELETE_ARRAY(channel);
	SAFE_DELETE_ARRAY(sound);

	if (system)
	{
		system->release();
		system->close();
	}
}

void SoundManager::Update()
{
	system->update();
}

void SoundManager::AddSound(string keyName, string soundName, bool bgm, bool loop)
{
 	if (loop)
	{
		if (bgm)
		{
			system->createStream((const char*)soundName.c_str(), FMOD_LOOP_NORMAL, 0, &sound[mTotalSounds.size()]);
		}
		else
		{
			system->createSound((const char*)soundName.c_str(), FMOD_LOOP_NORMAL, 0, &sound[mTotalSounds.size()]);
		}
	}
	else
	{
		if (bgm)
		{
			system->createStream((const char*)soundName.c_str(), FMOD_DEFAULT, 0, &sound[mTotalSounds.size()]);
		}
		else
		{
			system->createSound((const char*)soundName.c_str(), FMOD_DEFAULT, 0, &sound[mTotalSounds.size()]);
		}
	}

	mTotalSounds.insert(make_pair(keyName, &sound[mTotalSounds.size()]));
}

void SoundManager::Play(string keyName, float volume)
{
	arrSoundsIter iter = mTotalSounds.begin();

	int count = 0;

	for (iter; iter != mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			system->playSound(FMOD_CHANNEL_FREE, sound[count], false, &channel[count]);
			channel[count]->setVolume(volume);

			break;
		}
	}
}

void SoundManager::Stop(string keyName)
{
	arrSoundsIter iter = mTotalSounds.begin();

	int count = 0;

	for (iter; iter != mTotalSounds.end(); ++iter, count++)
	{
		//음악이 있다면
		if (keyName == iter->first)
		{
			channel[count]->stop();
			break;
		}
	}
}

void SoundManager::Pause(string keyName)
{
	arrSoundsIter iter = mTotalSounds.begin();

	int count = 0;

	for (iter; iter != mTotalSounds.end(); ++iter, count++)
	{
		//음악이 있다면
		if (keyName == iter->first)
		{
			channel[count]->setPaused(true);
			break;
		}
	}
}

void SoundManager::Resume(string keyName)
{
	arrSoundsIter iter = mTotalSounds.begin();

	int count = 0;

	for (iter; iter != mTotalSounds.end(); ++iter, count++)
	{
		//음악이 있다면
		if (keyName == iter->first)
		{
			channel[count]->setPaused(false);
			break;
		}
	}
}

bool SoundManager::IsPlaySound(string keyName)
{
	bool isPlay;
	arrSoundsIter iter = mTotalSounds.begin();

	int count = 0;

	for (iter; iter != mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool SoundManager::IsPauseSound(string keyName)
{
	bool isPause;
	arrSoundsIter iter = mTotalSounds.begin();

	int count = 0;

	for (iter; iter != mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			channel[count]->getPaused(&isPause);
			break;
		}
	}

	return isPause;
}
