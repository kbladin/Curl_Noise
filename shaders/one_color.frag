#version 330 core

// Out data
out vec4 color;

// Uniform data
uniform vec3 material_diffise_color;

void main(){
  color.rgb = material_diffise_color;
  color.a = 1;
}
