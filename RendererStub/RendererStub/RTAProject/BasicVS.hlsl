#pragma pack_matrix(row_major)


struct INPUT_VERTEX
{
	float2 uvs : TEXCOORD;
	float4 coordinate : POSITION;
	float3 nrms : NORMAL;
};

struct OUTPUT_VERTEX
{
	float2 uvs : TEXCOORD;
	float4 projectedCoordinate : SV_POSITION;
	float4 WorldPos : POSITION;
	float3 nrms : NORMAL;
};

cbuffer OBJECT : register(b0)
{
	float4x4 worldMatrix;
}

cbuffer SCENE : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}


OUTPUT_VERTEX main(INPUT_VERTEX fromVertexBuffer)
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;



	
	float4 localVert = float4(fromVertexBuffer.coordinate.xyz, 1.0f);

	localVert = mul(localVert, worldMatrix);
	sendToRasterizer.WorldPos = localVert;
	localVert = mul(localVert, viewMatrix);
	localVert = mul(localVert, projectionMatrix);


	sendToRasterizer.projectedCoordinate = localVert;
	sendToRasterizer.uvs = fromVertexBuffer.uvs;
	sendToRasterizer.nrms = mul(float4(fromVertexBuffer.nrms, 0), worldMatrix).xyz;


	return sendToRasterizer;
}