#include "pch.h"
#include "SoundManager.h"
#include "SoundEffect.h"
#include <iostream>

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	for (pair<string, SoundEffect*> p : m_Map)
	{
		delete p.second;
		p.second = nullptr;
	}
}

SoundEffect* SoundManager::GetSoundPointer(const string& fileName, const int volume)
{
	SoundEffect* pSound{ m_Map[fileName] };

	if (!pSound)
	{
		m_Map[fileName] = new SoundEffect{ fileName }; \

		m_Map[fileName]->SetVolume(volume);

		return m_Map[fileName];
	}

	
	
	return m_Map[fileName];
}

void SoundManager::RegulateVolume(const int volume)
{
	for (pair<string, SoundEffect*> p : m_Map)
	{
		p.second->SetVolume(volume);
	}
}
