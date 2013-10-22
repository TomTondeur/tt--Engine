#include "TerrainMaterial.h"
#include "../../Services/ServiceLocator.h"
#include "../GraphicsDevice.h"
#include "../EffectTechnique.h"

#define MAX_VERTICES 6 * static_cast<int>(pow(4,m_Tessellation))

class TerrainMaterial::Implementation final
{
public:
	Implementation(unsigned char tessellation, const std::tstring& heightMapPath): 
				m_pInitVB(nullptr), m_pIndexBuffer(nullptr), 
				m_pStreamOutVB(nullptr), m_pStreamOutSwapVB(nullptr), 
				m_pFinalVB(nullptr),
				m_Tessellation(tessellation),
				m_HeightMapFilename(heightMapPath)
	{
		ASSERT(tessellation <= m_Tessellation, (_T("Cannot create a terrain with tessellation ") + to_tstring(tessellation) 
												+ _T(".\nMax tessellation is ") + to_tstring(m_Tessellation)).c_str());
	}

	~Implementation(void)
	{
		m_pFinalVB->Release();
	}

	void BuildVertexBuffers(void)
	{
		//Init VB
		D3DXVECTOR3 initVerts[] = {D3DXVECTOR3(0,0,0), D3DXVECTOR3(1,0,0), D3DXVECTOR3(0,0,1), D3DXVECTOR3(1,0,1)};

		D3D10_BUFFER_DESC desc;
		desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		desc.ByteWidth = sizeof(D3DXVECTOR3)*4;
		desc.CPUAccessFlags = NULL;
		desc.MiscFlags = NULL;
		desc.Usage = D3D10_USAGE_DEFAULT;

		D3D10_SUBRESOURCE_DATA initData;
		initData.pSysMem = initVerts;
		
		auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();

		HR(pD3DDevice->CreateBuffer(&desc, &initData, &m_pInitVB));

		//Streamout VBs

		desc.BindFlags |= D3D10_BIND_STREAM_OUTPUT;
		desc.ByteWidth = MAX_VERTICES * sizeof(D3DXVECTOR3); //Pos

		HR(pD3DDevice->CreateBuffer(&desc, nullptr, &m_pStreamOutVB));
		HR(pD3DDevice->CreateBuffer(&desc, nullptr, &m_pStreamOutSwapVB));

		//Final VB

		desc.ByteWidth = MAX_VERTICES * sizeof(D3DXVECTOR3) * 2; //Pos, Normal

		HR(pD3DDevice->CreateBuffer(&desc, nullptr, &m_pFinalVB));
		
		//Indexbuffer
		DWORD initIndices[] = {1,0,2,1,2,3};

		desc.BindFlags = D3D10_BIND_INDEX_BUFFER;
		desc.ByteWidth = sizeof(DWORD) * 6;
		initData.pSysMem = initIndices;
		
		HR(pD3DDevice->CreateBuffer(&desc, &initData, &m_pIndexBuffer));

	}
	
