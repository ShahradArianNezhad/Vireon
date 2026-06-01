#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 color;
layout (location = 3) in float radius;
layout (location = 4) in float intensity;
layout (location = 5) in mat4 model;

out vec2 TexCoords;
out vec2 localPos;
out vec3 ocolor;
out float oradius;
out float ointensity;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    ocolor=color;
    ointensity=intensity;
    oradius=radius;
    localPos=vec2(aPos[0],aPos[1]);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}  
