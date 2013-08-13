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
#include "../Helpers/D3DUtil.h"
#include "../Helpers/Namespace.h"
#include "../Helpers/resrc_ptr.hpp"

struct Sprite;
struct SpriteVertex;
class SpriteMaterial;

class SpriteBatch
{
public:
	//Default constructor & destructor
	SpriteBatch(void);
	virtual ~SpriteBatch(void);

	//Methods
	void Initialize(void);
	void Draw(const Sprite& sprite);
	void Flush(const tt::GameContext& context);

	static const unsigned int sc_MaxNrOfSprites = 50;

private:
	//Datamembers
	std::map<ID3D10ShaderResourceView*, std::vector<SpriteVertex> > m_Sprites;
	resource_ptr<SpriteMaterial> m_pMaterial;
	
	ID3D10Buffer* m_pVertexBuffer;
	unsigned int m_VertexBufferStride;

	unsigned int m_NrOfSprites;

	//Disabling default copy constructor & assignment operator
	SpriteBatch(const SpriteBatch& src);
	SpriteBatch& operator=(const SpriteBatch& src);
};
