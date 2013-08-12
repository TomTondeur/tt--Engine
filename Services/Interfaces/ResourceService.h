//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// ResourceService.h : Class description for Resource service
// Copyright © 2013 Tom Tondeur
//

#pragma once

#include "Service.h"
#include "../../Helpers/stdafx.h"
#include "../../Helpers/resrc_ptr.hpp"
#include "../../Helpers/TemplateUtil.h"
//#include "../Implementations/ResourceLoaders/ResourceLoader.h"
#include "../../Diagnostics/Exceptions.h"

class ResourceService : public Service
{
public:
	//Default constructor & destructor
	ResourceService(void){};
	virtual ~ResourceService(void){};
	
	//Methods
	template <typename T> resource_ptr<T> Load(std::tstring filePath)
	{
		return ResourceLoader<T>::Load(filePath);
	}
	
	//This template is specialized in the corresponding .cpp file
	template <typename T> static std::unique_ptr<T> LoadResource(std::tstring filePath);

private:
	//Disabling default copy constructor & assignment operator
	ResourceService(const ResourceService& src);// = delete;
	ResourceService& operator=(const ResourceService& src);// = delete;
};

template<typename T>
class ResourceLoader
{
public:
	static resource_ptr<T> Load(std::tstring filename)
	{
		auto it = m_Resources.find(filename);
		
		if(it != m_Resources.end())
			return resource_ptr<T>(it->second);
		
		std::unique_ptr<T>& pResrc = m_Resources.insert(make_pair(filename, ResourceService::LoadResource<T>(filename) ) ).first->second;
		
		return resource_ptr<T>(pResrc);
	}

private:
	//Datamembers
	static std::map<std::tstring, std::unique_ptr<T>> m_Resources;
		
	//Disabling default constructor, copy constructor & assignment operator
	ResourceLoader(void);
	ResourceLoader(const ResourceLoader& src);// = delete;
	ResourceService& operator=(const ResourceLoader& src);// = delete;
};

template <typename T>
std::map<std::tstring, std::unique_ptr<T>> ResourceLoader<T>::m_Resources = std::map<std::tstring, std::unique_ptr<T>>();
