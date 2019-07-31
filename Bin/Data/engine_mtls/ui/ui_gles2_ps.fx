uniform mediump vec4 UVParam;
uniform mediump vec4 ShineEmissive;
varying mediump vec4 vertexColor0;
varying mediump vec2 vertexTCoord0;
uniform sampler2D SamplerBase;
void main()
{
	mediump vec2 texCord = vec2(vertexTCoord0.x, 1.0-vertexTCoord0.y)*UVParam.xy;
	mediump vec4 color = texture2D(SamplerBase, texCord);
	gl_FragColor = color*vertexColor0*ShineEmissive;
}