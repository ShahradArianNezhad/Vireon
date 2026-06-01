#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;
in vec3 ocolor;
in vec2 localPos;
in float oradius;
in float ointensity;

uniform sampler2D screenTexture;

void main()
{ 
  float dist = length(localPos);
  if(dist>1.0) discard;
  float att=max(0.0,1.0-dist);
  att*=att;
  vec3 light = ocolor * att * ointensity;
    FragColor = vec4(light,att);
}

