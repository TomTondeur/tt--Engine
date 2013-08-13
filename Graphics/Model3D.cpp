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

#include "Model3D.h"
#include "../Helpers/BinaryReader.h"
#include "../Services/ServiceLocator.h"
#include "GraphicsDevice.h"
#include "Material.h"
#include "EffectTechnique.h"

VertexBufferInfo::VertexBufferInfo(void):pDataStart(nullptr),pVertexBuffer(nullptr){}

void VertexBufferInfo::Release(void)
{
	free(pDataStart);

	if(pVertexBuffer)
		pVertexBuffer->Release();
}

Model3D::Model3D(void):m_pIndexBuffer(nullptr)
{

}

Model3D::~Model3D(void)
{
/*	//Why God? Why?
	m_Positions.clear();
	m_Normals.clear();
	m_Tangents.clear();
	m_Binormals.clear();
	m_Colors.clear();
	m_TexCoords.clear();
	m_BlendWeights.clear();
	m_BlendIndices.clear();
	m_Indices.clear();
*/
	for(auto& vertData : m_vecVertBufferInfo)
		vertData.Release();
}

//Methods

void Model3D::BuildVertexBuffer(resource_ptr<Material> pMaterial)
{
	auto pMaterialInputLayout = pMaterial->GetInputLayout();

	//Check if we already have a vertex buffer for the current input layout
	for(auto& vertBufferInfo : m_vecVertBufferInfo)
		if(vertBufferInfo.pInputLayout == pMaterialInputLayout)
			return;

	unsigned int vbStride = 0;

	//Check if we have at least the data MaterialIL needs
	for(auto& ilDesc : pMaterialInputLayout->InputLayoutDesc){
		switch(ilDesc.Semantic){
			case InputLayoutSemantic::Position:
				if(m_Positions.data.empty())
					throw exception();
				else
					vbStride += sizeof(D3DXVECTOR3);
				break;
			case InputLayoutSemantic::TexCoord:
				if(m_TexCoords[ilDesc.SemanticIndex].data.empty())
					throw exception();
				else
					vbStride += sizeof(D3DXVECTOR2);
				break;
			case InputLayoutSemantic::Normal:
				if(m_Normals.data.empty())
					throw exception();
				else
					vbStride += sizeof(D3DXVECTOR3);
				break;
			case InputLayoutSemantic::Tangent:
				if(m_Tangents.data.empty())
					throw exception();
				else
					vbStride += sizeof(D3DXVECTOR3);
				break;
			case InputLayoutSemantic::Binormal:
				if(m_Binormals.data.empty())
					throw exception();
				else
					vbStride += sizeof(D3DXVECTOR3);
				break;
			case InputLayoutSemantic::Color:
				if(m_Colors.data.empty())
					throw exception();
				else
					vbStride += sizeof(D3DXCOLOR);
				break;
		}
	}
	
	//Prepare VertBufferInfo
	VertexBufferInfo vbInfo;
	vbInfo.pInputLayout = pMaterialInputLayout;
	vbInfo.VertexStride = vbStride;
	vbInfo.BufferSize = vbStride * m_Positions.indices.size();

	//Allocate memory for actual buffer
	void* pDataLocation = malloc(vbInfo.BufferSize);	
	vbInfo.pDataStart = pDataLocation;

	//Write vertex data to buffer
	for(unsigned int i=0; i < m_Positions.indices.size(); ++i){
		for(auto& ilDesc : pMaterialInputLayout->InputLayoutDesc){
			switch(ilDesc.Semantic){
				case InputLayoutSemantic::Position:
					memcpy(pDataLocation, &m_Positions.GetRefAt(i), sizeof(D3DXVECTOR3));
					pDataLocation = static_cast<D3DXVECTOR3*>(pDataLocation) + 1;
					break;
				case InputLayoutSemantic::TexCoord:
					memcpy(pDataLocation, &m_TexCoords[ilDesc.SemanticIndex].GetRefAt(i), sizeof(D3DXVECTOR2));
					pDataLocation = static_cast<D3DXVECTOR2*>(pDataLocation) + 1;
					break;
				case InputLayoutSemantic::Normal:
					memcpy(pDataLocation, &m_Normals.GetRefAt(i), sizeof(D3DXVECTOR3));
					pDataLocation = static_cast<D3DXVECTOR3*>(pDataLocation) + 1;
					break;
				case InputLayoutSemantic::Tangent:
					memcpy(pDataLocation, &m_Tangents.GetRefAt(i), sizeof(D3DXVECTOR3));
					pDataLocation = static_cast<D3DXVECTOR3*>(pDataLocation) + 1;
					break;
				case InputLayoutSemantic::Binormal:
					memcpy(pDataLocation, &m_Binormals.GetRefAt(i), sizeof(D3DXVECTOR3));
					pDataLocation = static_cast<D3DXVECTOR3*>(pDataLocation) + 1;
					break;
				case InputLayoutSemantic::Color:
					memcpy(pDataLocation, &m_Colors.GetRefAt(i), sizeof(D3DXCOLOR));
					pDataLocation = static_cast<D3DXCOLOR*>(pDataLocation) + 1;
					break;
			}
		}
	}
	
	// Fill a D3D10 buffer description
	D3D10_BUFFER_DESC bd = {};
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = vbInfo.BufferSize;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA initData;
	initData.pSysMem = vbInfo.pDataStart;

	// Create a ID3D10Buffer containing the vertex info
	auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();
	HR(pD3DDevice->CreateBuffer( &bd, &initData, &vbInfo.pVertexBuffer));

	//Add the vertex buffer info to the array
	m_vecVertBufferInfo.push_back(vbInfo);
}

void Model3D::BuildIndexBuffer(bool force)
{
	if(m_pIndexBuffer!=nullptr)
	{
		if(!force) //Exit if indexbuffer already exists
			return;
		else //Unless if forced
			m_pIndexBuffer->Release();
	}

	//Create index buffer descriptor
	D3D10_BUFFER_DESC bd = {};
    bd.Usage = D3D10_USAGE_IMMUTABLE;
    bd.ByteWidth = sizeof(int) * m_Indices.size();
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    
	//Provide pointer to index data
	D3D10_SUBRESOURCE_DATA initData;
    initData.pSysMem = m_Indices.data();
	
	//Create buffer
	auto pD3DDevice = MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice();
	HR(pD3DDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer));
}

const VertexBufferInfo& Model3D::GetVertexBufferInfo(resource_ptr<Material> pMaterial)
{
	auto it = find_if(m_vecVertBufferInfo.begin(), m_vecVertBufferInfo.end(), [&](const VertexBufferInfo& vertData)
					{
						return vertData.pInputLayout == pMaterial->GetInputLayout();
					});

	if(it != m_vecVertBufferInfo.end())
		return *it;

	BuildVertexBuffer(pMaterial);
	return m_vecVertBufferInfo.back();
}

ID3D10Buffer* Model3D::GetIndexBuffer(void)
{
	BuildIndexBuffer();

	return m_pIndexBuffer;
}

unsigned int Model3D::GetNrOfIndices(void) const
{
	return m_Indices.size();
}
