layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

void main()
{
	vec3 norm = normalize(gl_NormalMatrix * normal);
	vec3 color = vec3(1.0, 1.0, 1.0);

	vec3 lightDir = normalize(vec3(3.0, 6.0, 6.0));
	float NdotL = max(dot(normal, lightDir), 0.1);

	color *= NdotL;
	gl_FrontColor.xyz = color;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}