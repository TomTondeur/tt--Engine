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
#include "../Helpers/Namespace.h"
#include "../Helpers/resrc_ptr.hpp"

class Material;
class Model3D;
class MeshAnimator;
class TransformComponent;

class ModelComponent : public ObjectComponent
{
public:
	//Constructor & destructor
	ModelComponent(std::tstring modelFilename, const TransformComponent* pTransform);
	virtual ~ModelComponent(void);

	//Methods
	virtual void Initialize(void) override;
	virtual void Update(const tt::GameContext& context) override;
	virtual void Draw(const tt::GameContext& context) override;
	virtual void DrawDeferred(const tt::GameContext& context);

	void SetMaterial(resource_ptr<Material> pMat);
	const TransformComponent* GetTransform(void) const;
	
	bool Cull(const tt::GameContext& context);

private:
	//Datamembers
	std::tstring m_ModelFile;
	resource_ptr<Model3D> m_pModel;
	resource_ptr<Material> m_pMaterial;
	const TransformComponent* m_pTransform;
	MeshAnimator* m_pMeshAnimator;

	//Disabling default copy constructor & assignment operator
	ModelComponent(const ModelComponent& src);
	ModelComponent& operator=(const ModelComponent& src);
};
