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

#include <memory>

#include "D3DUtil.h"

namespace std
{
	template<>
	struct default_delete< ID3D10Effect >
	{
	public:
		void operator()(ID3D10Effect* ptr)
		{
			ptr->Release();
		}
	};

	template<>
	struct default_delete< ID3D10ShaderResourceView >
	{
	public:
		void operator()(ID3D10ShaderResourceView* ptr)
		{
			ptr->Release();
		}
	};

	template<>
	struct default_delete< NxConvexMesh >
	{
	public:
		void operator()(NxConvexMesh* ptr)
		{
			NxGetPhysicsSDK()->releaseConvexMesh(*ptr);
		}
	};
	
	template<>
	struct default_delete< NxTriangleMesh >
	{
	public:
		void operator()(NxTriangleMesh* ptr)
		{
			NxGetPhysicsSDK()->releaseTriangleMesh(*ptr);
		}
	};
}

template <typename T>
class resource_ptr{
	template <typename U> friend class resource_ptr;

public:
	resource_ptr(void) : pData(nullptr) {}
	resource_ptr(const std::unique_ptr<T>& masterPtr) : pData(&masterPtr) {}
	
	T* operator->() const
	{	
		return pData->get();
	}

	T* get(void) const
	{
		return pData->get();
	}

	bool operator==(const T* ptr) const
	{
		return pData->get() == ptr;
	}
	
	bool operator==(const resource_ptr<T>& ptr) const
	{
		return pData->get() == *ptr->pData;
	}
	
	bool operator!=(const T* ptr) const
	{
		return pData->get() != ptr;
	}
	
	bool operator!=(const resource_ptr<T>& ptr) const
	{
		return pData->get() != *ptr->pData;
	}

	template<typename U>
	operator resource_ptr<U>(void)
	{
		static_assert(std::is_base_of<U, T>::value, "Cannot cast to resource_ptr of non-Base type.");
		resource_ptr<U> output;
		output.pData = reinterpret_cast<const std::unique_ptr<U>*>(pData);
		return output;
	}

private:
	const std::unique_ptr<T>* pData;
};
/*
template<typename T>
struct DefaultResourceDeleter
{
	void operator()(T* ptr)
	{
		delete ptr;
	}
};

template<typename T, typename DeleterType = DefaultResourceDeleter<T> > class resrc_ptr{
	 template<typename T> friend class resrc_inst_ptr;
public:
	resrc_ptr(void):pData(nullptr), _Delete(DefaultResourceDeleter<T>()){}
	
	resrc_ptr(T* pResource):pData(pResource), _Delete(DefaultResourceDeleter<T>()){}

	resrc_ptr(T* pResource, DeleterType _delete):pData(pResource), _Delete(_delete){}

	resrc_ptr(resrc_ptr<T>&& src)
	{
		pData = src.pData;
		pData = nullptr;
	}
	
	resrc_ptr<T>& operator=(resrc_ptr<T>&& src)
	{
		if(pData != src.pData){
			delete pData;
			pData = src.pData;
			src.pData = 0;
		}
		return *this
	}
//TODO: Fix warning when deleting temporary resrc_ptr in ResourceService.h
//#pragma warning(disable : 4150)
	~resrc_ptr(void)
	{
		if(pData)
			_Delete(pData);
	}
//#pragma warning(default : 4150)

	resrc_ptr& operator=(T* pResource)
	{
		delete pData;
		pData = pResource;
		return *this;
	}

	T* operator*(void) const
	{
		return pData;
	}
	
	T* operator->(void) const
	{
		return pData;
	}

private:
	T* pData;
	DeleterType _Delete;
	
	//Sharing ownership between resrc_ptrs is not allowed, use resrc_inst_ptr instead
	resrc_ptr(const resrc_ptr& src);// = delete;
	resrc_ptr& operator=(const resrc_ptr& src);// = delete;
};

template<typename T> class resrc_inst_ptr
{
public:
	resrc_inst_ptr(void):ppData(nullptr){}

	explicit resrc_inst_ptr(resrc_ptr<T>& masterPtr):ppData(&masterPtr.pData){}
	
	resrc_inst_ptr(const resrc_inst_ptr<T>& src):ppData(src.ppData){}

	~resrc_inst_ptr(void){}

	resrc_inst_ptr& operator=(resrc_ptr<T>& masterPtr)
	{
		ppData = &masterPtr.pData;
		return *this;
	}
	
	resrc_inst_ptr& operator=(const resrc_inst_ptr<T>& src)
	{
		ppData = src.ppData;
		return *this;
	}
	
	T* operator*(void) const
	{
		if(ppData)
			return *ppData;
		else return nullptr;
	}
	
	T* operator->(void) const
	{
		if(ppData)
			return *ppData;
		else return nullptr;
	}

private:
	T** ppData;
};
*/