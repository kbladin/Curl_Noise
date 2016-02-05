#version 330 core

// In data
// Lightsource properties
in vec3 light_position_viewspace;
in vec4 vertex_position_viewspace;

// Out data
out vec4 color;

// Uniform data
// Rendering properties for particles
uniform vec3 particle_color2;
// Lightsource properties
uniform float light_intensity;
uniform vec3 light_color;

// This shader renders a sphere with phong shading
void main(){
  vec2 coord = (gl_PointCoord - vec2(0.5))*2; // From [0,1] to [-0.5,0.5]
  coord.y = -coord.y; // y should point up for correct light
  if(length(coord) > 1) // Outside of circle radius?
      discard;

  // z_coord of sphere
  float z_coord = sqrt(1 - pow(length(coord),2));

  // Light calculations
  vec3 light_to_point =   vec3(vertex_position_viewspace) - light_position_viewspace;
  vec3 l =                normalize(light_to_point);
  vec3 n =                normalize(vec3(coord, z_coord));
  float cos_theta =       max(dot(-l,n),0);
  float distance =        length(light_to_point);
  float inv_dist_square = 1 / pow(distance, 2);

  // Light components
  vec3 under_light = max(dot(-vec3(0,1,0),n),0) * particle_color2 * 0.2;

  vec3 ambient = vec3(0.2,0.2,0.2) * particle_color2;

  vec3 diffuse =
    light_intensity * 
    cos_theta * 
    inv_dist_square *
    light_color * 
    particle_color2;

  // Final color
  color = vec4(ambient + diffuse + under_light, 1);
}