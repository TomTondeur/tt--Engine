cbuffer cbPerObject{
	float4x4 matWorld : World;
	float4x4 matView : View;
	float4x4 matWorldViewProj : WorldViewProjection;
	float3 vLightDir : DIRECTION;
	float4 color = float4(1,1,1,1);
};

cbuffer cbPerFrame{

};

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
};

VS_OUTPUT VS(VS_INPUT input){

	VS_OUTPUT output = (VS_OUTPUT)0;

	// Step 1:	convert position into float4 and multiply with matWorldViewProj
	//			the result is the position in homogeneous clip space
	output.PosH = mul(float4(input.PosL,1), matWorldViewProj);
	
	// Step 2:	rotate the normal: NO TRANSLATION
	//			this is achieved by casting the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
	//output.Normal = mul(normalize(input.Normal), (float3x3)matWorld);
	output.Normal = mul(float4(input.Normal, 1), matView);
	// Step3:	Just copy the texcoord
	output.TexCoord = input.TexCoord;

	return output;
}

float4 PS(VS_OUTPUT input):SV_TARGET
{
	float diffStrength = saturate(dot(-input.Normal, vLightDir));
	float4 outC= m_Texture.Sample(samLinear, input.TexCoord) * color ;// * diffStrength;
	return outC;
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

