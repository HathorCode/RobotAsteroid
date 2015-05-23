#version 130

in vec2 vertex;
in vec2 vertexTexCoord;

out vec2 texCoord;

uniform mat3 mdlViewMatrix;

void main() {
	gl_Position = vec4(mdlViewMatrix * vec3(vertex, 1.0), 1.0);
	gl_Position.z = 0.0;
	
	texCoord = vertexTexCoord;
}