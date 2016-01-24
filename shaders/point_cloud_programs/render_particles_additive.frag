#version 330 core

out vec4 color;

//uniform sampler2D accelerationSampler2D;
//uniform sampler2D velocitySampler2D;
//uniform sampler2D positionSampler2D;

uniform vec3 particle_color;

uniform float lightIntensity;
uniform vec3 lightColor;

in vec3 a;
in vec3 v;
in vec3 p;
in float t;
in float radius;

void main(){
	//float l = length(a);
	//float norm = 0.01;
	//float x = l/norm;
	//
	//vec3 c1 = vec3(0.2,0.2,1);
	//vec3 c2 = vec3(0.2,0.6,0.6);
	//vec3 c3 = vec3(1,0.5,0.5);
	//vec3 c4 = vec3(0.7,1,0.7);
	//vec3 c5 = vec3(1,1,0.5);
	//vec3 c6 = vec3(1,1,1);
	//
	//vec3 c = vec3(0,0,0);
	//	c1 * clamp(1 - x, 0, 1) + 
	//	c2 * clamp(min(2 - x, x), 0, 1) + 
	//	c3 * clamp(min(3 - x, x - 1), 0, 1) +
	//	c4 * clamp(min(4 - x, x - 2), 0, 1) +
	//	c5 * clamp(min(5 - x, x - 3), 0, 1) + 
	//	c6 * clamp(x - 4, 0, 1);

	vec2 coord = (gl_PointCoord - vec2(0.5))*2;  //from [0,1] to [-0.5,0.5]
	coord.y = -coord.y;
	if(length(coord) > 1)                  //outside of circle radius?
	    discard;
	
	color = vec4(particle_color, 0.1);
}