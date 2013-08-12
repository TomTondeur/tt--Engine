// ________________ _____ _   _  ____ ___ _   _ _____ 
// |__   __|__   __| ____| \ | |/ ___|_ _| \ | | ____|
//    | |     | |  |  _| |  \| | |  _ | ||  \| |  _|  
//    | |     | |  | |___| |\  | |_| || || |\  | |___ 
//    |_|     |_|  |_____|_| \_|\____|___|_| \_|_____|

// Property.h : File containing tt::Property implementation
//
// Properties provide a compact way to access private datamembers through accessor functions
// They work with a backing field that is not part of the struct itself, so the owner of the field can change it without the indirection of the accessors
//
// Initialization can be postponed and later done using the copy constructor, example:
//class Foo{
//public:
//	class Implementation
//	{
//	public:
//		Implementation():m_ObjectID(0),ObjectID([&](){return m_ObjectID;}) { }
//		~Implementation() { }
//		
//		tt::Property<int> p_ObjectID([&](){return m_ObjectID;});
//	private:
//		int m_ObjectID;
//	};
//
//	Foo(){
//		m_pImplementation = new Implementation();
//		p_ObjectID = m_pImplementation->p_ObjectID;
//	}
//
//	~Foo()
//	{
//		delete m_pImplementation;
//	}
//	
//	tt::Property<int> ObjectID;
//private:
//	Implementation* m_pImplementation;
//};
//

#pragma once
#include <functional>

namespace tt
{
	template<typename T, typename T_Owner>class Property 
	{
	public:
		static T_Owner::Property<T> Create(std::function<T(void)> _get)
		{
			return GetProperty<T>(_get);
		};
		
		static T_Owner::Property<T> Create(std::function<T(void)> _get, std::function<void(const T&)> _set)
		{
			return GetSetProperty<T>(_get, _set);
		};

	private:
		Property(void);// = delete;
		~Property(void){};
		Property(const Property& src);// = delete;
		Property& operator=(const Property& src);// = delete;
	};
	
	template<typename T, typename T_Owner> class T_Owner::GetProperty
	{		
	public:		
		//Automatically generate getter to given field
		explicit GetProperty(T& _field):get([&](){return _field;}) { }

		//Create empty property
		GetProperty(void):get(nullptr) { }

		//Pass accessor to the property
		GetProperty(std::function<T(void)> _get):get(_get) { }
		
		//Copy constructor, can be used to create pass-through properties
		GetProperty(const GetProperty& src)
		{
			get = src.get;
		}

		//Assignment operator, can be used to create pass-through properties
		GetProperty& operator=(const GetProperty& src)
		{
			get = src.get;
			return *this;
		}
		
		//Empty destructor
		~GetProperty(void) { }

		//Returns a copy to the backing field
		T Get(void) const
		{
			ASSERT(get!=nullptr,_T("[Error] Missing getter!"));
			return get();
		}

	private:
		std::function<T(void)> get;
	};

	template<typename T, typename T_Owner> class T_Owner::GetSetProperty
	{		
	public:		
		//Automatically generate accessors to given field
		explicit GetSetProperty(T& _field):get([&](){return _field;}), set([&](const T& value){_field = value;}) { }

		//Pass accessor functions to the property
		GetSetProperty(std::function<T(void)> _get, std::function<void(const T&)> _set):get(_get),set(_set) { }
		
		//Empty destructor
		~GetSetProperty(void) { }

		//Returns a copy to the backing field
		T Get(void) const
		{
			return get();
		}

		//Assigns a value to the backing field
		void Set(const T& src)
		{
			set(src);
		}

	private:
		std::function<T(void)> get;
		std::function<void(const T&)> set;
		T field;

		GetSetProperty(void);// = delete
		GetSetProperty(const Property& src);// = delete;
		GetSetProperty& operator=(const GetSetProperty& src);// = delete;
	};	
}
