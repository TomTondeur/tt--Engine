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
	//float4x4 matBones[70] : BoneTransforms;
	float2x4 g_dualquat[70] : DualQuats;
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

	// <NVIDIA>
	float2x4 dual = (float2x4)0;
	float2x4 m = g_dualquat[input.BlendIndices.x];
	float4 dq0 = (float1x4)m ;     
    
	dual = input.BlendWeight.x * m ;

	m = g_dualquat[input.BlendIndices.y];
	float4 dq = (float1x4)m ;   
	if (dot( dq0, dq ) < 0)        
		dual -= input.BlendWeight.y * m;
	else 
		dual += input.BlendWeight.y * m;
        
    m = g_dualquat[input.BlendIndices.z];
    dq = (float1x4)m ;          
    if (dot( dq0, dq ) < 0)        
		dual -= input.BlendWeight.z * m;
    else 
		dual += input.BlendWeight.z * m;
            
            
    m = g_dualquat[input.BlendIndices.w];
    dq = (float1x4)m ;              
    if (dot( dq0, dq ) < 0)        
		dual -= input.BlendWeight.w * m;
    else                
		dual += input.BlendWeight.w * m;    
   
    float4 Pos;
    float3 Norm, position, translation; 

    // fast dqs 
    float length = sqrt(dual[0].w * dual[0].w + dual[0].x * dual[0].x + dual[0].y * dual[0].y + dual[0].z * dual[0].z);
    dual = dual / length ; 
    position = input.Position.xyz + 2.0 * cross(dual[0].xyz, cross(dual[0].xyz, input.Position.xyz) + dual[0].w * input.Position.xyz);
    translation = 2.0 * (dual[0].w * dual[1].xyz - dual[1].w * dual[0].xyz + cross(dual[0].xyz, dual[1].xyz)); 
    position += translation; 
    
    Pos = float4(position,1);
    
	float3 t = 2 * cross(dual[0].xyz, input.Normal);
	Norm = input.Normal + dual[0].w * t + cross(dual[0].xyz, t); 

    float4 vAnimatedPos = Pos; 
    // </NVIDIA>

	output.Position = mul(vAnimatedPos, matWorldViewProj);
	output.Normal = mul(Norm, (float3x3)matWorld);
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
	
	retColor *= m_Texture.Sample(samLinear,texCoord).xyz;
	
	return retColor;
}

float4 PS(VS_OUTPUT input):SV_TARGET
{
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

//****************
//DEFERRED SHADING
//****************

struct PS_OUTPUT
{
	float4 Color : SV_TARGET0;
	float4 Normal : SV_TARGET1;
};

PS_OUTPUT PS_Deferred(VS_OUTPUT input)
{
	PS_OUTPUT mrtOut = (PS_OUTPUT)0;

	mrtOut.Color = float4(m_Texture.Sample(samLinear,input.TexCoord).xyz, 1);

	input.Normal = (normalize(input.Normal) + float3(1,1,1)) * .5f;
	mrtOut.Normal = float4(input.Normal,1);

	return mrtOut;
}

technique10 TechDeferred
{
	pass one
	{
		SetVertexShader( CompileShader( vs_4_0, VS_Anim() ) );
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader( ps_4_0, PS_Deferred() ) );
		SetRasterizerState(Solid);
		SetBlendState(NoBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	}
}
