#pragma once

#include "../Helpers/D3DUtil.h"
#include "../Helpers/resrc_ptr.hpp"

class Material;
struct InputLayout;

struct VertexBufferInfo
{
	void* pDataStart;
	ID3D10Buffer* pVertexBuffer;
	UINT BufferSize;
	UINT VertexStride;
	InputLayout* pInputLayout;

	VertexBufferInfo(void);

	void Release(void);
};

class Model3D
{
	friend class ResourceService;

	template <typename T>
	struct VertexAttribute
	{
		vector<T> data;
		vector<unsigned int> indices;

		T GetCopyAt(unsigned int vertexIndex) const
		{
			return data.at(indices.at(vertexIndex));
		}
	
		const T& GetRefAt(unsigned int vertexIndex) const
		{
			return data.at(indices.at(vertexIndex));
		}
	};

public:
	//Default constructor & destructor
	Model3D(void);
	virtual ~Model3D(void);

	//Methods
	void BuildVertexBuffer(resource_ptr<Material> pMaterial);
	void BuildIndexBuffer(bool force = false);
	
	const VertexBufferInfo& GetVertexBufferInfo(resource_ptr<Material> pMaterial);
	ID3D10Buffer* GetIndexBuffer(void);
	unsigned int GetNrOfIndices(void) const;

private:
	//Datamembers
	vector<VertexBufferInfo> m_vecVertBufferInfo; //We need a different vertex buffer for each different input layout
			
	VertexAttribute<D3DXVECTOR3> m_Positions;
	vector<VertexAttribute<D3DXVECTOR2>> m_TexCoords;
	VertexAttribute<D3DXVECTOR3> m_Normals;
	VertexAttribute<D3DXVECTOR3> m_Tangents;
	VertexAttribute<D3DXVECTOR3> m_Binormals;
	VertexAttribute<D3DXCOLOR>	m_Colors;
	VertexAttribute<D3DXVECTOR4> m_BlendWeights;
	VertexAttribute<D3DXVECTOR4> m_BlendIndices;
	
	vector<unsigned int> m_Indices;
	
	ID3D10Buffer* m_pIndexBuffer;

	//Disabling default copy constructor & assignment operator
	Model3D(const Model3D& src);
	Model3D& operator=(const Model3D& src);
};
