#pragma once

#include "../Material.h"
#include "../../Helpers/resrc_ptr.hpp"

class SkinnedMaterial : public Material
{
public:
	SkinnedMaterial(void);
	virtual ~SkinnedMaterial();
	
	virtual void UpdateEffectVariables(const tt::GameContext& context) override;
		
	void SetDiffuse(const std::tstring& diffuseFilename);
	void SetLightDirection(const tt::Vector3& lightDirection);
	void SetBoneTransforms(const std::vector<D3DXMATRIX>& boneTransforms);
	void SetDualQuats(const std::vector<tt::DualQuaternion>& boneTransforms);
	
	static const int MAX_NR_OF_BONES = 70;
private:
	tt::Vector3 m_vecLightDirection;

	resource_ptr<ID3D10ShaderResourceView> m_pTexture;

	std::vector<D3DXMATRIX> m_vecBoneTransforms;

	std::vector<tt::DualQuaternion> m_vecDualQuats;
	
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkinnedMaterial(const SkinnedMaterial& yRef);									
	SkinnedMaterial& operator=(const SkinnedMaterial& yRef);
};