// float4 SkyParam = float4(50.0f, 2.0f, 0.0f, 0.0f); 		// hazeTopAltitude, dayToSunsetSharpness
// float4 SunMoonParam = float4(512.0f, 0.2f, 400.0f, 0.2f); 	// sunRadiusAttenuation,sunLightness,largeSunRadiusAttenuation,largeSunLightness

void v_sky_sunmoon
(
	in float3 modelPosition : POSITION,
	out float4 clipPosition : POSITION,
	out float3 vertexTCoord0 : TEXCOORD0,
	uniform float4x4 PVWMatrix,
	uniform float4x4 WMatrix,
	uniform float4 CameraWorldPosition
)
{	
	clipPosition = mul(PVWMatrix, float4(modelPosition, 1.0f));
	float4 worldPosition = mul(WMatrix, float4(modelPosition, 1.0f));
	vertexTCoord0 = worldPosition.xyz - CameraWorldPosition.xyz;
}

sampler2D SamplerDay;
sampler2D SamplerSunset;
sampler2D SamplerNight;

void p_sky_sunmoon
(
	in float3 vertexTCoord0 : TEXCOORD0,
	out float4 pixelColor : COLOR,
	uniform float4 LightWorldDVector_Dir,
	uniform float4 CameraWorldDVector,
	uniform float4 SkyParam,
	uniform float4 SunMoonParam,
	uniform float4 SunMoonColourIn,
	uniform float4 SunMoonColourOut
)
{
	float3 camToVertex = normalize(vertexTCoord0);
	
	float3 flatLightVec = normalize(float3(-LightWorldDVector_Dir.x, -LightWorldDVector_Dir.y, 0.0f));
	float3 flatCameraVec = normalize(float3(CameraWorldDVector.x, CameraWorldDVector.y, 0.0f));
	float lcDot = dot(flatLightVec, flatCameraVec);
	float u =  1.0f - (lcDot + 1.0f) * 0.5f;
	
	float val = lerp(0.25, 1.25, min(1, SkyParam[0] / max(0.0001, -LightWorldDVector_Dir.z)));	
	float yAngle = pow(max(0, camToVertex.z), val);	
	float v =  1.0f - yAngle;
	
	float4 colorDay = tex2D(SamplerDay, float2(u, v));
	float4 colorSunSet = tex2D(SamplerSunset, float2(u, v));
	float4 colorNight = tex2D(SamplerNight, float2(u, v));
	
	float4 color = float4(0,0,0,1);
	if (LightWorldDVector_Dir.z < 0.0)
		color = lerp(colorDay, colorSunSet, min(1, pow(1 + LightWorldDVector_Dir.z, SkyParam[1])));
	else 
		color = lerp(colorSunSet, colorNight, min(1, LightWorldDVector_Dir.z * 4));
	
	float sunHighLight = pow(max(0, dot(camToVertex, -LightWorldDVector_Dir.xyz)), SunMoonParam[0]) * SunMoonParam[1];
	float largeSunHighLight = pow(max(0, dot(camToVertex, -LightWorldDVector_Dir.xyz)), SunMoonParam[2]) * SunMoonParam[3];
	color +=  SunMoonColourIn * sunHighLight + SunMoonColourOut * largeSunHighLight;
	
	pixelColor = color;
}
