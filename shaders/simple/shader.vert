#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec4 oColor;

uniform mat4 model;
uniform mat4 projection;

void main()
{
  oColor = vec4(aColor, 1.0);
  gl_Position = projection * model * vec4(aPos, 1.0);
}
