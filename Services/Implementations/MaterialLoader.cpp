#include "../Interfaces/ResourceService.h"

#include "../../Graphics/Material.h"

template<> unique_ptr<Material> ResourceService::LoadResource<Material>(std::tstring filename)
{
	auto ptr = new Material(_T("c:/users/user/desktop/PosNormTex.fx"));
	ptr->LoadEffect();
	return unique_ptr<Material>(ptr);
}