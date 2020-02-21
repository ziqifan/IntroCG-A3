#version 420

in vec3 norm;
in vec3 pos;

uniform samplerCube uCubeMap;

out vec4 outColor;

void main()
{
	outColor = texture(uCubeMap,normalize(norm));
}
