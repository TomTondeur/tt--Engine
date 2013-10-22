#include "Terrain.h"

#include "../Graphics/Materials/TerrainMaterial.h"
#include "../Services/ServiceLocator.h"

Terrain::Terrain(const tt::Vector3& dimensions, unsigned int tessellation, const std::tstring& heightMapPath):
			m_Dimensions(dimensions), m_Tessellation(tessellation),m_pMaterial(nullptr), m_HeightMapPath(heightMapPath)
{}

Terrain::~Terrain(void)
{
	delete m_pMaterial;
}

//Methods

void Terrain::Initialize(void)
{
	SetComponent<TransformComponent>(new TransformComponent());

	m_pMaterial = new TerrainMaterial(m_Tessellation, m_HeightMapPath);
	m_pMaterial->InitializeEffectVariables();
}

void Terrain::Draw(const tt::GameContext& context)
{
	GetComponent<TransformComponent>()->Scale(m_Dimensions);
	m_pMaterial->DrawTerrain(context, GetComponent<TransformComponent>()->GetWorldMatrix());
}