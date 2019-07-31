void v_skinskeleton
(
    in float3 modelPosition : POSITION,
	in float3 modelNormal : NORMAL,
    in float2 modelTCoord0 : TEXCOORD0,
	in float4 modelTCoord1 : TEXCOORD1, // indeces
	in float4 modelTCoord2 : TEXCOORD2, // wights
    out float4 clipPosition : POSITION,
    out float2 vertexTCoord0 : TEXCOORD0,
	out float2 vertexTCoord1 : TEXCOORD1,
	out float4 vertexTCoord2 : TEXCOORD2,
	uniform float4x4 PVWMatrix,
	uniform float4 CameraWorldPosition,
	uniform float4 LightWorldDVector_Dir,
	uniform float4 ShineEmissive,
	uniform float4 ShineAmbient,
	uniform float4 ShineDiffuse,
	uniform float4 ShineSpecular,
	uniform float4 LightAmbient_Dir,
	uniform float4 LightDiffuse_Dir,
	uniform float4 LightSpecular_Dir,
	uniform float4 FogParam,
	uniform float4 BoneTM[105]
)
{
	// Pos Normal
	int i0 = modelTCoord1[0]*3;
	int i1 = modelTCoord1[1]*3;
	int i2 = modelTCoord1[2]*3;
	int i3 = modelTCoord1[3]*3;
	float4 weight = modelTCoord2;
	
	float4 inputPos = float4(modelPosition, 1.0f);
	float3 worldPosition = float3(0, 0, 0);	
	worldPosition += float3(dot(inputPos, BoneTM[i0]), dot(inputPos, BoneTM[i0 + 1]), dot(inputPos, BoneTM[i0 + 2])) * weight[0];
	worldPosition += float3(dot(inputPos, BoneTM[i1]), dot(inputPos, BoneTM[i1 + 1]), dot(inputPos, BoneTM[i1 + 2])) * weight[1];
	worldPosition += float3(dot(inputPos, BoneTM[i2]), dot(inputPos, BoneTM[i2 + 1]), dot(inputPos, BoneTM[i2 + 2])) * weight[2];	
	worldPosition += float3(dot(inputPos, BoneTM[i3]), dot(inputPos, BoneTM[i3 + 1]), dot(inputPos, BoneTM[i3 + 2])) * weight[3];
	float3 worldNormal = float3(0, 0, 0);
	worldNormal += float3(dot(modelNormal, BoneTM[i0].xyz), dot(modelNormal, BoneTM[i0 + 1].xyz), dot(modelNormal, BoneTM[i0 + 2].xyz)) * weight[0];
	worldNormal += float3(dot(modelNormal, BoneTM[i1].xyz), dot(modelNormal, BoneTM[i1 + 1].xyz), dot(modelNormal, BoneTM[i1 + 2].xyz)) * weight[1];
	worldNormal += float3(dot(modelNormal, BoneTM[i2].xyz), dot(modelNormal, BoneTM[i2 + 1].xyz), dot(modelNormal, BoneTM[i2 + 2].xyz)) * weight[2];	
	worldNormal += float3(dot(modelNormal, BoneTM[i3].xyz), dot(modelNormal, BoneTM[i3 + 1].xyz), dot(modelNormal, BoneTM[i3 + 2].xyz)) * weight[3];
	worldNormal = normalize(worldNormal);
	
	clipPosition = mul(PVWMatrix, float4(worldPosition, 1.0f));
	
	// Tex Coord
	vertexTCoord0 = modelTCoord0;
	
	float3 viewVector = normalize(CameraWorldPosition.rgb - worldPosition);
	float dist = distance(CameraWorldPosition.rgb, worldPosition);
	
	// light
	float3 halfVector = normalize((viewVector - LightWorldDVector_Dir.rgb)/2.0);
	float dotH = dot(worldNormal, halfVector);
	
	vertexTCoord2.rgb = ShineEmissive.rgb + LightAmbient_Dir.a * (ShineAmbient.rgb * LightAmbient_Dir.rgb +
		ShineDiffuse.rgb * LightDiffuse_Dir.rgb * max(dot(worldNormal, -LightWorldDVector_Dir.rgb), 0) +
							ShineSpecular.rgb * LightSpecular_Dir.rgb * pow(max(dotH, 0), ShineSpecular.a*LightSpecular_Dir.a));		
	vertexTCoord2.a = ShineEmissive.a;
	
	// fog
	float fogValueHeight = (-FogParam.x + worldPosition.z)/(FogParam.y - FogParam.x);
	fogValueHeight = clamp(fogValueHeight, 0, 1.0);	
	float fogValueDist = (FogParam.w - dist)/(FogParam.w - FogParam.z);
	fogValueDist = clamp(fogValueDist, 0, 1.0);
	
	vertexTCoord1.x = fogValueHeight;
	vertexTCoord1.y = fogValueDist;
}

sampler2D SampleBase;

void p_skinskeleton
(
    in float2 vertexTCoord0 : TEXCOORD0,
	in float2 vertexTCoord1 : TEXCOORD1,
	in float4 vertexTCoord2 : TEXCOORD2,
    out float4 pixelColor : COLOR,
	uniform float4 UVOffset,
	uniform float4 FogColorHeight,
	uniform float4 FogColorDist
)
{
    float2 texCoord = vertexTCoord0;
    texCoord.y = 1.0 - vertexTCoord0.y;
	texCoord.xy += UVOffset.xy;
    float4 lastColor = tex2D(SampleBase, texCoord);
	
	if (lastColor.a < 0.25)
	{
		discard;
	}
	else
	{
		lastColor *= vertexTCoord2;
		
		lastColor.rgb = lerp(FogColorHeight.rgb, lastColor.rgb, vertexTCoord1.x);
		lastColor.rgb = lerp(FogColorDist.rgb, lastColor.rgb, vertexTCoord1.y);
		
		pixelColor = lastColor;
	}
}
