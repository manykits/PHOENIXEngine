attribute mediump vec3 modelPosition;
attribute mediump vec3 modelNormal;
attribute mediump vec2 modelTCoord0;
varying mediump vec4 vertexColor;
varying mediump vec2 vertexTCoord0;
varying mediump vec2 vertexTCoord1;
uniform mat4 PVWMatrix;
uniform mat4 WMatrix;
uniform vec4 CameraWorldPosition;
uniform vec4 LightWorldDVector_Dir;
uniform vec4 ShineEmissive;
uniform vec4 ShineAmbient;
uniform vec4 ShineDiffuse;
uniform vec4 ShineSpecular;	
uniform vec4 LightAmbient_Dir;
uniform vec4 LightDiffuse_Dir;
uniform vec4 LightSpecular_Dir;
uniform vec4 LightGroup[8];
uniform vec4 FogParam;

vec3 DoLight_Point_Diffuse(vec3 lightWorldPos, float lightRange, vec3 lightColor, vec3 shineDiffuse, vec3 vertexWorldPos, vec3 vertexWorldNormal)
{
	vec3 lightToVertex = lightWorldPos - vertexWorldPos;
	float squareDist = dot(lightToVertex, lightToVertex);
	lightToVertex = normalize(lightToVertex);
	return lightColor * shineDiffuse * max(0.0, dot(vertexWorldNormal, lightToVertex)) * max(0.0, (1.0 - squareDist / lightRange / lightRange) );
}

void main()
{
	gl_Position = PVWMatrix * vec4(modelPosition, 1.0);
	
	vertexTCoord0 = modelTCoord0;
	
	vec3 worldPosition = (WMatrix * vec4(modelPosition, 1.0)).xyz;
	vec4 col0 = WMatrix[0]; vec4 col1 = WMatrix[1]; vec4 col2 = WMatrix[2]; 
	mat3 worldMat = mat3(col0.xyz, col1.xyz, col2.xyz);
	vec3 worldNormal = normalize(worldMat * modelNormal);
	
	vec3 viewVector = normalize(CameraWorldPosition.xyz - worldPosition);
	float dist = sqrt((CameraWorldPosition.x - worldPosition.x)*(CameraWorldPosition.x - worldPosition.x) + (CameraWorldPosition.y - worldPosition.y)*(CameraWorldPosition.y - worldPosition.y) + (CameraWorldPosition.z - worldPosition.z)*(CameraWorldPosition.z - worldPosition.z));
	
	vec3 halfVector = normalize((viewVector - LightWorldDVector_Dir.xyz)/2.0);
	float dotH = dot(worldNormal, halfVector);
	
	vertexColor.rgb = ShineEmissive.rgb + LightAmbient_Dir.a * (ShineAmbient.rgb * LightAmbient_Dir.rgb + ShineDiffuse.rgb * LightDiffuse_Dir.rgb * max(dot(worldNormal, -LightWorldDVector_Dir.xyz), 0.0) + ShineSpecular.rgb * LightSpecular_Dir.rgb * pow(max(dotH, 0.0), ShineSpecular.a*LightSpecular_Dir.a));
	vertexColor.a = ShineEmissive.a;
	
	vertexColor.rgb += DoLight_Point_Diffuse(LightGroup[0].xyz, LightGroup[0].w, LightGroup[1].rgb, ShineDiffuse.rgb, worldPosition.xyz, worldNormal.xyz);
	vertexColor.rgb += DoLight_Point_Diffuse(LightGroup[2].xyz, LightGroup[2].w, LightGroup[3].rgb, ShineDiffuse.rgb, worldPosition.xyz, worldNormal.xyz);
	vertexColor.rgb += DoLight_Point_Diffuse(LightGroup[4].xyz, LightGroup[4].w, LightGroup[5].rgb, ShineDiffuse.rgb, worldPosition.xyz, worldNormal.xyz);
	vertexColor.rgb += DoLight_Point_Diffuse(LightGroup[6].xyz, LightGroup[6].w, LightGroup[7].rgb, ShineDiffuse.rgb, worldPosition.xyz, worldNormal.xyz);
	
	float fogValueHeight = (-FogParam.x + worldPosition.z)/(FogParam.y - FogParam.x);
	fogValueHeight = clamp(fogValueHeight, 0.0, 1.0);
	float fogValueDist = (FogParam.w - dist)/(FogParam.w - FogParam.z);
	fogValueDist = clamp(fogValueDist, 0.0, 1.0);
	
	vertexTCoord1.x = fogValueHeight;
	vertexTCoord1.y = fogValueDist;
}