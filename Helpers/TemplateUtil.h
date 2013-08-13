#pragma once

#include <functional>
#include <vector>
#include <type_traits>

class MemberNotFoundException{};

class TemplateUtil{
public:
	template <typename... T>
	static size_t GetNrOfArguments ()
	{
		return Num_Args<T...>::calculate();
	}

private:
	template <typename... T>
	class Num_Args;

	template <>
	struct Num_Args <>
	{
		static size_t calculate() {
			return 0;
		}
	};

	template <typename H, typename... T>
	struct Num_Args <H, T...>
	{
		static size_t calculate() {
			return 1 + Num_Args<T...>::calculate();
		}
	};
};

template<typename FirstType, typename... Tail>
class Tuple
{
public:
	Tuple(void) : pFirstType(nullptr), tailTuple() { }

	~Tuple(void)
	{
		delete pFirstType;
	}
	
	template<typename T, size_t index>
	//DON'T CALL THIS
	T* Get_Unsafe(void)
	{
		if(std::is_same<FirstType, T>::value){
			if(index == 0)
				return reinterpret_cast<T*>( pFirstType );
			else 
				return reinterpret_cast<T*>( tailTuple.Get_Unsafe<T, index - 1>() );
		}
		else
			return reinterpret_cast<T*>( tailTuple.Get_Unsafe<T, index>() );
	}

	template<typename T, size_t index=0>
	T* Get(void)
	{
		static_assert(contains_same<T, FirstType, Tail...>::value,"This Tuple does not contain a variable of type T");
		return Get_Unsafe<T, index>();
	}
		
	template<typename T, size_t index>
	//DON'T CALL THIS
	void Set_Unsafe(T* value)
	{
		if(std::is_same<FirstType, T>::value){
			if(index == 0)
				pFirstType = reinterpret_cast<FirstType*>(value);
			else 
				tailTuple.Set_Unsafe<T, index - 1>(value);
		}
		else
			tailTuple.Set_Unsafe<T, index>(value);
	}

	template<typename T, size_t index=0>
	void Set(T* value)
	{
		static_assert(contains_same<T, FirstType, Tail...>::value,"This Tuple does not contain a variable of type T");
		Set_Unsafe<T, index>(value);
	}
	
	template <typename... Types, typename _Fn>
	void ForEach(_Fn _Func)
	{
		_Func(pFirstType);
		tailTuple.ForEach<_Fn, Types...>(_Func);
	}

private:
	FirstType* pFirstType;
	Tuple<Tail...> tailTuple;
};

template<typename MyType>
class Tuple<MyType>{
public:
	Tuple(void) : pData(nullptr) { }

	~Tuple(void)
	{
		delete pData;
	}
	
	template<typename T, size_t index>
	//DON'T CALL THIS
	MyType* Get_Unsafe(void)
	{
		return pData;
	}

	template<typename T, size_t index=0>
	MyType* Get(void)
	{
		return pData;
	}
		
	template<typename T, size_t index>
	//DON'T CALL THIS
	void Set_Unsafe(T* value)
	{
		pData = reinterpret_cast<MyType*>(value);
	}

	template<typename T, size_t index=0>
	void Set(T* value)
	{
		pData = reinterpret_cast<MyType*>(value);
	}
	
	template <typename... Types, typename _Fn>
	void ForEach(_Fn _Func)
	{
		_Func(pData);
	}

private:
	MyType* pData;
};

template<typename AllowedType>
class MemberMapper{
public:	
	template <typename... Types>
	void Allocate ()
	{
		Implementation<Types...>::Allocate();
	}
	
	template <typename... Types>
	void Allocate (Types*... initData)
	{
		Implementation<Types...>::Allocate(initData...);
	}
	
	template <typename... Types>
	void Release ()
	{
		Implementation<Types...>::Release();
	}
	
	template <typename T>
	T* GetMember(void)
	{
		static_assert(is_base_of<AllowedType,T>::value,"T* GetMember<T>(void) ==> Invalid T");
		return Implementation<T>::pComponents[0];
	}
	
	template <typename T, typename AllocType, typename... AllocTypeTail>
	T* GetMember(void)
	{
		static_assert(contains_same<T, AllocType, AllocTypeTail...>::value,"T* GetMember<T,AllocatedTypes...>(void) ==> Invalid T");
		return Implementation<T>::pComponents[0];
	}
	
	template <typename T, typename _Pr>
	T* GetMember(_Pr _Pred)
	{
		static_assert(is_base_of<AllowedType,T>::value,"T* MemberMapper<AllowedType>::GetMember<T, _Pr>(_Pr _Pred) ==> T does not inherit from AllowedType");

		auto itComp = find_if(Implementation<T>::pComponents.begin(), Implementation<T>::pComponents.end(), _Pred);
		if(itComp == Implementation<T>::pComponents.end())
			throw MemberNotFoundException();

		return *itComp;
	}
	
