// blur_cg.fx

void v_blur
(
    in float3 modelPosition : POSITION,
    in float2 modelTCoord0 : TEXCOORD0,
    out float4 clipPosition : POSITION,
    out float2 vertexTCoord0 : TEXCOORD0,
	uniform float4x4 PVWMatrix
)
{
    clipPosition = mul(PVWMatrix, float4(modelPosition,1.0f));
    vertexTCoord0 = modelTCoord0;
}

sampler2D SampleBase;

void p_blur
(
    in float2 vertexTCoord0 : TEXCOORD0,
    out float4 pixelColor : COLOR,
	uniform float4 UVParam,
	uniform float4 ShineEmissive,
	uniform float4 UVOffsets[16]
)
{
	float2 uv = float2(vertexTCoord0.x, 1.0f-vertexTCoord0.y) * UVParam.xy;
	
	float2 uvSample = 0.0f;
	float4 sampleColor = 0.0f;
	float4 lastColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 15; i++)
    {
        uvSample = uv + UVOffsets[i].xy;
        sampleColor = tex2D(SampleBase, uvSample);
        lastColor += sampleColor * UVOffsets[i].z;
    }
	pixelColor = lastColor*ShineEmissive;
}