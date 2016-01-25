#version 330 core

out vec4 color;

// Rendering properties for particles
uniform vec3 particle_color;

// Lightsource properties
uniform float light_intensity;
uniform vec3 light_color;

void main(){
  vec2 coord = (gl_PointCoord - vec2(0.5))*2;  // From [0,1] to [-0.5,0.5]
  if(length(coord) > 1) // Outside of circle radius?
    discard;
  color = vec4(particle_color, 0.1);
}