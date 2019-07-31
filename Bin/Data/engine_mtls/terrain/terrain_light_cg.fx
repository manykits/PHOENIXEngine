
float3 DoLight_Point_Diffuse(float3 lightWorldPos, float lightRange, float3 lightColor, float3 shineDiffuse, float3 vertexWorldPos, float3 vertexWorldNormal)
{
	float3 lightToVertex = lightWorldPos - vertexWorldPos;
	float squareDist = dot(lightToVertex, lightToVertex);
	lightToVertex = normalize(lightToVertex);
	return lightColor * shineDiffuse * max(0, dot(vertexWorldNormal, lightToVertex)) * max( 0, (1.0 - squareDist / lightRange / lightRange) );
}

void v_terrain_light
(
    in float3 modelPosition : POSITION,
    in float3 modelNormal : NORMAL,
    in float2 modelTCoord0 : TEXCOORD0,
    out float4 clipPosition : POSITION,
    out float4 vertexColor : COLOR,
    out float2 vertexTCoord0 : TEXCOORD0,
	out float2 vertexTCoord1 : TEXCOORD1,
	uniform float4x4 PVWMatrix,
	uniform float4x4 WMatrix,
	uniform float4 CameraWorldPosition,
	uniform float4 LightWorldDVector_Dir,
	uniform float4 ShineEmissive,
	uniform float4 ShineAmbient,
	uniform float4 ShineDiffuse,
	uniform float4 ShineSpecular,	
	uniform float4 LightAmbient_Dir,
	uniform float4 LightDiffuse_Dir,
	uniform float4 LightSpecular_Dir,
	uniform float4 LightGroup[8],
	uniform float4 FogParam
)
{
    // Pos
    clipPosition = mul(PVWMatrix, float4(modelPosition,1.0f));

    // Tex Coord
    vertexTCoord0 = modelTCoord0;
	
	// params
	float4 worldPosition = mul(WMatrix, float4(modelPosition, 1.0f));
	float3 worldNormal = normalize(mul((float3x3)WMatrix, modelNormal));	
	
	float3 viewVector = normalize(CameraWorldPosition.xyz - worldPosition.xyz);
	float dist = distance(CameraWorldPosition.xyz, worldPosition.xyz);
	
	// light
	float3 halfVector = normalize((viewVector - LightWorldDVector_Dir.xyz)/2.0);
	float dotH = dot(worldNormal, halfVector);
	
	vertexColor.rgb = ShineEmissive.rgb + LightAmbient_Dir.a * (ShineAmbient.rgb * LightAmbient_Dir.rgb +
		ShineDiffuse.rgb * LightDiffuse_Dir.rgb * max(dot(worldNormal, -LightWorldDVector_Dir.rgb), 0) +
							ShineSpecular.rgb * LightSpecular_Dir.rgb * pow(max(dotH, 0), ShineSpecular.a*LightSpecular_Dir.a));		
	vertexColor.a = ShineDiffuse.a;
	
	// point lights
	vertexColor.rgb += DoLight_Point_Diffuse(LightGroup[0].xyz, LightGroup[0].w, LightGroup[1].xyz, ShineDiffuse.rgb, worldPosition.xyz, worldNormal.xyz);
	vertexColor.rgb += DoLight_Point_Diffuse(LightGroup[2].xyz, LightGroup[2].w, LightGroup[3].xyz, ShineDiffuse.rgb, worldPosition.xyz, worldNormal.xyz);
	vertexColor.rgb += DoLight_Point_Diffuse(LightGroup[4].xyz, LightGroup[4].w, LightGroup[5].xyz, ShineDiffuse.rgb, worldPosition.xyz, worldNormal.xyz);
	vertexColor.rgb += DoLight_Point_Diffuse(LightGroup[6].xyz, LightGroup[6].w, LightGroup[7].xyz, ShineDiffuse.rgb, worldPosition.xyz, worldNormal.xyz);
	
	// fog
	float fogValueHeight = (-FogParam.x + worldPosition.z)/(FogParam.y - FogParam.x);
	fogValueHeight = clamp(fogValueHeight, 0, 1.0);	
	float fogValueDist = (FogParam.w - dist)/(FogParam.w - FogParam.z);
	fogValueDist = clamp(fogValueDist, 0, 1.0);

	vertexTCoord1.x = fogValueDist;
	vertexTCoord1.y = fogValueHeight;
}

sampler2D SamplerAlpha;
sampler2D Sampler0;
sampler2D Sampler1;
sampler2D Sampler2;
sampler2D Sampler3;
sampler2D Sampler4;
void p_terrain_light
(
    in float4 vertexColor : COLOR,
    in float2 vertexTCoord0 : TEXCOORD0,
	in float2 vertexTCoord1 : TEXCOORD1,
    out float4 pixelColor : COLOR,
	uniform float4 UVScale01,
	uniform float4 UVScale23,
	uniform float4 UVScale4,
	uniform float4 FogColorHeight,
	uniform float4 FogColorDist
)
{
    // texture	
	float2 texCoord = vertexTCoord0;
    texCoord.y = 1.0 - vertexTCoord0.y;
	
	float4 colorAlpha = tex2D(SamplerAlpha, vertexTCoord0);
    float4 color0 = tex2D(Sampler0, texCoord*UVScale01.xy);
   	float4 color1 = tex2D(Sampler1, texCoord*UVScale01.zw);
    float4 color2 = tex2D(Sampler2, texCoord*UVScale23.xy);
    float4 color3 = tex2D(Sampler3, texCoord*UVScale23.zw);
    float4 color4 = tex2D(Sampler4, texCoord*UVScale4.xy);
    
	float4 lastColor = lerp(color0 ,color1, colorAlpha.r);
    lastColor = lerp(lastColor ,color2, colorAlpha.g);
    lastColor = lerp(lastColor ,color3, colorAlpha.b);
    lastColor = lerp(lastColor ,color4, colorAlpha.a);
	lastColor *= vertexColor;
	
	// fog
	lastColor.rgb = lerp(FogColorHeight.rgb, lastColor.rgb, vertexTCoord1.y);
	lastColor.rgb = lerp(FogColorDist.rgb, lastColor.rgb, vertexTCoord1.x);
	
	pixelColor = lastColor;
}