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

static const float SHADOW_EPSILON = 0.04f;
static const float SMAP_WIDTH = 1280.0f;
static const float SMAP_HEIGHT = 720.0f;
static const float SMAP_DX = 1.0f / SMAP_WIDTH;
static const float SMAP_DY = 1.0f / SMAP_HEIGHT;

cbuffer cbPerObject
{
	float4x4 matWorld : World;
	float4x4 matWorldViewProj : WorldViewProjection;
	float3 vLightDir : LightDirection;
	//float4x4 matBones[70] : BoneTransforms;
	float2x4 g_dualquat[70] : DualQuats;
};

cbuffer cbShadowGen
{
	float4x4 matWorldLightViewProjection : LightViewProjection;
};
Texture2D texShadowMap : ShadowMapSRV;

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

SamplerState samShadow
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp;
    AddressV = Clamp;
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
	float4 ProjTexCoord : TEXCOORD1;
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
	output.ProjTexCoord = mul(vAnimatedPos, matWorldLightViewProjection);

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

float CalculateShadowFactor(float4 projTexC)
{
	// Step 1: Complete projection by doing division by w.
	projTexC.xyz /= projTexC.w;

    // Step 2: Points outside the light volume are in shadow. Return 0.0f if this is the case.
	if(projTexC.x < -1 || projTexC.x > 1 || projTexC.y < -1 || projTexC.y > 1)
		return 0.0f;
	
    // Step 3: Transform from NDC [-1,1] space to texture space [0,1].
	projTexC.x = .5f*(projTexC.x + 1);
	projTexC.y = -.5f*(projTexC.y - 1);

    // Step 4: Depth in NDC space (assign projTexC.z to a local variable called depth).
	float depth = projTexC.z; 

    // Step 5: Sample shadow map to get nearest depth to light. Sample 4 values, offsetting the texture coordinates with DX, DY, and DX DY.
	// Store in 4 different local float values (s0 to3)
	float s0 = texShadowMap.Sample(samShadow, projTexC.xy + float2(-SMAP_DX,-SMAP_DY)).r;
	float s1 = texShadowMap.Sample(samShadow, projTexC.xy + float2(-SMAP_DX, SMAP_DY)).r;
	float s2 = texShadowMap.Sample(samShadow, projTexC.xy + float2( SMAP_DX,-SMAP_DY)).r;
	float s3 = texShadowMap.Sample(samShadow, projTexC.xy + float2( SMAP_DX, SMAP_DY)).r;

    // Step 6:  Is the pixel depth <= shadow map value? (compare depth to the results of step 5 (do not forget the EPSILON)
	// Store in 4 local result variables (result0 to 3)
	float result0 = (depth <= (s0 + SHADOW_EPSILON)) * s0;
	float result1 = (depth <= (s1 + SHADOW_EPSILON)) * s1;
	float result2 = (depth <= (s2 + SHADOW_EPSILON)) * s2;
	float result3 = (depth <= (s3 + SHADOW_EPSILON)) * s3;

    // Step 7: Transform the Projected texture coordinates to texel space.
	float2 texelSpaceCoord = projTexC.xy;

    // Step 8: Determine the interpolation amounts (use frac()).
	// Store in variable t (float2)
	float2 t = frac(texelSpaceCoord);

    // Step 9: Interpolate results.
	// Lerp between the lerp of result0 and result 1 (using t.x) and the lerp of result2 and result 3 (using t.x), using t.y.
    // Return the result of Step 9
	return lerp( lerp(result0, result1, t.x), lerp(result2, result3, t.x), t.y );
}

float4 PS(VS_OUTPUT input):SV_TARGET
{
	input.Normal = normalize(input.Normal);
	
	float3 diffuse = CalculateDiffuse(input.Normal, input.TexCoord);
	float shadowFactor = CalculateShadowFactor(input.ProjTexCoord) * 2;

	return float4(diffuse * shadowFactor,1);
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


//******************
// SHADOW GENERATION
//******************

VS_OUTPUT VS_ShadowGen(VS_INPUT input)
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

	output.Position = mul(vAnimatedPos, matWorldLightViewProjection);
	output.Normal = mul(Norm, (float3x3)matWorld);
	output.TexCoord = input.TexCoord;

	return output;
}

float4 PS_ShadowGen(VS_OUTPUT input):SV_TARGET
{
	return float4(input.Position.z, input.Position.z, input.Position.z, 1.0f);
}

technique10 TechShadowGen
{
	pass one
	{
		SetVertexShader( CompileShader( vs_4_0, VS_ShadowGen() ) );
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader( ps_4_0, PS_ShadowGen() ) );
	}
}