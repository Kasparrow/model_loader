#version 330 core

out vec4 FragColor;

in vec3 frag_position;
in vec3 normal;
in vec2 tex_coords;

uniform sampler2D texture_diffuse1;

void main ()
{
    FragColor = texture(texture_diffuse1, tex_coords).rgba;

    if (FragColor.a < 0.1f)
        discard;
        
}