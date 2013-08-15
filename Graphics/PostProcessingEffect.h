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

class RenderTarget2D;
class PostProcessingMaterial;

class PostProcessingEffect
{
public:
	//Default constructor & destructor
	PostProcessingEffect(resource_ptr<PostProcessingMaterial> pMaterial);
	virtual ~PostProcessingEffect(void);

	//Methods
	void Initialize(void);

	void Draw(const tt::GameContext& context, RenderTarget2D* pInputRT);

private:
	//Datamembers
	resource_ptr<PostProcessingMaterial> m_pMaterial;

	static ID3D10Buffer* s_pVertexBuffer;
	static unsigned int s_VertexBufferStride;

	//Disabling default copy constructor & assignment operator
	PostProcessingEffect(const PostProcessingEffect& src);
	PostProcessingEffect& operator=(const PostProcessingEffect& src);
};
