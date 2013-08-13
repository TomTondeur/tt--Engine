#pragma once

#include "../Helpers/stdafx.h"
#include "../Helpers/D3DUtil.h"
#include "../Helpers/Namespace.h"
#include "../Helpers/resrc_ptr.hpp"

struct Sprite;
struct SpriteVertex;
class SpriteMaterial;

class SpriteBatch
{
public:
	//Default constructor & destructor
	SpriteBatch(void);
	virtual ~SpriteBatch(void);

	//Methods
	void Initialize(void);
	void Draw(const Sprite& sprite);
	void Flush(const tt::GameContext& context);

	static const unsigned int sc_MaxNrOfSprites = 50;

private:
	//Datamembers
	std::map<ID3D10ShaderResourceView*, std::vector<SpriteVertex> > m_Sprites;
	resource_ptr<SpriteMaterial> m_pMaterial;
	
	ID3D10Buffer* m_pVertexBuffer;
	unsigned int m_VertexBufferStride;

	unsigned int m_NrOfSprites;

	//Disabling default copy constructor & assignment operator
	SpriteBatch(const SpriteBatch& src);
	SpriteBatch& operator=(const SpriteBatch& src);
};
