#include "../Interfaces/ResourceService.h"

#include "../../Graphics/Materials/SpriteMaterial.h"

template<> unique_ptr<SpriteMaterial> ResourceService::LoadResource<SpriteMaterial>(std::tstring filename)
{
	auto ptr = new SpriteMaterial();
	ptr->LoadEffect();
	return unique_ptr<SpriteMaterial>(ptr);
}