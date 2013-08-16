// Copyright © 2013 Tom Tondeur
// 
// This file is part of tt::Engine.
// 
// tt::Engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// tt::Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with tt::Engine.  If not, see <http://www.gnu.org/licenses/>.

Texture2D gTexture : ColorMap;

int gOffset = 5;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

RasterizerState BackfaceCulling 
{ 
	CullMode = BACK; 
};

struct VS_INPUT_STRUCT
{
    float3 Pos : POSITION;
	float2 TexCoord : TEXCOORD0;

};
struct PS_INPUT_STRUCT
{
    float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};

PS_INPUT_STRUCT VS(VS_INPUT_STRUCT input)
{
	PS_INPUT_STRUCT output = (PS_INPUT_STRUCT)0;
	
	output.Pos = float4(input.Pos, 1);
	output.TexCoord = input.TexCoord;

	return output;
}

float4 PS(PS_INPUT_STRUCT input):SV_TARGET
{
	//FIXME
	float du = .2f/1280.0f;
	float dv = .2f/1024.0f;
	
	float3 p0 = gTexture.Sample(samLinear, input.TexCoord + float2(0			,0));
	float3 p1 = gTexture.Sample(samLinear, input.TexCoord + float2(-gOffset*du	,0));
	float3 p2 = gTexture.Sample(samLinear, input.TexCoord + float2(-gOffset*du	,gOffset*dv));
	float3 p3 = gTexture.Sample(samLinear, input.TexCoord + float2(0			,gOffset*dv));
	float3 p4 = gTexture.Sample(samLinear, input.TexCoord + float2(gOffset*du	,gOffset*dv));
	float3 p5 = gTexture.Sample(samLinear, input.TexCoord + float2(gOffset*du	,0));
	float3 p6 = gTexture.Sample(samLinear, input.TexCoord + float2(gOffset*du	,-gOffset*dv));
	float3 p7 = gTexture.Sample(samLinear, input.TexCoord + float2(0			,-gOffset*dv));
	float3 p8 = gTexture.Sample(samLinear, input.TexCoord + float2(-gOffset*du	,-gOffset*dv));
	
	return float4( (p0+p1+p2+p3+p4+p5+p6+p7+p8)/9, 1);
}

technique10 Blur
{
    pass P0
    {      
		SetRasterizerState(BackfaceCulling);    
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}