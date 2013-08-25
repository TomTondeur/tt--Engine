#pragma once

#include "../Helpers/Namespace.h"
class Model3D;

struct Bone
{
	tstring Name;
	D3DXMATRIX BindPose;
};

struct AnimationKey
{
	float KeyTime;
	vector<D3DXMATRIX> BoneTransforms;
};

struct AnimationClip
{
	tstring Name;
	float KeysPerSecond;
	vector<AnimationKey> Keys;
};

class MeshAnimator final
{
public:
	MeshAnimator(void);
	~MeshAnimator(void);

	// Determine which animation clip should be played.
	bool SetAnimationClip(const std::tstring& name);
	
	//Calculate the bonetransforms
	void Update(const tt::GameContext& context);
	//currently empty, can be used to visualize bone transforms later
	void Draw(const tt::GameContext& context);

	// Set the mesh filter
	void SetModel(Model3D* pModel);

	// return the bone transforms
	const vector<D3DXMATRIX>& GetBoneTransforms(void) const;

private:
	static const int TICKS_PER_SECOND = 2800;

	Model3D* m_pModel;
	std::vector<D3DXMATRIX> m_BoneTransforms;
	AnimationClip m_CurrentClip;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MeshAnimator(const MeshAnimator& yRef);									
	MeshAnimator& operator=(const MeshAnimator& yRef);
};