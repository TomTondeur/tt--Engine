//--------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____|
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|  
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___ 
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____|
//
// Overlord Engine v0.44
// Copyright Overlord Brecht Kets & Overlord Thomas Goussaert
// http://www.digitalartsandentertainment.com/
//--------------------------------------------------------------------------------------

cbuffer cbPerObject
{
	float4x4 matWorld : World;
	float4x4 matWorldViewProj : WorldViewProjection;
	float3 vLightDir : LightDirection;
	float4x4 matBones[70] : BoneTransforms;
};

Texture2D m_Texture : DiffuseTexture;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = NONE;
};

BlendState NoBlend
{
	BlendEnable[0] = FALSE;
};

struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
	float4 BlendIndices : BLENDINDICES;
	float4 BlendWeight : BLENDWEIGHTS;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};

VS_OUTPUT VS_Anim(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 position = float4(input.Position, 1);
	float4 transformedPosition = float4(0,0,0,1);
	float3 normal = input.Normal;
	float3 transformedNormal = float3(0,0,0);	

	for(int i=0; i < 4; ++i)
	{
		int boneIndex = (int)input.BlendIndices[i];

		if(boneIndex > -1){
			transformedPosition += input.BlendWeight[i] * mul(position, matBones[boneIndex]);
			transformedNormal += input.BlendWeight[i] * mul(normal, (float3x3)matBones[boneIndex]);
			transformedPosition.w = 1;
		}
	}

	output.Position = mul(transformedPosition, matWorldViewProj);
	output.Normal = mul(transformedNormal, (float3x3)matWorld);
	output.TexCoord = input.TexCoord;

	return output;
}

float3 CalculateDiffuse(float3 normal, float2 texCoord)
{
	float3 retColor = float3(1,1,1);
	float intensity = dot(-normal, vLightDir);
	
	intensity = (saturate(intensity) + 1) / 2;
	intensity = pow(intensity, 1.5); //apply lambert power
	retColor *= intensity;
	
	retColor *= m_Texture.Sample(samLinear,texCoord);
	
	return retColor;
}

float4 PS(VS_OUTPUT input):SV_TARGET
{
	// TODO: CALCULATE DIFFUSE
	input.Normal = normalize(input.Normal);
	
	float3 diffuse = CalculateDiffuse(input.Normal, input.TexCoord);

	return float4(diffuse,1);
}

technique10 SkinnedAnimationTechnique
{
	pass one
	{
		SetVertexShader( CompileShader ( vs_4_0, VS_Anim() ));
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader ( ps_4_0, PS() ));
		SetRasterizerState(Solid);
		SetBlendState(NoBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	}
}
