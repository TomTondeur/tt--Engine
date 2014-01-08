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
class CompositeDeferredMaterial;

class DefaultGraphicsService : public IGraphicsService
{
public:
	//Default constructor & destructor
	DefaultGraphicsService(void);
	virtual ~DefaultGraphicsService(void);

	//Methods
	
	virtual void InitWindow(int windowWidth, int windowHeight, TTengine* pEngine) override;	
	
	virtual void Draw(resource_ptr<Model3D> pModel, const tt::Matrix4x4& worldMat, resource_ptr<Material> pMat, const tt::GameContext& context) override;
	
	virtual void PrepareShadowGeneration(void) override;
	virtual void GenerateShadows(resource_ptr<Model3D> pModel, const tt::Matrix4x4& worldMat, resource_ptr<Material> pMat, const tt::GameContext& context) override;

	virtual void PrepareDeferredShading(void) override;
	virtual void DrawDeferred(resource_ptr<Model3D> pModel, const tt::Matrix4x4& worldMat, resource_ptr<Material> pMat, const tt::GameContext& context) override;
	
	virtual void CompositeDeferredShading(const tt::GameContext& context) override;
	virtual Sprite RenderPostProcessing(const tt::GameContext& context, std::multimap<unsigned int, PostProcessingEffect*, std::greater_equal<unsigned int> >& postProEffects) override;

	virtual RenderTarget2D* GetShadowMapRenderTarget() const override;
	virtual GraphicsDevice* GetGraphicsDevice(void) const override;
	virtual Window* GetWindow(void) const override;
	virtual SpriteBatch* GetSpriteBatch(void) const override;

private:
	//Datamembers
	GraphicsDevice* m_pGraphicsDevice;
	Window* m_pWindow;
	SpriteBatch* m_pSpriteBatch;

	//Swapchain for post-processing
	RenderTarget2D* m_pSwapRT1;
	RenderTarget2D* m_pSwapRT2;

	//shadow map rendertarget
	RenderTarget2D* m_pShadowMapRenderTarget;

	//DirectX resources for deferred shading
	ID3D10Texture2D* m_pPositionTexture;
	ID3D10RenderTargetView* m_pPositionRT;
	ID3D10ShaderResourceView* m_pPositionSRV;
	
	ID3D10Texture2D* m_pNormalTexture;
	ID3D10RenderTargetView* m_pNormalRT;
	ID3D10ShaderResourceView* m_pNormalSRV;

	ID3D10DepthStencilView* m_pDeferredDepthStencilView;

	//Postprocessing effect to composite
	PostProcessingEffect* m_pCompositeDeferredShadingPostProEffect;
	CompositeDeferredMaterial* m_pCompositeDeferredShadingMaterial;
	
	bool m_bUseDeferredShading;

	void InitializeGBuffers(void);
		
	//Disabling default copy constructor & assignment operator
	DefaultGraphicsService(const DefaultGraphicsService& src);
	DefaultGraphicsService& operator=(const DefaultGraphicsService& src);
};
