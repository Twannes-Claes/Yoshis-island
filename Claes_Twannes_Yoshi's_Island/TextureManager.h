#pragma once
#include <string>
#include <unordered_map>
#include "utils.h"

using namespace std;

class Texture;

using MyTextureMap = unordered_map<string, Texture*>;

class TextureManager final
{
	public:

		TextureManager() = default;

		~TextureManager();

		TextureManager(const TextureManager& other) = delete;
		TextureManager& operator=(const TextureManager& other) = delete;
		TextureManager(TextureManager&& other) = delete;
		TextureManager& operator=(TextureManager&& other) = delete;

		Texture* GetTexturePointer(const string& fileName);	
		Texture* GetTexturePointerByFont(const string& text,const string& fontPath, const int textSize, const Color4f& color);

	private:
		
		MyTextureMap m_Map;
};

