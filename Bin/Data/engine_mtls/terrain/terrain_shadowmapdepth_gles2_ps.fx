varying mediump vec2 vertexTCoord0;
void main()
{
	gl_FragColor = vec4(vertexTCoord0.r, vertexTCoord0.r,vertexTCoord0.r, 1.0);
}