void v_terrain_shadowmapdepth
(
    in float3 modelPosition : POSITION,
    out float4 clipPosition : POSITION,
    out float2 vertexTCoord0 : TEXCOORD0,
	uniform float4x4 PVWMatrix
)
{
    // Pos
    clipPosition = mul(PVWMatrix, float4(modelPosition,1.0f));
	vertexTCoord0.r = clipPosition.z/clipPosition.w;
}

void p_terrain_shadowmapdepth
(
    in float2 vertexTCoord0 : TEXCOORD0,
    out float4 pixelColor : COLOR
)
{
	pixelColor.rgb = vertexTCoord0.r;
	pixelColor.a = 1.0;
}