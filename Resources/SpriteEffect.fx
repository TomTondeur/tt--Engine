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

float4x4 matTransform : ViewTransform;

Texture2D texSprite : Texture;
uint texWidth : Width;
uint texHeight : Height;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

BlendState EnableBlending 
{     
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

RasterizerState BackfaceCulling 
{ 
	CullMode = BACK; 
};

struct VS_INPUT
{
	float3 Position : POSITION;
	float3 MatRow0 : TEXCOORD0;
	float3 MatRow1 : TEXCOORD1;
	float3 MatRow2 : TEXCOORD2;
	float4 Color: COLOR;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float4 Color: COLOR;
	float2 TexCoord: TEXCOORD;
};

//Pass through vertex shader
VS_INPUT MainVS(VS_INPUT input)
{	
	return input;
}

void CreateVertex(inout TriangleStream<PS_INPUT> stream, float4 pos, float2 texCoord, float4 color)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Position = pos;
	output.TexCoord = texCoord;
	output.Color = color;

	stream.Append(output);
}

[maxvertexcount(4)]
void MainGS(point VS_INPUT input[1], inout TriangleStream<PS_INPUT> stream)
{	
	float4x4 matTrans;/*{ {input[0].MatRow0.x,  input[0].MatRow0.y,  input[0].MatRow0.z,  0}
					  ,{input[0].MatRow1.x,  input[0].MatRow1.y,  input[0].MatRow1.z,  0}
					  ,{input[0].MatRow2.x,  input[0].MatRow2.y,  input[0].MatRow2.z,  0}
					  ,{input[0].Position.x, input[0].Position.y, input[0].Position.z, 1}};*/ //Y U NO WORK?
	matTrans[0][0] = input[0].MatRow0.x;	matTrans[0][1] = input[0].MatRow0.y;	matTrans[0][2] = input[0].MatRow0.z;	matTrans[0][3] = 0;
	matTrans[1][0] = input[0].MatRow1.x;	matTrans[1][1] = input[0].MatRow1.y;	matTrans[1][2] = input[0].MatRow1.z;	matTrans[1][3] = 0;
	matTrans[2][0] = input[0].MatRow2.x;	matTrans[2][1] = input[0].MatRow2.y;	matTrans[2][2] = input[0].MatRow2.z;	matTrans[2][3] = 0;
	matTrans[3][0] = 0;						matTrans[3][1] = 0;						matTrans[3][2] = 0;						matTrans[3][3] = 1;
	matTrans = mul(matTrans, matTransform);
	
	float3 vUp =	mul(float3(0,texHeight,0), (float3x3)matTrans);
	float3 vRight = mul(float3(texWidth,0, 0), (float3x3)matTrans);

	float4 pos = mul(float4(input[0].Position, 1), matTrans);

	CreateVertex(stream, pos,						float2(0,0), input[0].Color); //bot-left
	CreateVertex(stream, pos + float4(vRight, 0),	float2(1,0), input[0].Color); //bot-right
	CreateVertex(stream, pos + float4(vUp, 0),		float2(0,1), input[0].Color); //top-left
	CreateVertex(stream, pos + float4(vUp+vRight,0),float2(1,1), input[0].Color); //top-right
}

float4 MainPS(PS_INPUT input) : SV_TARGET {	
	return texSprite.Sample(samLinear, input.TexCoord) * input.Color;
}

// Default Technique
technique10 Default {
	pass p0 {
		SetRasterizerState(BackfaceCulling);
		SetBlendState(EnableBlending,float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader(  CompileShader(vs_4_0, MainVS() ) );
		SetGeometryShader(CompileShader(gs_4_0, MainGS() ) );
		SetPixelShader(	  CompileShader(ps_4_0, MainPS() ) );
	}
}
