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

#include "../Interfaces/IGraphicsService.h"

class RenderTarget2D;

class DefaultGraphicsService : public IGraphicsService
{
public:
	//Default constructor & destructor
	DefaultGraphicsService(void);
	virtual ~DefaultGraphicsService(void);

	//Methods
	virtual void InitWindow(int windowWidth, int windowHeight, TTengine* pEngine) override;	

	virtual void Draw(resource_ptr<Model3D> pModel, const tt::Matrix4x4& worldMat, resource_ptr<Material> pMat, const tt::GameContext& context) override;
	virtual ID3D10ShaderResourceView* RenderPostProcessing(const tt::GameContext& context, std::multimap<unsigned int, PostProcessingEffect*, std::greater_equal<unsigned int> >& postProEffects) override;

	virtual GraphicsDevice* GetGraphicsDevice(void) const override;
	virtual Window* GetWindow(void) const override;
	virtual SpriteBatch* GetSpriteBatch(void) const override;

private:
	//Datamembers
	GraphicsDevice* m_pGraphicsDevice;
	Window* m_pWindow;
	SpriteBatch* m_pSpriteBatch;

	RenderTarget2D* m_pSwapRT1;
	RenderTarget2D* m_pSwapRT2;

	//Disabling default copy constructor & assignment operator
	DefaultGraphicsService(const DefaultGraphicsService& src);
	DefaultGraphicsService& operator=(const DefaultGraphicsService& src);
};
