#include "ParticlesMaterial.h"
#include "../../Services/ServiceLocator.h"
#include "../GraphicsDevice.h"
#include "../../Helpers/D3DUtil.h"

ParticlesMaterial::ParticlesMaterial(void):Material(_T("Resources/Particles/Particles.fx")),m_pTextureRandom(nullptr)
{}

ParticlesMaterial::~ParticlesMaterial(void)
{

}

//Methods

void ParticlesMaterial::InitializeEffectVariables(void)
{
	m_pTextureDiffuse = MyServiceLocator::GetInstance()->GetService<ResourceService>()->Load<ID3D10ShaderResourceView>(_T("Resources/Particles/DiffuseTexture.dds"));
	CreateRandomTexture();

	SetVariable(_T("GlobalTime"), 0.016f );
	SetVariable(_T("ElapsedTime"), 0.016f );
	SetVariable(_T("FrameGravity"), 0.016f * tt::Vector3(0,-9.81f, 0) );
	SetVariable(_T("DiffuseTexture"), m_pTextureDiffuse.get() );
	SetVariable(_T("RandomTexture"), m_pTextureRandom);
}

void ParticlesMaterial::UpdateEffectVariables(const tt::GameContext& context)
{
	SetVariable(_T("GlobalTime"), context.GameTimer.GetTotalSeconds() );
	SetVariable(_T("ElapsedTime"), context.GameTimer.GetElapsedSeconds() );
	SetVariable(_T("FrameGravity"), context.GameTimer.GetElapsedSeconds() * tt::Vector3(0,-9.81f, 0) );
	SetVariable(_T("DiffuseTexture"), m_pTextureDiffuse.get() );
	SetVariable(_T("RandomTexture"), m_pTextureRandom);
}

//--------------------------------------------------------------------------------------
// This helper function creates a 1D texture full of random vectors.  The shader uses
// the current time value to index into this texture to get a random vector.
//--------------------------------------------------------------------------------------
void ParticlesMaterial::CreateRandomTexture(void)
{
    int iNumRandValues = 1024;
    srand( timeGetTime() );
    //create the data
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = new float[iNumRandValues * 4];
    InitData.SysMemPitch = iNumRandValues * 4 * sizeof( float );
    InitData.SysMemSlicePitch = iNumRandValues * 4 * sizeof( float );

    for( int i = 0; i < iNumRandValues * 4; i++ )
        ( ( float* )InitData.pSysMem )[i] = float( ( rand() % 10000 ) - 5000 );

	ID3D10Texture1D* pRandomBuffer;

    // Create the texture
    D3D10_TEXTURE1D_DESC dstex;
    dstex.Width = iNumRandValues;
    dstex.MipLevels = 1;
    dstex.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    dstex.Usage = D3D10_USAGE_DEFAULT;
    dstex.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    dstex.CPUAccessFlags = 0;
    dstex.MiscFlags = 0;
    dstex.ArraySize = 1;
    HR(MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice()->CreateTexture1D( &dstex, &InitData, &pRandomBuffer) );
    delete[] InitData.pSysMem;

    // Create the resource view
    D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );
    SRVDesc.Format = dstex.Format;
    SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
    SRVDesc.Texture2D.MipLevels = dstex.MipLevels;
    HR(MyServiceLocator::GetInstance()->GetService<IGraphicsService>()->GetGraphicsDevice()->GetDevice()->CreateShaderResourceView( pRandomBuffer, &SRVDesc, &m_pTextureRandom ));
}
