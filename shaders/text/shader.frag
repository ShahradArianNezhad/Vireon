#version 330 core

out vec4 FragColor;

in vec4 oColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main() {
    float alpha = texture(ourTexture, TexCoord).r;

    FragColor = vec4(1.0,1.0,1.0, alpha);

}
