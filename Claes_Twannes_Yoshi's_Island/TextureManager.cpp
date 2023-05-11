#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"

TextureManager::~TextureManager()
{
	for (pair<string, Texture*> p : m_Map)
	{
		delete p.second;
		p.second = nullptr;
	}
}

Texture* TextureManager::GetTexturePointer(const string& fileName)
{
	/*if (m_Map.find(fileName) == m_Map.end())
	{
		return m_Map[fileName] = new Texture{ fileName };
	}

	return m_Map[fileName];*/

	Texture* pTex{ m_Map[fileName] };

	if (!pTex)
	{
		return m_Map[fileName] = new Texture{ fileName };
	}

	return m_Map[fileName];

}

Texture* TextureManager::GetTexturePointerByFont(const string& text, const string& fontPath, const int textSize, const Color4f& color)
{
	Texture* pTex{ m_Map[text] };

	if (!pTex)
	{
		return m_Map[text] = new Texture(text, fontPath, textSize, color);
	}

	return m_Map[text];
}
