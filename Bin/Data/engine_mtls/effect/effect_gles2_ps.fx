varying mediump vec4 vertexColor0;
varying mediump vec2 vertexTCoord0;
uniform sampler2D Sample0;
void main()
{
	mediump vec2 tecCord = vec2(vertexTCoord0.x, 1.0-vertexTCoord0.y);
	mediump vec4 color = texture2D(Sample0, tecCord);
	gl_FragColor = color*vertexColor0;
}