varying mediump vec2 vertexTCoord0;
varying mediump vec2 vertexTCoord1;
varying mediump vec4 vertexTCoord2;
varying mediump vec4 vertexTCoord3;
uniform mediump vec4 UVOffset;
uniform mediump vec4 FogColorHeight;
uniform mediump vec4 FogColorDist;
uniform sampler2D SampleBase;
uniform sampler2D SampleShadowDepth;

highp float GetDepth(mediump vec4 texCord, mediump float i, mediump float j)
{
	mediump vec4 newUV = texCord + vec4(texCord.w*i*0.001, texCord.w*j*0.001, 0.0, 0.0);
	highp float depthColor = texture2DProj(SampleShadowDepth, newUV).r;
				
	return depthColor;
}

void main()
{
	mediump vec2 texCoord = vec2(vertexTCoord0.x, 1.0-vertexTCoord0.y);
	texCoord.xy += UVOffset.xy;
	mediump vec4 lastColor = texture2D(SampleBase, texCoord*UVOffset.zw);
	
	highp vec4 texCord = vertexTCoord2;
	highp float depth = texCord.z/texCord.w;
	
	highp float shadowDepth = GetDepth(texCord, 0.0, 0.0);
	shadowDepth += GetDepth(texCord, -1.0, -1.0);
	shadowDepth += GetDepth(texCord, -1.0, 0.0);
	shadowDepth *= 0.3333;
	if ((depth-0.0006) > shadowDepth)
		lastColor.rgb *= 0.3;
		
	if (lastColor.a < 0.25)
	{
		discard;
	}
	else
	{	
		lastColor *= vertexTCoord3;	
		
		lastColor.rgb = lastColor.rgb * vertexTCoord1.x + FogColorHeight.rgb * (1.0 - vertexTCoord1.x);
		lastColor.rgb = lastColor.rgb * vertexTCoord1.y + FogColorDist.rgb * (1.0 - vertexTCoord1.y);
		
		gl_FragColor = lastColor;
	}
}