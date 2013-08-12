//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// ObjectComponent.h : file containing class that represents a component part of an entity in a GameScene
// Copyright © 2013 Tom Tondeur
//

#pragma once

#include "../Helpers/stdafx.h"
#include "../Helpers/Namespace.h"

class ObjectComponent
{
	friend class SceneObject;
public:
	ObjectComponent();
	virtual ~ObjectComponent();
	
	virtual void Initialize();
	virtual void Update(const tt::GameContext& context);
	virtual void Draw(const tt::GameContext& context);

	virtual void SetActive(bool b);
	bool IsActive(void) const;

private:
	bool m_bActive;

	ObjectComponent(const ObjectComponent& src);
	ObjectComponent& operator=(const ObjectComponent& src);
};
