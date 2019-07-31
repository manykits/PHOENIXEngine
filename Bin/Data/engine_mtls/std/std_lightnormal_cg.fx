float3 DoLight_Point_Diffuse(float3 lightWorldPos, float lightRange, float3 lightColor, float3 shineDiffuse, float3 vertexWorldPos, float3 vertexWorldNormal)
{
	float3 lightToVertex = lightWorldPos - vertexWorldPos;
	float squareDist = dot(lightToVertex, lightToVertex);
	lightToVertex = normalize(lightToVertex);
	return lightColor * shineDiffuse * max(0, dot(vertexWorldNormal, lightToVertex)) * max( 0, (1.0 - squareDist / lightRange / lightRange) );
}

void v_std_lightnormal
(
    in float3 modelPosition : POSITION,
	in float3 modelNormal : NORMAL,
    in float2 modelTCoord0 : TEXCOORD0,
	in float3 modelTCoord1 : TEXCOORD1, // Targent
	in float3 modelTCoord2 : TEXCOORD2, // Binormal
    out float4 clipPosition : POSITION,
    out float2 vertexTCoord0 : TEXCOORD0,
	out float2 vertexTCoord1 : TEXCOORD1, // fog
	out float3 vertexTCoord2 : TEXCOORD2, // World Targent 
	out float3 vertexTCoord3 : TEXCOORD3, // World Binormal
	out float3 vertexTCoord4 : TEXCOORD4, // World Normal
	out float4 vertexTCoord5 : TEXCOORD5, // World Position
    uniform float4x4 PVWMatrix,
	uniform float4x4 WMatrix,
	uniform float4 CameraWorldPosition,
	uniform float4 FogParam
)
{
    // Pos
    clipPosition = mul(PVWMatrix, float4(modelPosition,1.0f));

    // Tex Coord
    vertexTCoord0 = modelTCoord0;
	
	// params
	float4 worldPosition = mul(WMatrix, float4(modelPosition, 1.0f));	
	
	float3 worldNormal = normalize(mul((float3x3)WMatrix, modelNormal));  // World Normal 
	vertexTCoord2 = normalize(mul((float3x3)WMatrix, modelTCoord1)); // World Targent 
	vertexTCoord3 = normalize(mul((float3x3)WMatrix, modelTCoord2)); // World Binormal
	vertexTCoord4 = worldNormal;
	vertexTCoord5 = worldPosition;
	
	// fog
	float dist = distance(CameraWorldPosition.xyz, worldPosition.xyz);
	
	float fogValueHeight = (-FogParam.x + worldPosition.z)/(FogParam.y - FogParam.x);
	fogValueHeight = clamp(fogValueHeight, 0, 1.0);	
	float fogValueDist = (FogParam.w - dist)/(FogParam.w - FogParam.z);
	fogValueDist = clamp(fogValueDist, 0, 1.0);
	
	vertexTCoord1.x = fogValueHeight;
	vertexTCoord1.y = fogValueDist;
}

sampler2D SampleBase;
sampler2D SampleNormal;

void p_std_lightnormal
(
    in float2 vertexTCoord0 : TEXCOORD0,
	in float2 vertexTCoord1 : TEXCOORD1,
	in float3 vertexTCoord2 : TEXCOORD2, // World Targent 
	in float3 vertexTCoord3 : TEXCOORD3, // World Binormal 
	in float3 vertexTCoord4 : TEXCOORD4, // World Normal 
	in float4 vertexTCoord5 : TEXCOORD5, // World Positon 
    out float4 pixelColor : COLOR,
	uniform float4 UVOffset,
	uniform float4 FogColorHeight,
	uniform float4 FogColorDist,	
	uniform float4 CameraWorldPosition,
	uniform float4 LightWorldDVector_Dir,
	uniform float4 ShineEmissive,
	uniform float4 ShineAmbient,
	uniform float4 ShineDiffuse,
	uniform float4 ShineSpecular,	
	uniform float4 LightAmbient_Dir,
	uniform float4 LightDiffuse_Dir,
	uniform float4 LightSpecular_Dir
)
{
    float2 texCoord = vertexTCoord0;
    texCoord.y = 1.0 - vertexTCoord0.y;
	texCoord.xy += UVOffset.xy;
	float4 lastColor = tex2D(SampleBase, texCoord*UVOffset.zw);
	
	// normal
	float3 normalMap = tex2D(SampleNormal, texCoord).rgb * 2.0 - 1.0;
	
	float3x3 matTargentSpace;
	matTargentSpace[0] = normalize(vertexTCoord2);
	matTargentSpace[1] = normalize(vertexTCoord3);
	matTargentSpace[2] = normalize(vertexTCoord4);
	
	float3 worldNormal = mul(normalMap, matTargentSpace);
	float3 worldPosition = vertexTCoord5.xyz;
	
	// dot diffuse
	float3 viewVector = normalize(CameraWorldPosition.xyz - worldPosition.xyz);
	
	// light
	float3 lightWorldDir = normalize(LightWorldDVector_Dir.xyz);
	float3 halfVector = normalize((viewVector - lightWorldDir.xyz)/2.0);
	float dotH = saturate(dot(worldNormal, halfVector));		
	float difusseDot = saturate(dot(worldNormal, -lightWorldDir.xyz));
	
	float4 lightColor = float4(0.0, 0.0, 0.0, 0.0);
	lightColor.rgb = ShineEmissive.rgb + LightAmbient_Dir.a * (ShineAmbient.rgb * LightAmbient_Dir.rgb +
		ShineDiffuse.rgb * LightDiffuse_Dir.rgb * difusseDot +
							ShineSpecular.rgb * LightSpecular_Dir.rgb * pow(dotH, ShineSpecular.a*LightSpecular_Dir.a));		
	lightColor.a = ShineEmissive.a;
	
	if (lastColor.a < 0.25)
	{
		discard;
	}
	else
	{
		lastColor *= lightColor;
	
		lastColor.rgb = lerp(FogColorHeight.rgb, lastColor.rgb, vertexTCoord1.x);
		lastColor.rgb = lerp(FogColorDist.rgb, lastColor.rgb, vertexTCoord1.y);
	
		pixelColor = lastColor;
	}
}
