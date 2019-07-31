// ui.fx

void v_ui
(
    in float3 modelPosition : POSITION,
	in float4 modelColor0 : COLOR,
    in float2 modelTCoord0 : TEXCOORD0,
    out float4 clipPosition : POSITION,
    out float2 vertexTCoord0 : TEXCOORD0,
	out float4 vertexTCoord1 : TEXCOORD1,
	uniform float4x4 PVWMatrix
)
{
    clipPosition = mul(PVWMatrix, float4(modelPosition,1.0f));
    vertexTCoord0 = modelTCoord0;
	vertexTCoord1 = modelColor0;
}

sampler2D SampleBase;

void p_ui
(
    in float2 vertexTCoord0 : TEXCOORD0,
	in float4 vertexTCoord1 : TEXCOORD1,
    out float4 pixelColor : COLOR,
	uniform float4 UVParam,
	uniform float4 ShineEmissive
)
{
	float2 texCoord = vertexTCoord0;
    texCoord.y = 1.0 - vertexTCoord0.y;
	texCoord *= UVParam.xy;

    float4 texColor = tex2D(SampleBase, texCoord);
	pixelColor = texColor*vertexTCoord1*ShineEmissive;
}