	template <typename T, unsigned int index, typename... AllocatedTypes>
	void SetMember(T* ptr)
	{
		static_assert(is_base_of<AllowedType,T>::value,"void MemberMapper<AllowedType>::SetMember<T, index, AllocatedType...>(T* ptr) ==> T does not inherit from AllowedType");
		static_assert(index < count_T<T, AllocatedTypes...>::value,"void MemberMapper<AllowedType>::SetMember<T, index, AllocatedType...>(T* ptr) ==> Invalid index");
		
		Implementation<T>::pComponents[index] = ptr;
	}

	template <typename T, typename... AllocType>
	void AddMember(T* ptr)
	{
		static_assert(contains_base_of<T, AllocType...>::value,"void MemberMapper<AllowedType>::AddMember<T, AllocatedType...>(T* ptr) ==> T does not inherit from any AllocatedType");
		Implementation<T>::pComponents.push_back(ptr);
	}
	
	template <typename... Types,typename Func>
	void ForEach(Func _Func)
	{
		Implementation<Types...>::Call(_Func);
	}

private:
	template <typename... T>
	class Implementation;

	template <typename T>
	struct Implementation <T>
	{
		static std::vector<T*> pComponents;
		
		static void Allocate()
		{
			static_assert(std::is_base_of<AllowedType, T>::value,"MemberMapper<AllowedType>::Allocate<T>(void) ==> T does not inherit from AllowedType");
			//pComponents.push_back(nullptr);
		}
		
		static void Allocate(T* initData)
		{
			static_assert(is_base_of<AllowedType, T>::value,"MemberMapper<AllowedType>::Allocate<T>(void) ==> T does not inherit from AllowedType");
			pComponents.push_back(initData);
		}
		
		static void Release()
		{
			static_assert(std::is_base_of<AllowedType, T>::value,"MemberMapper<AllowedType>::Allocate<T>(void) ==> T does not inherit from AllowedType");
			for(unsigned int i=0; i < pComponents.size(); ++i)
				delete pComponents[i];
		}
		
		template<typename _Pr>
		static void Call(_Pr _Pred)
		{
			for_each(pComponents.begin(), pComponents.end(), _Pred);
		}
	};

	template <typename H, typename... T>
	struct Implementation <H, T...>
	{
		static void Allocate() {
			Implementation<H>::Allocate();
			Implementation<T...>::Allocate();
		}
		
		static void Allocate(H* initData, T*... initDataTail) {
			Implementation<H>::Allocate(initData);
			Implementation<T...>::Allocate(initDataTail...);
		}
		
		static void Release() {
			Implementation<H>::Release();
			Implementation<T...>::Release();
		}
		
		template<typename Func>
		static void Call(Func _Func)
		{
			Implementation<H>::Call(_Func);
			Implementation<T...>::Call(_Func);
		}
	};
};

template<typename AllowedType>
template<typename T>
std::vector<T*> MemberMapper<AllowedType>::Implementation<T>::pComponents = std::vector<T*>();


template <typename T, typename... TypesToCheck>
struct contains_same
{
private:
	template <typename... TypeToCheck>
	class impl;

	template <>
	struct impl<>
	{
		static const bool value = false;
	};

	template <typename TypeToCheck, typename... Tail>
	struct impl<TypeToCheck, Tail...>
	{
		static const bool value = is_same<T, TypeToCheck>::value || impl<Tail...>::value;
	};

public:
	static const bool value = impl<TypesToCheck...>::value;
};

template <typename T, typename... TypesToCheck>
struct count_T
{
private:		
	template <typename... TypeToCheck>
	class impl;

	template <>
	struct impl<>
	{
		static const unsigned int value = 0;
	};

	template <typename TypeToCheck, typename... Tail>
	struct impl<TypeToCheck, Tail...>
	{
		static const unsigned int value = is_same<T, TypeToCheck>::value ? 1 : 0 + impl<Tail...>::value;
	};

public:
	static const unsigned int value = impl<TypesToCheck...>::value;
};

template <typename T, typename... TypesToCheck>
struct contains_base_of
{
private:
	template <typename... TypeToCheck>
	class impl;

	template <>
	struct impl<>
	{
		static const bool value = false;
	};

	template <typename TypeToCheck, typename... Tail>
	struct impl<TypeToCheck, Tail...>
	{
		static const bool value = is_base_of<TypeToCheck, T>::value || impl<Tail...>::value;
	};

public:
	static const bool value = impl<TypesToCheck...>::value;
};
