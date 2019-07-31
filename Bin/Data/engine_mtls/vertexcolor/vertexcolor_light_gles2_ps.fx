varying mediump vec4 vertexTCoord0;
varying mediump vec2 vertexTCoord1;
uniform mediump vec4 FogColorHeight;
uniform mediump vec4 FogColorDist;

void main()
{
	mediump vec4 lastColor = vertexTCoord0;
	
	lastColor *= vertexTCoord2;	

	lastColor.rgb = lastColor.rgb * vertexTCoord1.x + FogColorHeight.rgb * (1.0 - vertexTCoord1.x);
	lastColor.rgb = lastColor.rgb * vertexTCoord1.y + FogColorDist.rgb * (1.0 - vertexTCoord1.y);

	gl_FragColor = lastColor;	
}