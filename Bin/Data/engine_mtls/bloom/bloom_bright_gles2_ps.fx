varying mediump vec2 vertexTCoord0;
uniform mediump vec4 BrightParam;
uniform sampler2D SamplerBase;
void main()
{
	mediump vec2 texCord = vec2(vertexTCoord0.x, 1.0-vertexTCoord0.y);
	mediump vec4 color = texture2D(SamplerBase, texCord);
	
	color.rgb -= normalize(color.rgb) * BrightParam.r;
	
	color = max(color, 0.0);
	
	color.rgb = color.rgb * color.rgb;
	
	gl_FragColor = color;
}