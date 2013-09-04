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
//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "../Interfaces/IPhysicsService.h"
class DebugRenderer;

class PhysicsAllocator : public NxUserAllocator
{
public:        
    void * malloc(NxU32 size)
    {
        return ::malloc(size);
    }
    
    void * mallocDEBUG(NxU32 size,const char *fileName, int line)
    {
        return ::_malloc_dbg(size,_NORMAL_BLOCK, fileName, line);
    }
    
    void * realloc(void * memory, NxU32 size)
    {
        return ::realloc(memory,size);
    }
    
    void free(void * memory)
    {
		::free(memory);
    }
};

class DefaultPhysicsService : public IPhysicsService
{
public:
	DefaultPhysicsService(void);
	virtual ~DefaultPhysicsService(void);

	virtual void Initialize(void) override;

	virtual void Simulate(float gameTime) override;
	virtual void FetchResults(void) override;

	virtual void RenderDebugInfo(const tt::GameContext& context) override;

	virtual NxPhysicsSDK* GetPhysicsSDK(void) override;

	virtual NxScene* CreateScene(void) override;
	virtual void RemovePhysicsScene(GameScene* scene) override;
	
	virtual void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status) override;

	virtual NxController* CreateController(GameScene* pGameScene, NxCapsuleControllerDesc desc) override;

	virtual NxShape* GetClosestShape(NxRay& ray, NxShapesType shapetype, float distance) const override;

	virtual void SetActiveScene(NxScene* pScene) override;
	virtual NxScene* GetActiveScene(void) const override;

	virtual SceneObject* Pick(const POINT& mousePosition, const tt::GameContext& context) const override;

private:
	NxPhysicsSDK *m_pPhysicsSDK;

	NxControllerManager* m_pControllerManager;
	PhysicsAllocator* m_pAllocator;
	std::multimap<std::tstring, NxController*> m_Controllers;
	DebugRenderer* m_pDebugRenderer;
	NxScene* m_pActiveScene;

	DefaultPhysicsService(const DefaultPhysicsService& t);// = delete;
	DefaultPhysicsService& operator=(const DefaultPhysicsService& t);// = delete;
};
