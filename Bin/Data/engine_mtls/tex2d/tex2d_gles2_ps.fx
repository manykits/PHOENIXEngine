varying mediump vec2 vertexTCoord0;
uniform sampler2D SampleBase;
void main()
{
	mediump vec2 texCoord = vec2(vertexTCoord0.x, 1.0-vertexTCoord0.y);
	gl_FragColor = texture2D(SampleBase, texCoord);
}