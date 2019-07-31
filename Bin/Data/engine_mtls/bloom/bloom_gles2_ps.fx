varying mediump vec2 vertexTCoord0;
uniform mediump vec4 BloomParam;
uniform sampler2D SamplerBase;
uniform sampler2D SamplerBloom;
void main()
{
	mediump vec2 uv = vec2(vertexTCoord0.x, 1.0-vertexTCoord0.y);
	
	mediump vec4 baseColor = texture2D(SamplerBase, uv);
	mediump vec4 bloomColor = texture2D(SamplerBloom, uv);
	
	gl_FragColor = baseColor + bloomColor*BloomParam.r;
}