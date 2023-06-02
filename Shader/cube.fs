#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

uniform vec3 color;

uniform bool sphere;

void main()
{    
    //FragColor = texture(texture_diffuse1, TexCoords);
    if(sphere){
        FragColor = vec4(color, 1.0);
    }
    else{
        FragColor = vec4(color, 0.2);
    }
}