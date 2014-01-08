static const float SHADOW_EPSILON = 0.004f;
static const float SMAP_WIDTH = 1280.0f;
static const float SMAP_HEIGHT = 720.0f;
static const float SMAP_DX = 1.0f / SMAP_WIDTH;
static const float SMAP_DY = 1.0f / SMAP_HEIGHT;

cbuffer cbPerObject{
	float4x4 matWorld : World;
	float4x4 matView : View;
	float4x4 matWorldViewProj : WorldViewProjection;
	float3 vLightDir : DIRECTION;
	float4 color = float4(1,1,1,1);
};

cbuffer cbPerFrame{

};

cbuffer cbShadowGen
{
	float4x4 matWorldLightViewProjection : LightViewProjection;
};

Texture2D texShadowMap : ShadowMapSRV;
	
Texture2D m_Texture : TEXTURE_DIFFUSE;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

RasterizerState Wireframe
{
	FillMode = WIREFRAME;
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

struct VS_INPUT{
	//iPosH represents the position in Local (object) space
	float3 PosL : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT{
	//oPosH represents the position in homogeneous clip space
	float4 PosH : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
	float4 ProjTexCoord : TEXCOORD1;
};

VS_OUTPUT VS(VS_INPUT input){

	VS_OUTPUT output = (VS_OUTPUT)0;

	// Step 1:	convert position into float4 and multiply with matWorldViewProj
	//			the result is the position in homogeneous clip space
	output.PosH = mul(float4(input.PosL,1.0f), matWorldViewProj);
	
	// Step 2:	rotate the normal: NO TRANSLATION
	//			this is achieved by casting the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
	output.Normal = mul(normalize(input.Normal), (float3x3)matWorld);

	// Step3:	Just copy the texcoord
	output.TexCoord = input.TexCoord;

	// Step4: Calculate the Projected Texture Coordinates
	output.ProjTexCoord = mul(float4(input.PosL, 1.0f), matWorldLightViewProjection);
	
	return output;
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
	float diffStrength = saturate(dot(-input.Normal, vLightDir));
	float shadowFactor = CalculateShadowFactor(input.ProjTexCoord);
	return m_Texture.Sample(samLinear, input.TexCoord) * color * diffStrength * shadowFactor;
}

//no diffuse light when rendering wireframe
float4 PSNoNormal(VS_OUTPUT input):SV_TARGET
{
	return color;
}

technique10 TechSolid
{
	pass one
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader ( ps_4_0, PS() ));
		SetRasterizerState(Solid);
		SetBlendState(NoBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	}
}

technique10 TechWireframe
{
	pass one
	{
		SetVertexShader( CompileShader ( vs_4_0, VS() ));
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader ( ps_4_0, PSNoNormal() ));
		SetRasterizerState(Wireframe);
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
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
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

	// Step 1:	convert position into float4 and multiply with matWorldLightViewProjection
	//			the result is the position in homogeneous clip space
	output.PosH = mul(float4(input.PosL,1), matWorldLightViewProjection);
	// Step 2:	rotate the normal: NO TRANSLATION
	//			this is achieved by casting the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
	output.Normal = mul(normalize(input.Normal), (float3x3)matWorld);

	// Step3:	Just copy the texcoord
	output.TexCoord = input.TexCoord;

	return output;
}

float4 PS_ShadowGen(VS_OUTPUT input):SV_TARGET
{
	return float4(input.PosH.z, input.PosH.z, input.PosH.z, 1.0f);
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