	//Calculate terrain on GPU
	void GenerateTerrain(TerrainMaterial* pThis)
	{
		auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();
		unsigned int stride = sizeof(D3DXVECTOR3);
		unsigned int offset = 0;
		D3D10_TECHNIQUE_DESC techDesc;

		//************
		//TESSELLATION
		//************

		//Configure shader
		pThis->SetActiveTechnique(_T("TechTessellateTerrain"));
		pThis->GetActiveTechnique()->GetDesc(&techDesc);

		//First SubD

		//Prepare IA stage for a first subdivision
		pD3DDevice->IASetInputLayout(pThis->GetInputLayout()->pInputLayout);
		pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pD3DDevice->IASetVertexBuffers(0, 1, &m_pInitVB, &stride, &offset);
		pD3DDevice->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
		//The updated vertices are streamed out to the target VB
		pD3DDevice->SOSetTargets(1, &m_pStreamOutVB, &offset);
		
		//Execute shader
		for(unsigned int p=0; p < techDesc.Passes; ++p){
			pThis->GetActiveTechnique()->GetPassByIndex(p)->Apply(0);
			pD3DDevice->DrawIndexed(6,0,0);
		}

		//Additional subD iterations

		ID3D10Buffer* nullBufferArr[1] = {0};
		for(unsigned char i=0; i < m_Tessellation-1; ++i){
			//StreamOutSwap (input VB) should contain the latest subdivision
			swap(m_pStreamOutSwapVB, m_pStreamOutVB);
			
			//Use StreamOutputSwap as input, SteamOutput is still bound as SO target
			pD3DDevice->SOSetTargets(1, nullBufferArr, &offset);
			pD3DDevice->IASetVertexBuffers(0, 1, &m_pStreamOutSwapVB, &stride, &offset);
			pD3DDevice->SOSetTargets(1, &m_pStreamOutVB, &offset);
		
			//Execute shader
			for(unsigned int p=0; p < techDesc.Passes; ++p){
				pThis->GetActiveTechnique()->GetPassByIndex(p)->Apply(0);
				pD3DDevice->DrawAuto();
			}
		}		

		//****************
		//SAMPLE HEIGHTMAP
		//****************
		m_pHeightMapSRV = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<ID3D10ShaderResourceView>(m_HeightMapFilename);

		pThis->SetActiveTechnique(_T("TechSampleHeightMap"));
		pThis->SetVariable(_T("HEIGHTMAP"), m_pHeightMapSRV.get());
		pD3DDevice->SOSetTargets(1, &m_pFinalVB, &offset);
		pD3DDevice->IASetVertexBuffers(0, 1, &m_pStreamOutVB, &stride, &offset);

		for(unsigned int p=0; p < techDesc.Passes; ++p){
			pThis->GetActiveTechnique()->GetPassByIndex(p)->Apply(0);
			pD3DDevice->DrawAuto();
		}

		//We're done streaming out -> unbind stream output buffer by binding null buffer
		pD3DDevice->SOSetTargets(1, nullBufferArr, &offset);
		
		//We don't need the temporary buffers anymore //UPDATE: Apparently we do
		m_pInitVB->Release();
		m_pIndexBuffer->Release();
		m_pStreamOutVB->Release();
		m_pStreamOutSwapVB->Release();
	}

	void DrawTerrain(TerrainMaterial* pThis, const tt::GameContext& context, const tt::Matrix4x4& worldMat)
	{
		auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();
		unsigned int offset=0;
		unsigned int stride = sizeof(D3DXVECTOR3)*2;
		D3D10_TECHNIQUE_DESC techDesc;
		
		//Configure shader
		pThis->SetActiveTechnique(_T("TechDraw"));
		pThis->Update(context, worldMat);

		//Configure IA
		pD3DDevice->IASetInputLayout(pThis->GetInputLayout()->pInputLayout);
		pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pD3DDevice->IASetVertexBuffers(0,1,&m_pFinalVB, &stride, &offset);

		//Execute shader
		pThis->GetActiveTechnique()->GetDesc(&techDesc);
		for(unsigned int p=0; p < techDesc.Passes; ++p)
		{
			pThis->GetActiveTechnique()->GetPassByIndex(p)->Apply(0);
			pD3DDevice->DrawAuto();
		}
	}

private:
	ID3D10Buffer* m_pInitVB, *m_pIndexBuffer;
	ID3D10Buffer* m_pStreamOutVB, *m_pStreamOutSwapVB;
	ID3D10Buffer* m_pFinalVB;
	resource_ptr<ID3D10ShaderResourceView> m_pHeightMapSRV;
	std::tstring m_HeightMapFilename;
	unsigned char m_Tessellation;
};

TerrainMaterial::TerrainMaterial(unsigned char tessellation, const std::tstring& heightMapPath):Material(_T("Resources/Effects/Terrain.fx")),m_pImpl(nullptr)
{
	m_pImpl = new Implementation(tessellation, heightMapPath);
	LoadEffect();
}

TerrainMaterial::~TerrainMaterial(void)
{
	delete m_pImpl;
}

//Methods

void TerrainMaterial::InitializeEffectVariables(void)
{
	m_pImpl->BuildVertexBuffers();
	m_pImpl->GenerateTerrain(this);
}

void TerrainMaterial::UpdateEffectVariables(const tt::GameContext& context)
{

}

void TerrainMaterial::DrawTerrain(const tt::GameContext& context, const tt::Matrix4x4& worldMat)
{
	m_pImpl->DrawTerrain(this, context, worldMat);
}
