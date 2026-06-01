#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D SceneTexture;
uniform sampler2D LightTexture;
uniform float ambient;

void main()
{ 
  vec3 scene = texture(SceneTexture,TexCoords).rgb;
  vec3 light = texture(LightTexture,TexCoords).rgb;
  FragColor = vec4(scene*(ambient+light),1.0);
}

