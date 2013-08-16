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

#pragma once

#include "../Helpers/D3DUtil.h"
#include "../Helpers/resrc_ptr.hpp"
#include "../Helpers/Namespace.h"

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

struct Ray
{
	Ray(const tt::Vector3& origin, const tt::Vector3& direction);
	
	tt::Vector3 Origin;
	tt::Vector3 Direction;
	tt::Vector3 InvDirection;
	int sign[3];
};

struct AABBox
{
	AABBox(void);
	~AABBox(void);

	void Initialize(const vector<D3DXVECTOR3>& vertices);
	void GetVertices(tt::Vector3* targetArr) const;
	bool Intersect(const Ray& r, float t0, float t1) const;

	tt::Vector3 Bounds[2];
	
	static const Ray FrustumCullRay0;
	static const Ray FrustumCullRay1;
	static const Ray FrustumCullRay2;
	static const Ray FrustumCullRay3;
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
	const AABBox& GetAABB(void) const;

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

	AABBox m_BoundingBox;

	//Disabling default copy constructor & assignment operator
	Model3D(const Model3D& src);
	Model3D& operator=(const Model3D& src);
};
