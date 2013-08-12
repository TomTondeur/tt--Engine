//    __  __      ______            _          
//   / /_/ /__ _ / ____/___  ____ _(_)___  ___ 
//  / __/ __(_|_) __/ / __ \/ __ `/ / __ \/ _ \
// / /_/ /__ _ / /___/ / / / /_/ / / / / /  __/
// \__/\__(_|_)_____/_/ /_/\__, /_/_/ /_/\___/ 
//                        /____/               
//
// ServiceLocator.cpp - Copyright © 2013 Tom Tondeur
//

#include "ServiceLocator.h"
#include "Implementations/DefaultGraphicsService.h"
#include "Implementations/DefaultInputService.h"

std::unique_ptr<MyServiceLocator> MyServiceLocator::s_pInstance = nullptr;


MyServiceLocator::MyServiceLocator(void)
{
	AddService<IGraphicsService>(new DefaultGraphicsService());
	AddService<IInputService>(new DefaultInputService());
	AddService<ResourceService>(new ResourceService());
	AddService<DebugService>(new DebugService());
}

MyServiceLocator::~MyServiceLocator(void)
{

}

MyServiceLocator* MyServiceLocator::GetInstance(void)
{
	if(s_pInstance.get() == nullptr)
		s_pInstance = std::unique_ptr<MyServiceLocator>(new MyServiceLocator());
	return s_pInstance.get();
}
