#pragma once

#include <string>
#include <unordered_map>

using namespace std;

class SoundEffect;

using MySoundMap = unordered_map<string, SoundEffect*>;

class SoundManager final
{
	public:

		SoundManager();

		~SoundManager();

		SoundManager(const SoundManager& other) = delete;
		SoundManager& operator=(const SoundManager& other) = delete;
		SoundManager(SoundManager&& other) = delete;
		SoundManager& operator=(SoundManager&& other) = delete;

		SoundEffect* GetSoundPointer(const string& fileName, const int volume);

		void RegulateVolume(const int volume);

	private:

		MySoundMap m_Map;
};

