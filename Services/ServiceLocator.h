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
// ServiceLocator.h : A base class provided to create a service locator. This is the access point for services provided by the tt::Engine, such as Audio, Graphics, Input, File I/O...
// Copyright © 2013 Tom Tondeur
//

#pragma once

#include "ServiceIncludes.h"
#include "../Helpers/TemplateUtil.h"
#include <memory>

template<typename... SupportedServiceInterfaces>
class ServiceLocator
{
public:
	//Default constructor & destructor
	ServiceLocator()
	{

	}

	virtual ~ServiceLocator()
	{

	}

	//Methods
	template<typename T>
	T* GetService()
	{
		return m_Services.Get<T>();
	}

	template<typename Interface, typename Implementation>
	void SetService()
	{
		/*
		Interface* pInterface = m_Services.GetMember<Interface>();
		Interface* pImplementation = nullptr;
		vector<Interface*> ptrs;

		m_Services.ForEach<Interface>([&](Interface*& _pInterface){
			if(dynamic_cast<Implementation*>(_pInterface)){
				pImplementation = _pInterface;
				pInterface = pImplementation;
			}
		});
		
		if(pImplementation!=nullptr)
			m_Services.SetMember<Interface,0,SupportedServiceInterfaces...>(pInterface);
		else
			DEBUG_BREAK
			*/
	}
	
protected:
	template<typename T>
	void AddService(T* pService)
	{
		m_Services.Set<T>(pService);
	}

private:
	//Datamembers
	Tuple<SupportedServiceInterfaces...> m_Services;

	//Disabling default copy constructor & assignment operator
	ServiceLocator(const ServiceLocator& src);// = delete;
	ServiceLocator& operator=(const ServiceLocator& src);// = delete;
};

class MyServiceLocator : public ServiceLocator<IGraphicsService, IInputService, ResourceService, DebugService, IPhysicsService>
{
public:
	virtual ~MyServiceLocator(void);

	static MyServiceLocator* GetInstance(void);

private:
	MyServiceLocator(void);

	static std::unique_ptr<MyServiceLocator> s_pInstance;

	MyServiceLocator(const MyServiceLocator& src);// = delete;
	MyServiceLocator& operator=(const MyServiceLocator& src);// = delete;
};

//---------------------------
// Error checker for HRESULTs
// We don't use a function because then __FILE__ and __LINE__ wouldn't point to where the function is used, but to here
//---------------------------

#ifndef HR
	#define HR(x)												\
	{															\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
			MyServiceLocator::GetInstance()->GetService<DebugService>()->LogDirectXError(hr, __LINE__, __FILE__);	\
	}
#endif
