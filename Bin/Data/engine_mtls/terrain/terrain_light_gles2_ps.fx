varying mediump vec4 vertexColor;
varying mediump vec2 vertexTCoord0;
varying mediump vec2 vertexTCoord1;
uniform mediump vec4 UVScale01;
uniform mediump vec4 UVScale23;
uniform mediump vec4 UVScale4;
uniform mediump vec4 FogColorHeight;
uniform mediump vec4 FogColorDist;
uniform sampler2D SamplerAlpha;
uniform sampler2D Sampler0;
uniform sampler2D Sampler1;
uniform sampler2D Sampler2;
uniform sampler2D Sampler3;
uniform sampler2D Sampler4;

mediump vec4 LerpColor(mediump vec4 color0, mediump vec4 color1, mediump float alpha)
{
	return color0 * (1.0-alpha) + color1 * alpha;
}

void main()
{
	mediump vec2 texCoord = vertexTCoord0;
	texCoord.y = 1.0 - vertexTCoord0.y;
	
	mediump vec4 colorAlpha = texture2D(SamplerAlpha, vertexTCoord0);
    mediump vec4 color0 = texture2D(Sampler0, texCoord*UVScale01.xy);
    mediump vec4 color1 = texture2D(Sampler1, texCoord*UVScale01.zw);
    mediump vec4 color2 = texture2D(Sampler2, texCoord*UVScale23.xy);
    mediump vec4 color3 = texture2D(Sampler3, texCoord*UVScale23.zw);
    mediump vec4 color4 = texture2D(Sampler4, texCoord*UVScale4.xy);
	
	mediump vec4 lastColor = LerpColor(color0 ,color1, colorAlpha.r);
	lastColor = LerpColor(lastColor ,color2, colorAlpha.g);
	lastColor = LerpColor(lastColor ,color3, colorAlpha.b);
	lastColor = LerpColor(lastColor ,color4, colorAlpha.a);
	
	lastColor *= vertexColor;	
	
	lastColor.rgb = lastColor.rgb * vertexTCoord1.x + FogColorHeight.rgb * (1.0 - vertexTCoord1.x);
	lastColor.rgb = lastColor.rgb * vertexTCoord1.y + FogColorDist.rgb * (1.0 - vertexTCoord1.y);
	
	gl_FragColor.rgb = lastColor.rgb;
	gl_FragColor.a = 1.0;
}