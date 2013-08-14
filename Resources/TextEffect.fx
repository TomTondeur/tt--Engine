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
float2 texSize : TextureDimensions;

BlendState EnableBlending 
{     
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

DepthStencilState NoDepth
{
	DepthEnable = false;
};

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

//SHADER STRUCTS
//**************
struct GS_INPUT
{
	float3 Position : POSITION;
	float4 Color: COLOR;
	float2 TexCoord: TEXCOORD0;
	float2 CharSize: TEXCOORD1;
	int Channel: TEXCOORD2;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float4 Color: COLOR;
	float2 TexCoord: TEXCOORD0;
	int Channel: TEXCOORD1;
};

//VERTEX SHADER
//*************
GS_INPUT MainVS(GS_INPUT input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<PS_INPUT> triStream, float3 pos, float4 col, float2 texCoord, int channel)
{
	PS_INPUT temp = (PS_INPUT)0;
	temp.Position = mul(float4(pos, 1), matTransform);
	temp.Color = col;
	temp.TexCoord = texCoord;
	temp.Channel = channel;

	triStream.Append(temp);
}

[maxvertexcount(4)]
void MainGS(point GS_INPUT vertex[1], inout TriangleStream<PS_INPUT> triStream)
{
	//Calculate texOffset
	float2 texOffset = vertex[0].CharSize / texSize;

	//texOffset.y = -texOffset.y;
	
	CreateVertex(triStream, vertex[0].Position,										vertex[0].Color, vertex[0].TexCoord,							vertex[0].Channel); //left-bot
	CreateVertex(triStream, vertex[0].Position + float3(vertex[0].CharSize.x, 0, 0),vertex[0].Color, vertex[0].TexCoord + float2(texOffset.x, 0),	vertex[0].Channel); //right-bot
	CreateVertex(triStream, vertex[0].Position + float3(0, vertex[0].CharSize.y, 0),vertex[0].Color, vertex[0].TexCoord + float2(0, texOffset.y),	vertex[0].Channel); //left-top
	CreateVertex(triStream, vertex[0].Position + float3(vertex[0].CharSize, 0),		vertex[0].Color, vertex[0].TexCoord + texOffset,				vertex[0].Channel); //right-top
}

//PIXEL SHADER
//************
float4 MainPS(PS_INPUT input) : SV_TARGET {
	return input.Color * texSprite.Sample(samPoint, input.TexCoord)[input.Channel];
}

// Default Technique
technique10 Default {

	pass p0 {
		SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState(NoDepth, 0);

		SetVertexShader(	CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(	CompileShader(gs_4_0, MainGS()));
		SetPixelShader(		CompileShader(ps_4_0, MainPS()));
	}
}
