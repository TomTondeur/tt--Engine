Texture2D g_TexColor : G_COLOR;
Texture2D g_TexNormal : G_NORMAL;

float3 vLightDir = float3(1,0,0);

BlendState EnableBlending 
{     
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

struct VS_INPUT
{
	float3 Position : POSITION;
	float2 TexCoord : TEXCOORD;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;
	
	output.Position = float4(input.Position, 1);
	output.TexCoord = input.TexCoord;
	
	return output;
}

float3 CalculateDiffuse(float3 normal, float3 color)
{
	float3 retColor = float3(1,1,1);
	float intensity = dot(-normal, vLightDir);
	
	intensity = (saturate(intensity) + 1) / 2;
	intensity = pow(intensity, 1.5); //apply lambert power
	retColor *= intensity;
	
	retColor *= color;
	
	return retColor;
}

float4 PS(PS_INPUT input) : SV_TARGET
{
	float4 diffuse = g_TexColor.Sample(samLinear, input.TexCoord);
	float3 normal = g_TexNormal.Sample(samLinear, input.TexCoord).xyz;
	normal = (normal - float3(1,1,1)) * 2; //Restore normal from 0 - 1 to -1 - 1 range
	
	float3 finalRGB = CalculateDiffuse(normal, diffuse.rgb);
	
	return float4(finalRGB, diffuse.a);
}

RasterizerState NoCull
{
	CullMode = NONE;
};

DepthStencilState LessEqualDSS
{
	// Make sure the depth function is LESS_EQUAL and not just LESS.
    	// Otherwise, the normalized depth values at z = 1 (NDC) will
    	// fail the depth test if the depth buffer was cleared to 1.
	DepthFunc = LESS_EQUAL;
};

technique10 TechDraw
{
    pass one
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );   
		
		SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetRasterizerState(NoCull);
		SetDepthStencilState(LessEqualDSS,0);
    }
}
