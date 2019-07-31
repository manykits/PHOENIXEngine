float3 DoLight_Point_Diffuse(float3 lightWorldPos, float lightRange, float3 lightColor, float3 shineDiffuse, float3 vertexWorldPos, float3 vertexWorldNormal)
{
	float3 lightToVertex = lightWorldPos - vertexWorldPos;
	float squareDist = dot(lightToVertex, lightToVertex);
	lightToVertex = normalize(lightToVertex);
	return lightColor * shineDiffuse * max(0.0, dot(vertexWorldNormal, lightToVertex)) * max( 0.0, (1.0 - squareDist / lightRange / lightRange) );
}

void v_std_lightshadow
(
    in float3 modelPosition : POSITION,
	in float3 modelNormal : NORMAL,
    in float2 modelTCoord0 : TEXCOORD0,
    out float4 clipPosition : POSITION,
    out float2 vertexTCoord0 : TEXCOORD0,
	out float2 vertexTCoord1 : TEXCOORD1,
	out float4 vertexTCoord2 : TEXCOORD2,
	out float4 vertexTCoord3 : TEXCOORD3,
    uniform float4x4 PVWMatrix,
	uniform float4x4 WMatrix,
    uniform float4x4 ProjectPVBSMatrix_Dir,
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
	
	vertexTCoord3.rgb = ShineEmissive.rgb + LightAmbient_Dir.a * (ShineAmbient.rgb * LightAmbient_Dir.rgb +
		ShineDiffuse.rgb * LightDiffuse_Dir.rgb * max(dot(worldNormal, -LightWorldDVector_Dir.rgb), 0.0) +
							ShineSpecular.rgb * LightSpecular_Dir.rgb * pow(max(dotH, 0.0), ShineSpecular.a*LightSpecular_Dir.a));
	vertexTCoord3.a = ShineEmissive.a;
	
	// point lights
	vertexTCoord3.rgb += DoLight_Point_Diffuse(LightGroup[0].xyz, LightGroup[0].w, LightGroup[1].rgb, ShineDiffuse.rgb, worldPosition.xyz, worldNormal.xyz);
	vertexTCoord3.rgb += DoLight_Point_Diffuse(LightGroup[2].xyz, LightGroup[2].w, LightGroup[3].rgb, ShineDiffuse.rgb, worldPosition.xyz, worldNormal.xyz);
	vertexTCoord3.rgb += DoLight_Point_Diffuse(LightGroup[4].xyz, LightGroup[4].w, LightGroup[5].rgb, ShineDiffuse.rgb, worldPosition.xyz, worldNormal.xyz);
	vertexTCoord3.rgb += DoLight_Point_Diffuse(LightGroup[6].xyz, LightGroup[6].w, LightGroup[7].rgb, ShineDiffuse.rgb, worldPosition.xyz, worldNormal.xyz);
	
	// fog
	float fogValueHeight = (-FogParam.x + worldPosition.z)/(FogParam.y - FogParam.x);
	fogValueHeight = clamp(fogValueHeight, 0, 1.0);	
	float fogValueDist = (FogParam.w - dist)/(FogParam.w - FogParam.z);
	fogValueDist = clamp(fogValueDist, 0, 1.0);
	
	vertexTCoord1.x = fogValueHeight;
	vertexTCoord1.y = fogValueDist;
	
	// shadow
    vertexTCoord2 = mul(ProjectPVBSMatrix_Dir, float4(modelPosition, 1.0f));
}

sampler2D SampleBase;
sampler2D SampleShadowDepth;

float GetDepth(float4 texCord, int i, int j)
{
	float4 newUV = texCord + float4(texCord.w*i*0.001f, texCord.w*j*0.001f, 0.0f, 0.0f);
	float4 depthColor = tex2Dproj(SampleShadowDepth, newUV);
	//float4 depthColor = tex2D(SampleShadowDepth, float2(newUV.x/texCord.w, newUV.y/texCord.w));
				
	return depthColor.r;
}

void p_std_lightshadow
(
    in float2 vertexTCoord0 : TEXCOORD0,
	in float2 vertexTCoord1 : TEXCOORD1,
	in float4 vertexTCoord2 : TEXCOORD2,
	in float4 vertexTCoord3 : TEXCOORD3,
    out float4 pixelColor : COLOR,
	uniform float4 UVOffset,
	uniform float4 FogColorHeight,
	uniform float4 FogColorDist
)
{
    // base
    float2 texCoord = vertexTCoord0;
    texCoord.y = 1.0 - vertexTCoord0.y;
	texCoord.xy += UVOffset.xy;
	float4 lastColor = tex2D(SampleBase, texCoord*UVOffset.zw);
	
	// shadow
	float4 texCord = vertexTCoord2;
	
	// shadow map depth
	float shadowDepth = GetDepth(texCord, 0, 0);
	shadowDepth += GetDepth(texCord, -1, -1);
	shadowDepth += GetDepth(texCord, -1, 0);
	//shadowDepth += GetDepth(texCord, -1, 1);
	//shadowDepth += GetDepth(texCord, 0, -1);
	//shadowDepth += GetDepth(texCord, 0, 1);
	//shadowDepth += GetDepth(texCord, 1, -1);
	//shadowDepth += GetDepth(texCord, 1, 0);
	//shadowDepth += GetDepth(texCord, 1, 1);
	//shadowDepth *= 0.1111f;
	shadowDepth *= 0.3333f;

	shadowDepth = clamp(shadowDepth, 0.5, 1.0);	
	lastColor.rgb *= shadowDepth;	
		
	if (lastColor.a < 0.25)
	{
		discard;
	}
	else
	{	
		// light
		lastColor *= vertexTCoord3;	
	
		// fog
		lastColor.rgb = lerp(FogColorHeight.rgb, lastColor.rgb, vertexTCoord1.x);
		lastColor.rgb = lerp(FogColorDist.rgb, lastColor.rgb, vertexTCoord1.y);
				
		pixelColor = lastColor;
	}
}
