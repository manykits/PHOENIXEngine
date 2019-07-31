// bloom_cg.fx

void v_bloom
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
sampler2D SamplerBloom;

void p_bloom
(
    in float2 vertexTCoord0 : TEXCOORD0,
    out float4 pixelColor : COLOR,
	uniform float4 BloomParam
)
{
	float2 uv = float2(vertexTCoord0.x, 1.0f-vertexTCoord0.y);
	float4 baseColor = tex2D(SamplerBase, uv);
	float4 bloomColor = tex2D(SamplerBloom, uv);
	
	pixelColor = baseColor + bloomColor * BloomParam.r;
}