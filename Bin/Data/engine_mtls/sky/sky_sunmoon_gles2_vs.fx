attribute mediump vec3 modelPosition;
varying mediump vec3 vertexTCoord0;
uniform mat4 PVWMatrix;
uniform mat4 WMatrix;
uniform vec4 CameraWorldPosition;

void main() 
{
	gl_Position = PVWMatrix * vec4(modelPosition, 1.0);
	vec4 worldPosition = WMatrix * vec4(modelPosition, 1.0);
	vertexTCoord0 = worldPosition.xyz - CameraWorldPosition.xyz;
}