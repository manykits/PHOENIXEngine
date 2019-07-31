varying mediump vec3 vertexTCoord0;
uniform mediump vec4 LightWorldDVector_Dir;
uniform mediump vec4 CameraWorldDVector;
uniform mediump vec4 SkyParam;
uniform mediump vec4 SunMoonParam;
uniform mediump vec4 SunMoonColourIn;
uniform mediump vec4 SunMoonColourOut;

uniform sampler2D SamplerDay;
uniform sampler2D SamplerSunset;
uniform sampler2D SamplerNight;

mediump float LerpFloat(mediump float val0, mediump float val1, mediump float alpha)
{
	return val0 * (1.0 - alpha) + val1 * alpha;
}

mediump vec4 LerpColor(mediump vec4 color0, mediump vec4 color1, mediump float alpha)
{
	return color0 * (1.0-alpha) + color1 * alpha;
}

void main()
{
	mediump vec3 camToVertex = normalize(vertexTCoord0);
	
	mediump vec3 flatLightVec = normalize(vec3(-LightWorldDVector_Dir.x, -LightWorldDVector_Dir.y, 0.0));
	mediump vec3 flatCameraVec = normalize(vec3(CameraWorldDVector.x, CameraWorldDVector.y, 0.0));
	mediump float lcDot = dot(flatLightVec, flatCameraVec);
	mediump float u =  1.0 - (lcDot + 1.0) * 0.5;
	
	mediump float val = LerpFloat(0.25, 1.25, min(1.0, SkyParam[0] / max(0.0001, -LightWorldDVector_Dir.z)));	
	mediump float yAngle = pow(max(0.0, camToVertex.z), val);	
	mediump float v =  1.0 - yAngle;
	
	mediump vec4 colorDay = texture2D(SamplerDay, vec2(u, v));
	mediump vec4 colorSunSet = texture2D(SamplerSunset, vec2(u, v));
	mediump vec4 colorNight = texture2D(SamplerNight, vec2(u, v));
	
	mediump vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
	if (LightWorldDVector_Dir.z < 0.0)
		color = LerpColor(colorDay, colorSunSet, min(1.0, pow((1.0 + LightWorldDVector_Dir.z), SkyParam[1])));
	else
		color = LerpColor(colorSunSet, colorNight, min(1.0, LightWorldDVector_Dir.z * 4.0));
	
	mediump float sunHighLight = pow(max(0.0, dot(camToVertex, -LightWorldDVector_Dir.xyz)), SunMoonParam[0]) * SunMoonParam[1];	
	mediump float largeSunHighLight = pow(max(0.0, dot(camToVertex, -LightWorldDVector_Dir.xyz)), SunMoonParam[2]) * SunMoonParam[3];
	color +=  SunMoonColourIn * sunHighLight + SunMoonColourOut * largeSunHighLight;
		
	gl_FragColor = color;
}