#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 iTint;
uniform sampler2D ourTexture;

void main() {
  FragColor = texture(ourTexture, TexCoord)*iTint;
}
