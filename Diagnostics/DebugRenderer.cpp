#include "DebugRenderer.h"

#include "../Services/ServiceLocator.h"
#include "../Graphics/Materials/DebugMaterial.h"
#include "../Graphics/GraphicsDevice.h"
#include "../Graphics/EffectTechnique.h"

struct DebugVertex{
	DebugVertex(const tt::Vector3& _pos, const tt::Vector4& _color) : pos(static_cast<D3DXVECTOR3>(_pos)), color(static_cast<D3DXCOLOR>(_color)){}

	D3DXVECTOR3 pos;
	D3DXCOLOR color;
};

const unsigned int DebugRenderer::sc_MaxNrOfVertices = 1000;

DebugRenderer::DebugRenderer(void):m_pVertexBuffer(nullptr),m_pMaterial(nullptr){}

DebugRenderer::~DebugRenderer(void)
{
	delete m_pMaterial;
	if(m_pVertexBuffer)
		m_pVertexBuffer->Release();
}

//Methods

void DebugRenderer::Initialize(void)
{
	m_pMaterial = new DebugMaterial();
	m_pMaterial->LoadEffect();
	
	BuildVertexBuffer();
}

void DebugRenderer::DrawLine(const tt::Vector3& startPt, const tt::Vector3& endPt, const tt::Vector4& color)
{
	m_Lines.push_back(DebugVertex(startPt, color));
	m_Lines.push_back(DebugVertex(endPt, color));
}

void DebugRenderer::Flush(const tt::GameContext& context)
{
	if(m_Lines.empty())
		return;

	auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();

	DebugVertex* pVertices = nullptr;
	m_pVertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, NULL, reinterpret_cast<void**>(&pVertices));

	for(unsigned int i=0; i < m_Lines.size(); ++i)
		pVertices[i] = m_Lines[i];

	m_pVertexBuffer->Unmap();

	pD3DDevice->IASetInputLayout(m_pMaterial->GetInputLayout()->pInputLayout);
	pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	unsigned int stride = sizeof(DebugVertex), offset = 0;
	pD3DDevice->IASetVertexBuffers(0, 1, &m_pVertexBuffer,&stride, &offset); 
	
	m_pMaterial->Update(context, tt::Matrix4x4::Identity);

	D3D10_TECHNIQUE_DESC techDesc;
	auto pTech = m_pMaterial->GetActiveTechnique();
	pTech->GetDesc(&techDesc);
	for(unsigned int i=0; i < techDesc.Passes; ++i){
		pTech->GetPassByIndex(i)->Apply(0);
		pD3DDevice->Draw(m_Lines.size(), 0);
	}

	m_Lines.clear();
}

void DebugRenderer::BuildVertexBuffer(void)
{
	D3D10_BUFFER_DESC desc;
	desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(DebugVertex) * sc_MaxNrOfVertices;
	desc.Usage = D3D10_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	desc.MiscFlags = NULL;

	HR( MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice()->CreateBuffer(&desc, nullptr, &m_pVertexBuffer) );
}
