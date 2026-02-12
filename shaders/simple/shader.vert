#version 330 core
layout(location = 0) in vec3 aPos;

uniform vec4 iColor;
uniform mat4 model;
uniform mat4 projection;

out vec4 oColor;
void main()
{
  oColor=iColor;
  gl_Position = projection * model * vec4(aPos, 1.0);
}
