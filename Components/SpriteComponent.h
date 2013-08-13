#pragma once

#include "../Helpers/Namespace.h"
#include "../Helpers/resrc_ptr.hpp"
#include "../Scenegraph/ObjectComponent.h"

class TransformComponent;

struct Sprite
{
	Sprite(const tt::Matrix4x4& transform);

	resource_ptr<ID3D10ShaderResourceView> pTexture;
	tt::Vector4 Color;
	const tt::Matrix4x4& Transform;
};

class SpriteComponent : public ObjectComponent
{
public:
	//Default constructor & destructor
	SpriteComponent(std::tstring filename, const TransformComponent* pTransform);
	virtual ~SpriteComponent(void);

	//Methods
	virtual void Initialize(void) override;
	virtual void Draw(const tt::GameContext& context) override;

private:
	//Datamembers
	std::tstring m_TextureFilename;
	Sprite m_Sprite;
		
	//Disabling default copy constructor & assignment operator
	SpriteComponent(const SpriteComponent& src);
	SpriteComponent& operator=(const SpriteComponent& src);
};
