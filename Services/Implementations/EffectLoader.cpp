// Copyright � 2013 Tom Tondeur
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

#include "../ServiceLocator.h"
#include "../../Graphics/GraphicsDevice.h"

template<> unique_ptr<ID3D10Effect> ResourceService::LoadResource<ID3D10Effect>(const std::tstring& filePath)
{
	ID3D10Blob* pErrorBlob;
	ID3D10Effect* pEffect;
	
	HRESULT hr = D3DX10CreateEffectFromFile(filePath.c_str(),
				 NULL,
				 NULL,
				 "fx_4_0",
#ifndef NDEBUG
				 D3D10_SHADER_DEBUG|D3D10_SHADER_SKIP_OPTIMIZATION|D3D10_SHADER_WARNINGS_ARE_ERRORS|D3D10_SHADER_PACK_MATRIX_ROW_MAJOR, //HLSL Flags: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172416%28v=vs.85%29.aspx
#else
				 D3D10_SHADER_OPTIMIZATION_LEVEL3|D3D10_SHADER_IEEE_STRICTNESS|D3D10_SHADER_PACK_MATRIX_ROW_MAJOR,
#endif
				 0,
				 MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice(),
				 NULL,
				 NULL,
				 &pEffect,
				 &pErrorBlob,
				 NULL);

	if(FAILED(hr))
	{
		tstringstream ss;
		
		if(pErrorBlob!=nullptr)
		{
			char *errors = (char*)pErrorBlob->GetBufferPointer();
 
			for(unsigned int i = 0; i < pErrorBlob->GetBufferSize(); i++)
				ss<<errors[i];
 
			OutputDebugString(ss.str().c_str());
			pErrorBlob->Release();
			
			throw LoaderException(_T("effect ") + filePath, ss.str() );
		}
		MyServiceLocator::GetInstance()->GetService<DebugService>()->LogDirectXError(hr, __LINE__, __FILE__);
	}

	return unique_ptr<ID3D10Effect>(pEffect);
}
