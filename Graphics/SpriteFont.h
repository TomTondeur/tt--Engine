// Copyright © 2013 Tom Tondeur
// 
// This file is part of tt::Engine.
// 
// tt::Engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// tt::Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with tt::Engine.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "../Helpers/stdafx.h"
#include "../Helpers/resrc_ptr.hpp"
#include "../Helpers/Namespace.h"

typedef unsigned char BMFONT_CHAR;
typedef std::string BMFONT_STRING;
#define _BM 

struct SpriteFontGlyphInfo
{
	tt::Vector2 TexCoord;
	tt::Vector2 Dimensions;
	tt::Vector2 OffsetTexToScreen;
	float AdvanceX;
	unsigned char Page;
	unsigned char Channel;
};

struct TextData
{
	TextData(const std::tstring& text, const tt::Vector2& pos, const tt::Vector4& color);

	BMFONT_STRING Text;
	tt::Vector2 Position;
	tt::Vector4 Color;
};

class TextMaterial;

class SpriteFont
{
	friend class ResourceService;
	friend class SpriteBatch;

public:
	//Default constructor & destructor
	SpriteFont(void);
	virtual ~SpriteFont(void);

	//Methods
	static void Initialize();
	
	void DrawText(const std::tstring& text, tt::Vector2 position, const tt::Vector4& color);
	void Flush(void);

	static const int sc_MaxNrOfChars = 100;

private:
	//Datamembers
	unsigned int m_FontSize;
	std::tstring m_FontName;
	tt::Vector2 m_TextureDimensions;
	resource_ptr<ID3D10ShaderResourceView> m_pTexture;	
	std::map<BMFONT_CHAR, SpriteFontGlyphInfo> m_Characters;

	std::vector<TextData> m_TextQueue;
	size_t m_NrOfCharsToDraw;

	static resource_ptr<TextMaterial> s_pMaterial;
	static ID3D10Buffer* s_pVertexBuffer;
	static unsigned int s_VertexBufferStride;

	//Disabling default copy constructor & assignment operator
	SpriteFont(const SpriteFont& src);
	SpriteFont& operator=(const SpriteFont& src);
};
