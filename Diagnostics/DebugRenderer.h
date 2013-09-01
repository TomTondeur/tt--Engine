#pragma once

#include "../Helpers/Namespace.h"
#include "../Helpers/resrc_ptr.hpp"
class Material;
struct DebugVertex;

class DebugRenderer
{
public:
	//Default constructor & destructor
	DebugRenderer(void);
	virtual ~DebugRenderer(void);

	//Methods

	void Initialize(void);
	void DrawLine(const tt::Vector3& startPt, const tt::Vector3& endPt, const tt::Vector4& color);
	void Flush(const tt::GameContext& context);

	//constants
	static const unsigned int sc_MaxNrOfVertices;

private:
	//Datamembers
	Material* m_pMaterial;
	vector<DebugVertex> m_Lines;
	ID3D10Buffer* m_pVertexBuffer;

	//Private methods
	void BuildVertexBuffer(void);

	//Disabling default copy constructor & assignment operator
	DebugRenderer(const DebugRenderer& src);
	DebugRenderer& operator=(const DebugRenderer& src);
};
