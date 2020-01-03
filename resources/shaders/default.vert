#version 330 core

layout(location = 0) in vec3 _position;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _tex_coords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 normal;
out vec3 frag_position;
out vec2 tex_coords;

void main()
{
	frag_position = vec3(model * vec4(_position, 1.0));
	normal = _normal;
	tex_coords = _tex_coords;
	gl_Position = projection * view * model * vec4(_position, 1.0f);
}