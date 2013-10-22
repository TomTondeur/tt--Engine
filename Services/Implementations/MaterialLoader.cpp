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

#include "../Interfaces/ResourceService.h"

#include "../../Graphics/Materials/SpriteMaterial.h"
#include "../../Graphics/Materials/PostProcessing/BlurMaterial.h"
#include "../../Graphics/Materials/TextMaterial.h"
#include "../../Graphics/Materials/ParticlesMaterial.h"
#include "../../Graphics/Materials/SkinnedMaterial.h"
#include "../../Graphics/Materials/DebugMaterial.h"

template<> unique_ptr<SpriteMaterial> ResourceService::LoadResource<SpriteMaterial>(const std::tstring& filename)
{
	auto ptr = new SpriteMaterial();
	ptr->LoadEffect();
	return unique_ptr<SpriteMaterial>(ptr);
}

template<> unique_ptr<BlurMaterial> ResourceService::LoadResource<BlurMaterial>(const std::tstring& filename)
{
	auto ptr = new BlurMaterial();
	ptr->LoadEffect();
	return unique_ptr<BlurMaterial>(ptr);
}

template<> unique_ptr<TextMaterial> ResourceService::LoadResource<TextMaterial>(const std::tstring& filename)
{
	auto ptr = new TextMaterial();
	ptr->LoadEffect();
	return unique_ptr<TextMaterial>(ptr);
}

template<> unique_ptr<ParticlesMaterial> ResourceService::LoadResource<ParticlesMaterial>(const std::tstring& filename)
{
	auto ptr = new ParticlesMaterial();
	ptr->LoadEffect();
	return unique_ptr<ParticlesMaterial>(ptr);
}

template<> unique_ptr<SkinnedMaterial> ResourceService::LoadResource<SkinnedMaterial>(const std::tstring& filename)
{
	auto ptr = new SkinnedMaterial();
	ptr->LoadEffect();
	return unique_ptr<SkinnedMaterial>(ptr);
}

template<> unique_ptr<DebugMaterial> ResourceService::LoadResource<DebugMaterial>(const std::tstring& filename)
{
	auto ptr = new DebugMaterial();
	ptr->LoadEffect();
	return unique_ptr<DebugMaterial>(ptr);
}
