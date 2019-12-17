#version 330 core

struct DirLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cut_off;
	float outer_cut_off;
	
	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// - vars

#define MAX_LIGHT 20

out vec4 FragColor;

in vec3 frag_position;
in vec3 normal;
in vec2 tex_coords;

uniform vec3 view_position;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform DirLight dir_lights[MAX_LIGHT];
uniform PointLight point_lights[MAX_LIGHT];
uniform SpotLight spot_lights[MAX_LIGHT];
uniform int directional_count;
uniform int point_count;
uniform int spot_count;


// - funcs

vec3 calculate_directional_light(DirLight light, vec3 normal, vec3 view_dir);
vec3 calculate_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);
vec3 calculate_spot_light(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);

void main ()
{
	vec3 norm = normalize(normal);
	vec3 view_dir = normalize(view_position - frag_position);
	vec3 result;
	
	// - directional lights
	for (int i = 0; i < directional_count; i++)
		result += calculate_directional_light(dir_lights[i], norm, view_dir);

	// - point lights
	for (int i = 0; i < point_count; i++)
		result += calculate_point_light(point_lights[i], norm, frag_position, view_dir);

	// - spot lights
	for (int i = 0; i < spot_count; i++)
		result += calculate_spot_light(spot_lights[i], norm, frag_position, view_dir);

	FragColor = vec4(result, 1.0);
}

vec3 calculate_directional_light(DirLight light, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(-light.direction);
	vec3 reflect_dir = reflect(-light_dir, normal);

	float diff_factor = max(dot(normal, light_dir), 0.0);
	float spec_factor = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0f);

	vec3 ambient = light.ambient * texture(texture_diffuse1, tex_coords).rgb;
	vec3 diffuse = light.diffuse * diff_factor * texture(texture_diffuse1, tex_coords).rgb;
	vec3 specular = light.specular * spec_factor * texture(texture_specular1, tex_coords).rgb;

	return (ambient + diffuse + specular);
}

vec3 calculate_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
	vec3 light_dir = normalize(light.position - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);

	float diff_factor = max(dot(normal, light_dir), 0.0);
	float spec_factor = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0f);
	float distance = length(light.position - frag_pos);
	float attenuation = 1.0 / (light.constant + distance * light.linear + distance * distance * light.quadratic);

	vec3 ambient = light.ambient * texture(texture_diffuse1, tex_coords).rgb;
	vec3 diffuse = light.diffuse * diff_factor * texture(texture_diffuse1, tex_coords).rgb;
	vec3 specular = light.specular * spec_factor * texture(texture_specular1, tex_coords).rgb;
	
	ambient *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calculate_spot_light(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    
    float diffuse_factor = max(dot(normal, light_dir), 0.0);
    float specular_factor = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0f);

	float distance = length(light.position - frag_pos);
	float attenuation = 1.0 / (light.constant + distance * light.linear + distance * distance * light.quadratic);

    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.cut_off - light.outer_cut_off;
    float intensity = clamp((theta - light.outer_cut_off) /epsilon, 0.0, 1.0);
    
	vec3 ambient = light.ambient * texture(texture_diffuse1, tex_coords).rgb;
	vec3 diffuse = light.diffuse * diffuse_factor * texture(texture_diffuse1, tex_coords).rgb;
	vec3 specular = light.specular * specular_factor * texture(texture_specular1, tex_coords).rgb;

	ambient *= attenuation * intensity;
	diffuse  *= attenuation * intensity;
	specular *= attenuation * intensity;

    return (ambient + diffuse + specular);

}