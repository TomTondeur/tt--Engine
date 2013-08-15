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

#include "../Helpers/Namespace.h"
#include "../Helpers/resrc_ptr.hpp"
#include "../Scenegraph/ObjectComponent.h"

class TransformComponent;

struct Sprite
{
	Sprite(const tt::Matrix4x4& transform);

	ID3D10ShaderResourceView* pTexture;
	tt::Vector4 Color;
	const tt::Matrix4x4& Transform;
};

class SpriteComponent : public ObjectComponent
{
public:
	//Default constructor & destructor
	SpriteComponent(std::tstring filename, const TransformComponent* pTransform);
	virtual ~SpriteComponent(void);

	//Methods
	virtual void Initialize(void) override;
	virtual void Draw(const tt::GameContext& context) override;

private:
	//Datamembers
	std::tstring m_TextureFilename;
	Sprite m_Sprite;
		
	//Disabling default copy constructor & assignment operator
	SpriteComponent(const SpriteComponent& src);
	SpriteComponent& operator=(const SpriteComponent& src);
};
