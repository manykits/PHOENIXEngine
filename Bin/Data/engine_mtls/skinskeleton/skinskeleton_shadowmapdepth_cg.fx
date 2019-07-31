void v_skinskeleton_shadowmapdepth
(
    in float3 modelPosition : POSITION,
    in float2 modelTCoord0 : TEXCOORD0,
	in float4 modelTCoord1 : TEXCOORD1, // indeces
	in float4 modelTCoord2 : TEXCOORD2, // wights
    out float4 clipPosition : POSITION,
    out float2 vertexTCoord0 : TEXCOORD0,
	out float2 vertexTCoord1 : TEXCOORD1,
	uniform float4x4 PVWMatrix,
	uniform float4 BoneTM[108]
)
{
	// Pos Normal
	int i0 = modelTCoord1[0]*3;
	int i1 = modelTCoord1[1]*3;
	int i2 = modelTCoord1[2]*3;
	int i3 = modelTCoord1[3]*3;
	float4 weight = modelTCoord2;
	
	float4 inputPos = float4(modelPosition, 1.0f);
	float3 worldPosition = float3(0, 0, 0);	
	worldPosition += float3(dot(inputPos, BoneTM[i0]), dot(inputPos, BoneTM[i0 + 1]), dot(inputPos, BoneTM[i0 + 2])) * weight[0];
	worldPosition += float3(dot(inputPos, BoneTM[i1]), dot(inputPos, BoneTM[i1 + 1]), dot(inputPos, BoneTM[i1 + 2])) * weight[1];
	worldPosition += float3(dot(inputPos, BoneTM[i2]), dot(inputPos, BoneTM[i2 + 1]), dot(inputPos, BoneTM[i2 + 2])) * weight[2];	
	worldPosition += float3(dot(inputPos, BoneTM[i3]), dot(inputPos, BoneTM[i3 + 1]), dot(inputPos, BoneTM[i3 + 2])) * weight[3];
	
	clipPosition = mul(PVWMatrix, float4(worldPosition, 1.0f));
	
	vertexTCoord0 = modelTCoord0;
	vertexTCoord1.r = clipPosition.z/clipPosition.w;
}

sampler2D SampleBase;

void p_skinskeleton_shadowmapdepth
(
    in float2 vertexTCoord0 : TEXCOORD0,
	in float2 vertexTCoord1 : TEXCOORD1,
    out float4 pixelColor : COLOR,
	uniform float4 UVOffset
)
{
    float2 texCoord = vertexTCoord0;
    texCoord.y = 1.0 - vertexTCoord0.y;
	texCoord.xy += UVOffset.xy;
    float4 lastColor = tex2D(SampleBase, texCoord);
	
	if (lastColor.a < 0.25)
	{
		discard;
	}
	else
	{		
		pixelColor.rgb = vertexTCoord1.r;
		pixelColor.a = 1.0;
	}
}
