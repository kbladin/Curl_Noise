#version 330 core

// Input data from vertex shader
in vec3 normal_viewspace;
in vec3 vertex_position_viewspace;
in vec3 light_position_viewspace;

// Fragment output
out vec4 color;

// Uniform data
// Material properties
uniform vec3  material_diffise_color;
uniform vec3  material_specularColor;
uniform float   material_specularity;
uniform int   material_shinyness;
// Light properties
uniform float   light_intensity;
uniform vec3  light_color;

void main(){
  // Light calculations
  vec3 light_to_position = vertex_position_viewspace - light_position_viewspace;
  vec3 l = normalize(light_to_position);
  vec3 v = normalize(vertex_position_viewspace);
  vec3 n = normalize(normal_viewspace);
  vec3 r = reflect(l,n);

  // Diffuse and reflected intensity
  float cos_theta = max(dot(-l,n),0);
  float cos_alpha = max(dot(v,-r),0);

  float distance = length(light_to_position);
  float inv_dist_square = 1 / pow(distance, 2);

  // Light components
  vec3 under_light = max(dot(-vec3(0,1,0),n),0) * material_diffise_color * 0.2;
  vec3 ambient = vec3(0.2,0.2,0.2) * material_diffise_color;
  vec3 diffuse =
    light_intensity * 
    cos_theta * 
    inv_dist_square *
    light_color * 
    material_diffise_color;
  vec3 specular =
    light_intensity *
    material_specularity *
    pow(cos_alpha, material_shinyness) *
    inv_dist_square *
    light_color *
    material_specularColor;

  color = vec4(ambient + diffuse + specular + under_light, 1);
}
