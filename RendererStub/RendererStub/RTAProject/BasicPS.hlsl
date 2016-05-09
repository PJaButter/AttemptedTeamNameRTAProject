#pragma pack_matrix(row_major)


cbuffer Spot : register(b0)
{
	float4 pos;
	float4 dir;
	float4 ratio; //x - inner y-outer z-radius w - 0
	float4 spotcolor;
}

cbuffer pointRot : register(b1)
{
	float4x4 pointMatrix;
}

cbuffer pointRot : register(b2)
{
	float4x4 dirMatrix;
}

struct OUTPUT_VERTEX
{
	float2 uvs : TEXCOORD;
	float4 projectedCoordinate : SV_POSITION;
	float4 WorldPos : POSITION;
	float3 nrms : NORMAL;
};

texture2D baseTexture : register(t0);

SamplerState filter : register(s0);

float4 main(OUTPUT_VERTEX PS_IN) : SV_TARGET
{

	float3 surNorm = float3(PS_IN.nrms.x, PS_IN.nrms.y, PS_IN.nrms.z);

	float4 dirPos = dirMatrix[3].xyzw;

	float4 dirLight = float4(0.08f, 0.075f, 0.3f, 0.75f);

	float3 lightdir = float3(0, -1, 1);

	float lightratio = saturate(dot(-lightdir, normalize(surNorm)));


float3 pointPos = mul(float3(0, 10, 5), pointMatrix);

float3 pointdir = normalize(pointPos - PS_IN.WorldPos);

float4 pointColor = float4(1, 1, 1, 0.75f);

float pointratio = saturate(dot(pointdir, normalize(surNorm)));

float4 baseColor = baseTexture.Sample(filter, PS_IN.uvs);

float attenuation = 1.0f - saturate(length(pointPos - PS_IN.WorldPos) / 15.0f);

float4 pointresult = pointratio * pointColor * baseColor;

pointresult *= attenuation;

float4 DirColor = lightratio * dirLight * baseColor;

float4 finalColor = saturate(pointresult + DirColor);


return  finalColor;
}


