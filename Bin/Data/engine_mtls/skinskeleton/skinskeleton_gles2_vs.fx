attribute mediump vec3 modelPosition;
attribute mediump vec3 modelNormal;
attribute mediump vec2 modelTCoord0;
attribute mediump vec4 modelTCoord1;
attribute mediump vec4 modelTCoord2;
varying mediump vec2 vertexTCoord0;
varying mediump vec2 vertexTCoord1;
varying mediump vec4 vertexTCoord2;
uniform mat4 PVWMatrix;
uniform vec4 BoneTM[108];
uniform vec4 CameraWorldPosition;
uniform vec4 LightWorldDVector_Dir;
uniform vec4 ShineEmissive;
uniform vec4 ShineAmbient;
uniform vec4 ShineDiffuse;
uniform vec4 ShineSpecular;
uniform vec4 LightAmbient_Dir;
uniform vec4 LightDiffuse_Dir;
uniform vec4 LightSpecular_Dir;
uniform vec4 FogParam;

void main()
{
	int i0 = int(modelTCoord1[0])*3;
	int i1 = int(modelTCoord1[1])*3;
	int i2 = int(modelTCoord1[2])*3;
	int i3 = int(modelTCoord1[3])*3;
	
	vec4 inputPos = vec4(modelPosition.x, modelPosition.y, modelPosition.z, 1.0);
	vec3 worldPosition = vec3(0, 0, 0);
	worldPosition += vec3(dot(inputPos, BoneTM[i0]), dot(inputPos, BoneTM[i0 + 1]), dot(inputPos, BoneTM[i0 + 2])) * modelTCoord2[0];
	worldPosition += vec3(dot(inputPos, BoneTM[i1]), dot(inputPos, BoneTM[i1 + 1]), dot(inputPos, BoneTM[i1 + 2])) * modelTCoord2[1];
	worldPosition += vec3(dot(inputPos, BoneTM[i2]), dot(inputPos, BoneTM[i2 + 1]), dot(inputPos, BoneTM[i2 + 2])) * modelTCoord2[2];
	worldPosition += vec3(dot(inputPos, BoneTM[i3]), dot(inputPos, BoneTM[i3 + 1]), dot(inputPos, BoneTM[i3 + 2])) * modelTCoord2[3];
	vec3 worldNormal = vec3(0, 0, 0);
	worldNormal += vec3(dot(modelNormal, BoneTM[i0].xyz), dot(modelNormal, BoneTM[i0 + 1].xyz), dot(modelNormal, BoneTM[i0 + 2].xyz)) * modelTCoord2[0];
	worldNormal += vec3(dot(modelNormal, BoneTM[i1].xyz), dot(modelNormal, BoneTM[i1 + 1].xyz), dot(modelNormal, BoneTM[i1 + 2].xyz)) * modelTCoord2[1];
	worldNormal += vec3(dot(modelNormal, BoneTM[i2].xyz), dot(modelNormal, BoneTM[i2 + 1].xyz), dot(modelNormal, BoneTM[i2 + 2].xyz)) * modelTCoord2[2];	
	worldNormal += vec3(dot(modelNormal, BoneTM[i3].xyz), dot(modelNormal, BoneTM[i3 + 1].xyz), dot(modelNormal, BoneTM[i3 + 2].xyz)) * modelTCoord2[3];
	worldNormal = normalize(worldNormal);
	
	gl_Position = PVWMatrix * vec4(worldPosition, 1.0);
	
	vertexTCoord0 = modelTCoord0;
	
	vec3 viewVector = normalize(CameraWorldPosition.xyz - worldPosition);
	float dist = sqrt((CameraWorldPosition.x - worldPosition.x)*(CameraWorldPosition.x - worldPosition.x) + (CameraWorldPosition.y - worldPosition.y)*(CameraWorldPosition.y - worldPosition.y) + (CameraWorldPosition.z - worldPosition.z)*(CameraWorldPosition.z - worldPosition.z));
	vec3 halfVector = normalize((viewVector - LightWorldDVector_Dir.xyz)/2.0);
	float dotH = dot(worldNormal, halfVector);
	
	vertexTCoord2.rgb = ShineEmissive.rgb + LightAmbient_Dir.a * (ShineAmbient.rgb * LightAmbient_Dir.rgb + ShineDiffuse.rgb * LightDiffuse_Dir.rgb * max(dot(worldNormal, -LightWorldDVector_Dir.xyz), 0.0) + ShineSpecular.rgb * LightSpecular_Dir.rgb * pow(max(dotH, 0.0), ShineSpecular.a*LightSpecular_Dir.a));
	vertexTCoord2.a = ShineEmissive.a;
	
	float fogValueHeight = (-FogParam.x + worldPosition.z)/(FogParam.y - FogParam.x);
	fogValueHeight = clamp(fogValueHeight, 0.0, 1.0);
	float fogValueDist = (FogParam.w - dist)/(FogParam.w - FogParam.z);
	fogValueDist = clamp(fogValueDist, 0.0, 1.0);
	
	vertexTCoord1.x = fogValueHeight;
	vertexTCoord1.y = fogValueDist;
}