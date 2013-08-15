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

#include "../Interfaces/ResourceService.h"
#include "../../Helpers/BinaryReader.h"
#include "../ServiceLocator.h"
#include "../../Diagnostics/Exceptions.h"
#include "../../Graphics/SpriteFont.h"

template <>
std::unique_ptr<SpriteFont> ResourceService::LoadResource<SpriteFont>(const std::tstring& filePath)
{
	//File format documentation: file:///C:/Program%20Files%20%28x86%29/AngelCode/BMFont/doc/file_format.html
	BinaryReader binReader(filePath);
	auto pDebugService = MyServiceLocator::GetInstance()->GetService<DebugService>();
	
	//Parse the Identification bytes (B,M,F)
	char idBytes[4];
	binReader.ReadArray<char>(idBytes, 3);
	idBytes[3] = '\0';

	//If Identification bytes doesn't match BMF,
	//Log Error (Not a valid .fnt font) &
	tstringstream errors;
	if(strcmp(idBytes, "BMF") != 0)
		errors << _T("Not a valid .fnt font, detected following identification bytes: ") << idBytes << _T(" (should be BMF)") << endl;

	//Parse the version (version 3 required)
	auto versionId = binReader.Read<char>();	
	if(versionId != 3)
		errors << _T("Only .fnt version 3 is supported, detected version: ") << versionId;
	
	//Log errors if applicable
	if(strcmp(idBytes, "BMF") != 0 || versionId != 3)
		throw LoaderException(std::tstring(_T("bmfont") + filePath), errors.str() );
	
	//Create SpriteFont
	auto pSpriteFont = new SpriteFont();
	
	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	auto blockId0 = binReader.Read<char>();
	auto blockSize0 = binReader.Read<int>();
	
	pSpriteFont->m_FontSize = -binReader.Read<short>(); //Retrieve the FontSize (will be -25, BMF bug) [SpriteFont::m_FontSize]
	
	binReader.Advance(12); //Skip stuff like padding and spacing

	pSpriteFont->m_FontName = binReader.ReadNullTerminatedString(); //Retrieve the FontName [SpriteFont::m_FontName]
	
	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	auto blockId1 = binReader.Read<char>();
	auto blockSize1 = binReader.Read<int>();
	
	binReader.Advance(4);

	//Retrieve Texture Width & Height [SpriteFont::m_TextureWidth/Height]
	pSpriteFont->m_TextureDimensions.x = binReader.Read<unsigned short>();
	pSpriteFont->m_TextureDimensions.y = binReader.Read<unsigned short>();

	//Check if pagecount is valid
	unsigned short pageCount = binReader.Read<unsigned short>();
	if(pageCount > 1)
		throw LoaderException(std::tstring(_T("bmfont")) + filePath, std::tstring(_T("SpriteFont (.fnt): Only one texture per font allowed, detected ") ) + to_tstring(pageCount) + _T(" textures in ") );
		
	binReader.Advance(5); //Advance to Block 2
	
	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	auto blockId2 = binReader.Read<char>();
	auto blockSize2 = binReader.Read<int>();

	std::tstring pageName = binReader.ReadNullTerminatedString(); //Retrieve the page name
	
	//Empty page name -> invalid font
	if(pageName.size() == 0)
		throw LoaderException(std::tstring(_T("bmfont")) + filePath, _T("SpriteFont (.fnt): Invalid Font Sprite [Empty]") );
	
	//Get bmfont file directory
	std::tstring texFilePath = filePath.substr(0, filePath.find_last_of(_T('/') ) + 1);

	//Load page texture
	pSpriteFont->m_pTexture = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<ID3D10ShaderResourceView>(texFilePath + pageName);
	
	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	auto blockId3 = binReader.Read<char>();
	auto blockSize3 = binReader.Read<int>();

	//Retrieve Character Count (see documentation)
	unsigned int charCount = blockSize3/20;

	//Retrieve Every Character, For every Character:
	for(unsigned int i = 0; i < charCount; ++i){
		//> Retrieve CharacterId
		auto charId = binReader.Read<unsigned int>();
		if( charId > std::numeric_limits<BMFONT_CHAR>::max() ){
			errors << _T("Invalid character found: ") << charId;
			continue;
		}

		// Create new glyph
		SpriteFontGlyphInfo glyphInfo;
		
		auto xPos = binReader.Read<unsigned short>();
		auto yPos = binReader.Read<unsigned short>();
		glyphInfo.TexCoord.x = xPos / pSpriteFont->m_TextureDimensions.x;
		glyphInfo.TexCoord.y = yPos / pSpriteFont->m_TextureDimensions.y;

		glyphInfo.Dimensions.x = binReader.Read<unsigned short>();
		glyphInfo.Dimensions.y = binReader.Read<unsigned short>();
		
		glyphInfo.OffsetTexToScreen.x = binReader.Read<short>();
		glyphInfo.OffsetTexToScreen.y = binReader.Read<short>();
		
		glyphInfo.AdvanceX = binReader.Read<short>();
		
		glyphInfo.Page = binReader.Read<unsigned char>();
		
		auto bmfChannel = binReader.Read<unsigned char>();
		switch(bmfChannel){
			case 1: //blue
				glyphInfo.Channel = 2;
				break;
			case 2: //green
				glyphInfo.Channel = 1;
				break;
			case 15: //all channels
			case 4: //red
				glyphInfo.Channel = 0;
				break;

			case 8: //alpha
				glyphInfo.Channel = 3;
				break;
		}
		
		//Add glyph to character set
		pSpriteFont->m_Characters.insert(make_pair(static_cast<BMFONT_CHAR>(charId),  glyphInfo) );
	}

	//Ignore kerning, no one cares

	if( !errors.str().empty() )
		throw LoaderException(std::tstring(_T("bmfont")) + filePath, errors.str() );

	pSpriteFont->Initialize();

	return std::unique_ptr<SpriteFont>(pSpriteFont);
}
