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

//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// GameScene.h : file containing class that holds scene objects
// Copyright © 2013 Tom Tondeur
//

#pragma once

#include "SceneObject.h"

class CameraComponent;

class GameScene
{
public:
	GameScene();
	virtual ~GameScene();

	static GameScene* GetActiveScene(void);
	
	void InitializeScene(void);
	void UpdateScene(const tt::GameContext& context);
	void DrawScene(const tt::GameContext& context);
	
	virtual void Initialize(void);
	virtual void Update(const tt::GameContext& context);
	virtual void Draw(const tt::GameContext& context);

	std::tstring GetName(void) const;
	NxScene* GetPhysicsScene(void) const;
	void SetActiveCamera(CameraComponent* pCam);
	const CameraComponent* GetActiveCamera(void) const;

protected:
	void AddSceneObject(SceneObject* pObject);

private:
	vector<SceneObject*> m_Objects;
	std::tstring m_Name;
	NxScene* m_pPhysicsScene;
	CameraComponent* m_pActiveCamera;
	static GameScene* s_pActiveScene;

	GameScene(const GameScene& src);// = delete;
	GameScene& operator=(const GameScene& src);// = delete;
};
