
void v_effect
(
    in float3 modelPosition : POSITION,
	in float4 modelColor : COLOR,
    in float2 modelTCoord0 : TEXCOORD0,
    out float4 clipPosition : POSITION,
	out float4 vertexColor : COLOR,
    out float2 vertexTCoord0 : TEXCOORD0,
	uniform float4x4 PVWMatrix
)
{
    clipPosition = mul(PVWMatrix, float4(modelPosition,1.0f));

    vertexColor = modelColor;

    vertexTCoord0 = modelTCoord0;
}

sampler2D SampleBase;

void p_effect
(
    in float4 vertexColor : COLOR,
    in float2 vertexTCoord0 : TEXCOORD0,
    out float4 pixelColor : COLOR
)
{
	float2 uv = float2(vertexTCoord0.x, 1.0f-vertexTCoord0.y);
    pixelColor = tex2D(SampleBase, uv);
    pixelColor *= vertexColor;
}
