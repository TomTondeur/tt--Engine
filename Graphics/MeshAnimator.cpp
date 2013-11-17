#include "MeshAnimator.h"
#include "Model3D.h"
#include "../Services/ServiceLocator.h"

MeshAnimator::MeshAnimator(void):m_pModel(nullptr)
{	
	m_CurrentClip.Name = _T("");
	m_CurrentClip.KeysPerSecond = 0;
}

MeshAnimator::~MeshAnimator(void)
{

}

// Determine which animation clip should be played.
bool MeshAnimator::SetAnimationClip(const std::tstring& name)
{
	auto it = find_if(m_pModel->m_AnimClips.begin(), m_pModel->m_AnimClips.end(), [&](AnimationClip thisClip){
		return thisClip.Name == name;
	});

	if(it == m_pModel->m_AnimClips.end())
		return false;
	else{
		m_CurrentClip = *it;
		return true;
	}
}

tt::Vector3 ToVec3(const tt::Quaternion& q)
{
	return tt::Vector3(q.x, q.y, q.z);
}

tt::Quaternion ExtractPos(const tt::DualQuaternion& dq)
{
	return tt::Quaternion::Inverse(dq.Data[0]) * (dq.Data[1]*2);
}

tt::DualQuaternion Combine(const tt::DualQuaternion& lhs, const tt::DualQuaternion& rhs)
{
	//Example: lhs = invBindpose, rhs = transform
	return tt::DualQuaternion(lhs.Data[0] * rhs.Data[0]*-1, ToVec3(tt::Quaternion::Inverse(rhs.Data[0]) * ExtractPos(lhs) * rhs.Data[0] + ExtractPos(rhs)));
}

//Calculate the bonetransforms
void MeshAnimator::Update(const tt::GameContext& context)
{
	float currentTick = context.GameTimer.GetTotalSeconds() * m_CurrentClip.KeysPerSecond;
	
	//Get remainder of currentTick and clipDuration
	float clipDuration = (m_CurrentClip.Keys.end()-1)->KeyTime - m_CurrentClip.Keys.begin()->KeyTime;
	float targetTick = currentTick - ((int)(currentTick/clipDuration)) * clipDuration;
	
	//Get animation tick after target tick
	auto itNextTick = find_if(m_CurrentClip.Keys.begin(), m_CurrentClip.Keys.end(), [=](const AnimationKey& thisKey){
		return thisKey.KeyTime > targetTick;
	});

	//Get animation tick before target tick
	std::vector<AnimationKey>::iterator itPrevTick;
	if(itNextTick == m_CurrentClip.Keys.end())
		MyServiceLocator::GetInstance()->GetService<DebugService>()->Log(_T("Failed to find next animation tick."), LogLevel::Error);
	else if(itNextTick == m_CurrentClip.Keys.begin())
		itPrevTick = m_CurrentClip.Keys.end()-1;
	else 
		itPrevTick = itNextTick - 1;

	//Lerp between transformations of previous and next animation tick
	float blendFactor = (targetTick - itPrevTick->KeyTime) / (itNextTick->KeyTime - itPrevTick->KeyTime);
	
	m_BoneTransforms.resize(itPrevTick->BoneTransforms.size());
	m_DualQuats.resize(m_BoneTransforms.size());

	for(UINT i = 0; i < itPrevTick->BoneTransforms.size(); ++i)
	{		
		tt::DualQuaternion bindposeInv = tt::DualQuaternion::Inverse(m_pModel->m_Skeleton[i].BindPose)*-1;
	
		auto trans1 = itPrevTick->BoneTransforms[i];
		auto trans2 = itNextTick->BoneTransforms[i];
	
		auto dlb = tt::DualQuaternion::DLB(trans1, trans2, blendFactor);
		
		m_DualQuats[i] = Combine(bindposeInv, dlb);
	}
		
}
#include "../Graphics/Materials/DebugMaterial.h"
//currently empty, can be used to visualize bone transforms later
void MeshAnimator::Draw(const tt::GameContext& context)
{/*
	auto pModel = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<Model3D>(_T("Resources/bone.bin"));
	auto pMat = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<DebugMaterial>(_T("BoneMat"));
	for(auto& bone : m_BoneTransforms)
		MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->Draw(pModel, tt::Matrix4x4(bone), pMat, context);*/
}

void MeshAnimator::SetModel(Model3D* pModel)
{ 
	m_pModel = pModel; 
}

const vector<D3DXMATRIX>& MeshAnimator::GetBoneTransforms(void) const 
{
	return m_BoneTransforms;
}

const vector<tt::DualQuaternion>& MeshAnimator::GetDualQuats(void) const
{
	return m_DualQuats;
}
