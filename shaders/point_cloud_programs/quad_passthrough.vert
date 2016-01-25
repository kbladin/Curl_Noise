#version 330 core

layout(location = 0) in vec3 vertex_position_modelspace;

void main(){
	gl_Position = vec4(vertex_position_modelspace,1);
}