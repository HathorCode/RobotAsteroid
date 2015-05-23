#version 130

in vec2 texCoord;

uniform sampler2D textureSampler;

void main() {
	gl_FragColor = texture2D(textureSampler, texCoord);
}