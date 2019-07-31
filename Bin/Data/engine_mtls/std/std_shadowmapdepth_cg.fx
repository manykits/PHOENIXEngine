
void v_std_shadowmapdepth
(
    in float3 modelPosition : POSITION,
    in float2 modelTCoord0 : TEXCOORD0,
    out float4 clipPosition : POSITION,
    out float2 vertexTCoord0 : TEXCOORD0,
	out float2 vertexTCoord1 : TEXCOORD1,
    uniform float4x4 PVWMatrix
)
{
    clipPosition = mul(PVWMatrix, float4(modelPosition, 1.0f));
    vertexTCoord0 = modelTCoord0;
	vertexTCoord1.r = clipPosition.z/clipPosition.w;
}

sampler2D SampleBase;

void p_std_shadowmapdepth
(
    in float2 vertexTCoord0 : TEXCOORD0,
	in float2 vertexTCoord1 : TEXCOORD1,
    out float4 pixelColor : COLOR
)
{
    // base
    float2 texCoord = vertexTCoord0;
    texCoord.y = 1.0 - vertexTCoord0.y;
	
	float4 texColor = tex2D(SampleBase, texCoord);
	if (texColor.a < 0.25)
	{
		discard;
	}
	else
	{
		pixelColor.rgb = vertexTCoord1.r;
		pixelColor.a = 1.0;
	}
}
