#include "../Interfaces/ResourceService.h"

#include "../../Graphics/Materials/Object3DMaterial.h"

template<> unique_ptr<Object3DMaterial> ResourceService::LoadResource<Object3DMaterial>(std::tstring filename)
{
	auto ptr = new Object3DMaterial();
	ptr->LoadEffect();
	return unique_ptr<Object3DMaterial>(ptr);
}