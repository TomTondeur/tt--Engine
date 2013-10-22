//TERRAIN TESSELLATION TECHNIQUE

//****************
// RENDER STATES *
//****************
DepthStencilState NoDepth
{
	DepthEnable = FALSE;
};

//****************
// VERTEX STRUCT *
//****************
struct GS_DATA{
	float3 Pos : POSITION;
};

//****************
// VERTEX SHADER *
//****************
GS_DATA NoVS(GS_DATA pos)
{
	return pos;
}

//******************
// GEOMETRY SHADER *
//******************

GS_DATA SubDivideEdge(GS_DATA v0, GS_DATA v1)
{
	GS_DATA temp = (GS_DATA)0;
	temp.Pos = (v0.Pos + v1.Pos)*.5f;
	return temp;
}

[maxvertexcount(8)]
void SubdividerGS(triangle GS_DATA vertices[3], inout TriangleStream<GS_DATA> ioStream)
{
	GS_DATA v01, v12, v02;

	v01 = SubDivideEdge(vertices[0], vertices[1]);
	v12 = SubDivideEdge(vertices[1], vertices[2]);
	v02 = SubDivideEdge(vertices[0], vertices[2]);
	
	//bottom - left to right
	ioStream.Append(vertices[0]);
	ioStream.Append(v01);
	ioStream.Append(v02);
	ioStream.Append(v12);
	ioStream.Append(vertices[2]);

	ioStream.RestartStrip();

	//top triangle
	ioStream.Append(v01);
	ioStream.Append(vertices[1]);
	ioStream.Append(v12);
}

//************
// TECHNIQUE *
//************
GeometryShader gsStreamOut = ConstructGSWithSO( CompileShader ( gs_4_0, SubdividerGS() ),
	"POSITION.xyz;");

technique10 TechTessellateTerrain
{
	pass one
	{
		SetVertexShader( CompileShader ( vs_4_0, NoVS() ));
		SetGeometryShader(gsStreamOut);
		SetPixelShader(NULL);
		SetDepthStencilState(NoDepth,0);
	}
}

//SAMPLE HEIGHT MAP TECHNIQUE

//****************
// VERTEX STRUCT *
//****************
struct GS_FINALDATA{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
};

//****************
// GLOBALS       *
//****************
Texture2D<uint> g_HeightMap : HEIGHTMAP;
static const float g_HeightMapScale = 1.0f / 65536.0f;

//No VS

//******************
// GEOMETRY SHADER *
//******************
float3 GetTriNormal(float3 v0, float3 v1, float3 v2)
{
	return normalize(cross(v1-v0,v2-v0));
}

float3 GetNormal(float3 pos, float2 step, float2 dim)
{
	//Gather surrounding vertex positions
	float3 v00 = pos + float3(-step.x, 0, step.y);	v00.y = g_HeightMap.Load(int3(v00.x*dim.x, v00.z*dim.y, 0)) * g_HeightMapScale;
	float3 v10 = pos + float3(0		 , 0, step.y);	v10.y = g_HeightMap.Load(int3(v10.x*dim.x, v10.z*dim.y, 0)) * g_HeightMapScale;
	float3 v01 = pos + float3(-step.x, 0, 0);		v01.y = g_HeightMap.Load(int3(v01.x*dim.x, v01.z*dim.y, 0)) * g_HeightMapScale;
	//v11 = pos
	float3 v21 = pos + float3(step.x, 0, 0);		v21.y = g_HeightMap.Load(int3(v21.x*dim.x, v21.z*dim.y, 0)) * g_HeightMapScale;
	float3 v12 = pos + float3(0		, 0, -step.y);	v12.y = g_HeightMap.Load(int3(v12.x*dim.x, v12.z*dim.y, 0)) * g_HeightMapScale;
	float3 v22 = pos + float3(step.x, 0, -step.y);	v22.y = g_HeightMap.Load(int3(v22.x*dim.x, v22.z*dim.y, 0)) * g_HeightMapScale;

	return (GetTriNormal(pos, v00, v10) + 
			GetTriNormal(pos, v01, v00) + 
			GetTriNormal(pos, v10, v21) + 
			GetTriNormal(pos, v12, v01) + 
			GetTriNormal(pos, v22, v12) + 
			GetTriNormal(pos, v21, v22));
}

[maxvertexcount(3)]
void SampleHeightMapGS(triangle GS_DATA vertices[3], inout TriangleStream<GS_FINALDATA> ioStream)
{
	GS_FINALDATA v0, v1, v2;
	
	v0.Pos = vertices[0].Pos;
	v1.Pos = vertices[1].Pos;
	v2.Pos = vertices[2].Pos;

	float2 dim; g_HeightMap.GetDimensions(dim.x, dim.y);

	v0.Pos.y = g_HeightMap.Load(int3(v0.Pos.x*dim.x, v0.Pos.z*dim.y, 0)) * g_HeightMapScale;
	v1.Pos.y = g_HeightMap.Load(int3(v1.Pos.x*dim.x, v1.Pos.z*dim.y, 0)) * g_HeightMapScale;
	v2.Pos.y = g_HeightMap.Load(int3(v2.Pos.x*dim.x, v2.Pos.z*dim.y, 0)) * g_HeightMapScale;

	v0.Normal += GetNormal(v0.Pos, 1.0f/dim, dim);
	v1.Normal += GetNormal(v1.Pos, 1.0f/dim, dim);
	v2.Normal += GetNormal(v2.Pos, 1.0f/dim, dim);

	ioStream.Append(v0);
	ioStream.Append(v1);
	ioStream.Append(v2);
}

//************
// TECHNIQUE *
//************
GeometryShader gsSample = ConstructGSWithSO( CompileShader ( gs_4_0, SampleHeightMapGS() ),
	"POSITION.xyz; NORMAL.xyz");

technique10 TechSampleHeightMap
{
	pass one
	{
		SetVertexShader( CompileShader ( vs_4_0, NoVS() ));
		SetGeometryShader(gsSample);
		SetPixelShader(NULL);
		SetDepthStencilState(NoDepth,0);
	}
}

//DRAW TECHNIQUE

//****************
// RENDER STATES *
//****************
RasterizerState Wireframe
{
	FillMode = SOLID;
	CullMode = BACK;
};

//****************
// GLOBALS       *
//****************
float4x4 g_matWVP : WorldViewProjection;
float4x4 g_matWorld : World;

//****************
// VERTEX STRUCT *
//****************
struct PS_DATA
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};

//****************
// VERTEX SHADER *
//****************
PS_DATA DrawVS(GS_FINALDATA inVert)
{
	PS_DATA oData;
	oData.TexCoord = float2(inVert.Pos.x, inVert.Pos.z);
	oData.Position = mul(float4(inVert.Pos, 1), g_matWVP);	
	oData.Normal = normalize(mul(inVert.Normal, (float3x3)g_matWorld));
	
	return oData;
}

float4 DrawPS(PS_DATA inVert) : SV_TARGET
{
	float l = saturate(dot( inVert.Normal, float3(0,1,0)));
	return float4(l, l, l, 1);
}

//************
// TECHNIQUE *
//************
technique10 TechDraw
{
	pass one
	{
		SetVertexShader( CompileShader ( vs_4_0, DrawVS() ));
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader ( ps_4_0, DrawPS() ));
		SetRasterizerState(Wireframe);
	}
}
