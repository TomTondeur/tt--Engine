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

#include "../Scenegraph/ObjectComponent.h"
#include "../Helpers/resrc_ptr.hpp"

class Material;
class TransformComponent;
class RenderTarget2D;
struct Sprite;

class ParticleEmitterComponent : public ObjectComponent
{
public:
	//Default constructor & destructor
	ParticleEmitterComponent(resource_ptr<Material> pMat, const TransformComponent* pTransform, unsigned int nrOfParticles=30000);
	virtual ~ParticleEmitterComponent(void);

	//Methods
	virtual void Initialize(void) override;
	virtual void Draw(const tt::GameContext& context) override;

	static Sprite RenderDeferred(const tt::GameContext& context);

private:
	//Datamembers
	ID3D10Buffer* m_pInitVB;
	ID3D10Buffer* m_pUpdateVB;
	ID3D10Buffer* m_pDrawVB;
	
	resource_ptr<Material> m_pMaterial;
	const TransformComponent* m_pTransform;

	unsigned int m_VertexStride, m_NrOfParticles;

	static std::vector<ParticleEmitterComponent*> s_DeferredParticles;
	static std::unique_ptr<RenderTarget2D> s_pRenderTarget;

	void CreateVertexBuffers(void);

	//Disabling default copy constructor & assignment operator
	ParticleEmitterComponent(const ParticleEmitterComponent& src);
	ParticleEmitterComponent& operator=(const ParticleEmitterComponent& src);
};
