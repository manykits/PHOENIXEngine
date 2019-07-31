varying mediump vec2 vertexTCoord0;
varying mediump vec2 vertexTCoord1;
uniform sampler2D SampleBase;
void main()
{
	mediump vec2 texCoord = vec2(vertexTCoord0.x, 1.0-vertexTCoord0.y);
	mediump vec4 texColor = texture2D(SampleBase, texCoord);
	
	if (texColor.a < 0.25)
	{
		discard;
	}
	else
	{
		gl_FragColor = vec4(vertexTCoord1.r, vertexTCoord1.r,vertexTCoord1.r, 1.0);
	}
}