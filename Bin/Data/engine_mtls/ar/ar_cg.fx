// ar_cg.fx

void v_ar
(
    in float3 modelPosition : POSITION,
    in float2 modelTCoord0 : TEXCOORD0,
    out float4 clipPosition : POSITION,
    out float2 vertexTCoord0 : TEXCOORD0,
	uniform float4x4 PVWMatrix
)
{
    clipPosition = mul(PVWMatrix, float4(modelPosition, 1.0f));
    vertexTCoord0 = modelTCoord0;
}

sampler2D SamplerColor;
sampler2D SamplerDepth;
sampler2D SamplerColorScene;
sampler2D SamplerDepthScene;

void p_ar
(
    in float2 vertexTCoord0 : TEXCOORD0,
    out float4 pixelColor : COLOR,
	uniform float4 ARParam
)
{
	float2 uv = float2(vertexTCoord0.x, 1.0f-vertexTCoord0.y);
	float4 colorColor = tex2D(SamplerColor, uv);
	float4 depthColor = tex2D(SamplerDepth, uv);
	float4 colorSceneColor = tex2D(SamplerColorScene, uv);
	float4 depthSceneColor = tex2D(SamplerDepthScene, uv);
	
	pixelColor = colorColor;
	
	if (depthSceneColor.r < depthColor.r)
		pixelColor = colorSceneColor;
}