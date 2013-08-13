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

#include "Service.h"
#include "../../Helpers/resrc_ptr.hpp"
#include "../../Helpers/Namespace.h"

class GraphicsDevice;
class TTengine;
class Window;
class Model3D;
class Material;
struct GameContext;
class SpriteBatch;

class IGraphicsService : public Service
{
public:
	//Default constructor & destructor
	IGraphicsService(void){};
	virtual ~IGraphicsService(void){};

	//Methods
	virtual void InitWindow(int windowWidth, int windowHeight, TTengine* pEngine)=0;

	virtual void Draw(resource_ptr<Model3D> pModel, const tt::Matrix4x4& worldMat, resource_ptr<Material> pMat, const tt::GameContext& context)=0;

	virtual GraphicsDevice* GetGraphicsDevice(void) const=0;
	virtual Window* GetWindow(void) const=0;
	virtual SpriteBatch* GetSpriteBatch(void) const=0;

private:
	//Datamembers


	//Disabling default copy constructor & assignment operator
	IGraphicsService(const IGraphicsService& src);// = delete;
	IGraphicsService& operator=(const IGraphicsService& src);// = delete;
};
