#version 330 core

// In data
in float age;

// Out data
out vec4 color;

// Uniform data
// Rendering properties for particles
uniform vec3 particle_color1;
uniform vec3 particle_color2;
// Lightsource properties
uniform float light_intensity;
uniform vec3 light_color;

void main(){
  vec2 coord = (gl_PointCoord - vec2(0.5))*2;  // From [0,1] to [-0.5,0.5]
  if(length(coord) > 1) // Outside of circle radius?
    discard;

  int n_colors = 3; // Number of colors to blend
  float x = age * (n_colors - 1); // Variable to determine color by blending

  // Transition from particle_color1 to particle_color2 and then fade out.
  vec4 c1 = vec4(particle_color1, 1);
  vec4 c2 = vec4(particle_color2, 1);
  vec4 c3 = vec4(particle_color2, 0);
  vec4 c =
    c1 * clamp(     1 - x,          0, 1) + 
    c2 * clamp(min( 2 - x, x),      0, 1) + 
    c3 * clamp(min( 3 - x, x - 1),  0, 1);
  color = c;
}