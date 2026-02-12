#version 330 core
in vec4 oColor;
out vec4 FragColor;

uniform vec4 iTint;

void main()
{
  FragColor = oColor*iTint;
}
