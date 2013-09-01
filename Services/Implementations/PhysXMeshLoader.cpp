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
#include "../../Helpers/PhysxUserStream.h"

template<> unique_ptr<NxConvexMesh> ResourceService::LoadResource<NxConvexMesh>(const std::tstring& filePath)
{
	if(std::tifstream(filePath))
		return unique_ptr<NxConvexMesh>( NxGetPhysicsSDK()->createConvexMesh( UserStream( std::string( filePath.begin(), filePath.end() ),true) ) );
	else
		throw LoaderException(std::tstring(_T("convex mesh ")) + filePath);
}

template<> unique_ptr<NxTriangleMesh> ResourceService::LoadResource<NxTriangleMesh>(const std::tstring& filePath)
{
	if(std::tifstream(filePath))
		return unique_ptr<NxTriangleMesh>( NxGetPhysicsSDK()->createTriangleMesh( UserStream( std::string( filePath.begin(),filePath.end() ),true) ) );
	else
		throw LoaderException(std::tstring(_T("triangle mesh ")) + filePath);
}