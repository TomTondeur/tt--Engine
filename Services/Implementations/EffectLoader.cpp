#include "../Interfaces/ResourceService.h"

#include "../ServiceLocator.h"
#include "../../Graphics/GraphicsDevice.h"

template<> unique_ptr<ID3D10Effect> ResourceService::LoadResource<ID3D10Effect>(std::tstring filePath)
{
	ID3D10Blob* pErrorBlob;
	ID3D10Effect* pEffect;
	
	HRESULT hr = D3DX10CreateEffectFromFile(filePath.c_str(),
				 NULL,
				 NULL,
				 "fx_4_0",
				 D3D10_SHADER_DEBUG|D3D10_SHADER_SKIP_OPTIMIZATION|D3D10_SHADER_WARNINGS_ARE_ERRORS|D3D10_SHADER_PACK_MATRIX_ROW_MAJOR, //HLSL Flags: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172416%28v=vs.85%29.aspx
				 0,
				 MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice(),
				 NULL,
				 NULL,
				 &pEffect,
				 &pErrorBlob,
				 NULL);

	if(FAILED(hr))
	{
		if(pErrorBlob!=nullptr)
		{
			char *errors = (char*)pErrorBlob->GetBufferPointer();
 
			tstringstream ss;
			for(unsigned int i = 0; i < pErrorBlob->GetBufferSize(); i++)
				ss<<errors[i];
 
			OutputDebugString(ss.str().c_str());
			pErrorBlob->Release();
		}

		throw LoaderException(_T("effect ") + filePath);
	}

	return unique_ptr<ID3D10Effect>(pEffect);
}
