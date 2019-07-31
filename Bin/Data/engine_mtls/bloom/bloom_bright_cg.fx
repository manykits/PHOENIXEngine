// bloom_bright_cg.fx

void v_bloom_bright
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

sampler2D SamplerBase;

void p_bloom_bright
(
    in float2 vertexTCoord0 : TEXCOORD0,
    out float4 pixelColor : COLOR,
	uniform float4 BrightParam
)
{
	float2 uv = float2(vertexTCoord0.x, 1.0f-vertexTCoord0.y);
	float4 color = tex2D(SamplerBase, uv);
	
	// 减去暗的像素
	color.rgb -= normalize(color.rgb) * BrightParam.r;
	
	color = max(color, 0.0f);
	
	color.rgb = color.rgb * color.rgb;
	
	pixelColor = color;
}