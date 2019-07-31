uniform mat4 PVWMatrix;
attribute mediump vec3 modelPosition;
attribute mediump vec4 modelColor0;
attribute mediump vec2 modelTCoord0;
varying mediump vec4 vertexColor0;
varying mediump vec2 vertexTCoord0;
void main()
{
	gl_Position = PVWMatrix	* vec4(modelPosition, 1.0);
	vertexColor0 = modelColor0;
	vertexTCoord0 = modelTCoord0;
}