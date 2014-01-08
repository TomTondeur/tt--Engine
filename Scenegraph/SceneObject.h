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
// SceneObject.h : file containing class that represents an entity in a scene
// Copyright © 2013 Tom Tondeur
//

#pragma once

#include "../Helpers/Namespace.h"
#include "ObjectComponent.h"
#include "../Helpers/TemplateUtil.h"

class GameScene;

class SceneObject
{
public:
	SceneObject();
	virtual ~SceneObject();
	
	virtual void InitializeObject(void)=0;
	virtual void UpdateObject(const tt::GameContext& context)=0;
	virtual void DrawObject(const tt::GameContext& context)=0;

	virtual void Initialize(void);
	virtual void Update(const tt::GameContext& context);
	virtual void Draw(const tt::GameContext& context);

	virtual void GenerateShadows(const tt::GameContext& context);

private:

	SceneObject(const SceneObject& src);
	SceneObject& operator=(const SceneObject& src);
};

template<typename... T>
class GenericSceneObject : public SceneObject
{
public:
	GenericSceneObject()
	{
		//m_Components.Allocate<T...>();
	}
	
	virtual ~GenericSceneObject(void)
	{
		//m_Components.Release<T...>();
	}

	void InitializeObject(void)
	{
		m_Components.ForEach<T...>([](ObjectComponent* pComp)
										{
											pComp->Initialize();
										});
	}
	
	void UpdateObject(const tt::GameContext& context)
	{
		m_Components.ForEach<T...>([&](ObjectComponent* pComp)
										{
											pComp->Update(context);
										});
	}
		
	void DrawObject(const tt::GameContext& context)
	{
		m_Components.ForEach<T...>([&](ObjectComponent* pComp)
										{
											pComp->Draw(context);
										});
	}

	template<typename ComponentType, size_t index = 0>
	ComponentType* GetComponent(void)
	{
		static_assert(contains_same<ComponentType, T...>::value, "Provided type does not belong to this class");
		//return m_Components.GetMember<ComponentType>();
		return m_Components.Get<ComponentType, index>();
	}
	/*
	template<typename ComponentType, typename _Pr>
	ComponentType* GetComponent(_Pr _Pred)
	{
		static_assert(contains_same<ComponentType, T...>::value, "Provided type does not belong to this class");
		return m_Components.GetMember<ComponentType>(_Pred);
	}
	*/
	template<typename ComponentType, size_t index = 0>
	void SetComponent(ComponentType* ptr)
	{
		static_assert(contains_same<ComponentType, T...>::value, "Provided type does not belong to this class");
		//m_Components.SetMember<ComponentType, index, T...>(ptr);
		m_Components.Set<ComponentType, index>(ptr);
	}

	/*
protected:
	template<typename ComponentType>
	void AddComponent(ComponentType* pComponent)
	{
		m_Components.AddMember<ComponentType, T...>(pComponent);
	}
	*/
private:
	//MemberMapper<ObjectComponent> m_Components;
	Tuple<T...> m_Components;

	GenericSceneObject(const GenericSceneObject& src);// = delete;
	GenericSceneObject& operator=(const GenericSceneObject& src);// = delete;
};
