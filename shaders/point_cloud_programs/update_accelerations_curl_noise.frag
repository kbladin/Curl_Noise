#version 330 core
















// Cellular noise ("Worley noise") in 3D in GLSL.
// Copyright (c) Stefan Gustavson 2011-04-19. All rights reserved.
// This code is released under the conditions of the MIT license.
// See LICENSE file for details.

// Permutation polynomial: (34x^2 + x) mod 289
vec3 permute(vec3 x) {
  return mod((34.0 * x + 1.0) * x, 289.0);
}

// Cellular noise, returning F1 and F2 in a vec2.
// 3x3x3 search region for good F2 everywhere, but a lot
// slower than the 2x2x2 version.
// The code below is a bit scary even to its author,
// but it has at least half decent performance on a
// modern GPU. In any case, it beats any software
// implementation of Worley noise hands down.

vec2 cellular(vec3 P) {
#define K 0.142857142857 // 1/7
#define Ko 0.428571428571 // 1/2-K/2
#define K2 0.020408163265306 // 1/(7*7)
#define Kz 0.166666666667 // 1/6
#define Kzo 0.416666666667 // 1/2-1/6*2
#define jitter 1.0 // smaller jitter gives more regular pattern

  vec3 Pi = mod(floor(P), 289.0);
  vec3 Pf = fract(P) - 0.5;

  vec3 Pfx = Pf.x + vec3(1.0, 0.0, -1.0);
  vec3 Pfy = Pf.y + vec3(1.0, 0.0, -1.0);
  vec3 Pfz = Pf.z + vec3(1.0, 0.0, -1.0);

  vec3 p = permute(Pi.x + vec3(-1.0, 0.0, 1.0));
  vec3 p1 = permute(p + Pi.y - 1.0);
  vec3 p2 = permute(p + Pi.y);
  vec3 p3 = permute(p + Pi.y + 1.0);

  vec3 p11 = permute(p1 + Pi.z - 1.0);
  vec3 p12 = permute(p1 + Pi.z);
  vec3 p13 = permute(p1 + Pi.z + 1.0);

  vec3 p21 = permute(p2 + Pi.z - 1.0);
  vec3 p22 = permute(p2 + Pi.z);
  vec3 p23 = permute(p2 + Pi.z + 1.0);

  vec3 p31 = permute(p3 + Pi.z - 1.0);
  vec3 p32 = permute(p3 + Pi.z);
  vec3 p33 = permute(p3 + Pi.z + 1.0);

  vec3 ox11 = fract(p11*K) - Ko;
  vec3 oy11 = mod(floor(p11*K), 7.0)*K - Ko;
  vec3 oz11 = floor(p11*K2)*Kz - Kzo; // p11 < 289 guaranteed

  vec3 ox12 = fract(p12*K) - Ko;
  vec3 oy12 = mod(floor(p12*K), 7.0)*K - Ko;
  vec3 oz12 = floor(p12*K2)*Kz - Kzo;

  vec3 ox13 = fract(p13*K) - Ko;
  vec3 oy13 = mod(floor(p13*K), 7.0)*K - Ko;
  vec3 oz13 = floor(p13*K2)*Kz - Kzo;

  vec3 ox21 = fract(p21*K) - Ko;
  vec3 oy21 = mod(floor(p21*K), 7.0)*K - Ko;
  vec3 oz21 = floor(p21*K2)*Kz - Kzo;

  vec3 ox22 = fract(p22*K) - Ko;
  vec3 oy22 = mod(floor(p22*K), 7.0)*K - Ko;
  vec3 oz22 = floor(p22*K2)*Kz - Kzo;

  vec3 ox23 = fract(p23*K) - Ko;
  vec3 oy23 = mod(floor(p23*K), 7.0)*K - Ko;
  vec3 oz23 = floor(p23*K2)*Kz - Kzo;

  vec3 ox31 = fract(p31*K) - Ko;
  vec3 oy31 = mod(floor(p31*K), 7.0)*K - Ko;
  vec3 oz31 = floor(p31*K2)*Kz - Kzo;

  vec3 ox32 = fract(p32*K) - Ko;
  vec3 oy32 = mod(floor(p32*K), 7.0)*K - Ko;
  vec3 oz32 = floor(p32*K2)*Kz - Kzo;

  vec3 ox33 = fract(p33*K) - Ko;
  vec3 oy33 = mod(floor(p33*K), 7.0)*K - Ko;
  vec3 oz33 = floor(p33*K2)*Kz - Kzo;

  vec3 dx11 = Pfx + jitter*ox11;
  vec3 dy11 = Pfy.x + jitter*oy11;
  vec3 dz11 = Pfz.x + jitter*oz11;

  vec3 dx12 = Pfx + jitter*ox12;
  vec3 dy12 = Pfy.x + jitter*oy12;
  vec3 dz12 = Pfz.y + jitter*oz12;

  vec3 dx13 = Pfx + jitter*ox13;
  vec3 dy13 = Pfy.x + jitter*oy13;
  vec3 dz13 = Pfz.z + jitter*oz13;

  vec3 dx21 = Pfx + jitter*ox21;
  vec3 dy21 = Pfy.y + jitter*oy21;
  vec3 dz21 = Pfz.x + jitter*oz21;

  vec3 dx22 = Pfx + jitter*ox22;
  vec3 dy22 = Pfy.y + jitter*oy22;
  vec3 dz22 = Pfz.y + jitter*oz22;

  vec3 dx23 = Pfx + jitter*ox23;
  vec3 dy23 = Pfy.y + jitter*oy23;
  vec3 dz23 = Pfz.z + jitter*oz23;

  vec3 dx31 = Pfx + jitter*ox31;
  vec3 dy31 = Pfy.z + jitter*oy31;
  vec3 dz31 = Pfz.x + jitter*oz31;

  vec3 dx32 = Pfx + jitter*ox32;
  vec3 dy32 = Pfy.z + jitter*oy32;
  vec3 dz32 = Pfz.y + jitter*oz32;

  vec3 dx33 = Pfx + jitter*ox33;
  vec3 dy33 = Pfy.z + jitter*oy33;
  vec3 dz33 = Pfz.z + jitter*oz33;

  vec3 d11 = dx11 * dx11 + dy11 * dy11 + dz11 * dz11;
  vec3 d12 = dx12 * dx12 + dy12 * dy12 + dz12 * dz12;
  vec3 d13 = dx13 * dx13 + dy13 * dy13 + dz13 * dz13;
  vec3 d21 = dx21 * dx21 + dy21 * dy21 + dz21 * dz21;
  vec3 d22 = dx22 * dx22 + dy22 * dy22 + dz22 * dz22;
  vec3 d23 = dx23 * dx23 + dy23 * dy23 + dz23 * dz23;
  vec3 d31 = dx31 * dx31 + dy31 * dy31 + dz31 * dz31;
  vec3 d32 = dx32 * dx32 + dy32 * dy32 + dz32 * dz32;
  vec3 d33 = dx33 * dx33 + dy33 * dy33 + dz33 * dz33;

  // Sort out the two smallest distances (F1, F2)
#if 0
  // Cheat and sort out only F1
  vec3 d1 = min(min(d11,d12), d13);
  vec3 d2 = min(min(d21,d22), d23);
  vec3 d3 = min(min(d31,d32), d33);
  vec3 d = min(min(d1,d2), d3);
  d.x = min(min(d.x,d.y),d.z);
  return sqrt(d.xx); // F1 duplicated, no F2 computed
#else
  // Do it right and sort out both F1 and F2
  vec3 d1a = min(d11, d12);
  d12 = max(d11, d12);
  d11 = min(d1a, d13); // Smallest now not in d12 or d13
  d13 = max(d1a, d13);
  d12 = min(d12, d13); // 2nd smallest now not in d13
  vec3 d2a = min(d21, d22);
  d22 = max(d21, d22);
  d21 = min(d2a, d23); // Smallest now not in d22 or d23
  d23 = max(d2a, d23);
  d22 = min(d22, d23); // 2nd smallest now not in d23
  vec3 d3a = min(d31, d32);
  d32 = max(d31, d32);
  d31 = min(d3a, d33); // Smallest now not in d32 or d33
  d33 = max(d3a, d33);
  d32 = min(d32, d33); // 2nd smallest now not in d33
  vec3 da = min(d11, d21);
  d21 = max(d11, d21);
  d11 = min(da, d31); // Smallest now in d11
  d31 = max(da, d31); // 2nd smallest now not in d31
  d11.xy = (d11.x < d11.y) ? d11.xy : d11.yx;
  d11.xz = (d11.x < d11.z) ? d11.xz : d11.zx; // d11.x now smallest
  d12 = min(d12, d21); // 2nd smallest now not in d21
  d12 = min(d12, d22); // nor in d22
  d12 = min(d12, d31); // nor in d31
  d12 = min(d12, d32); // nor in d32
  d11.yz = min(d11.yz,d12.xy); // nor in d12.yz
  d11.y = min(d11.y,d12.z); // Only two more to go
  d11.y = min(d11.y,d11.z); // Done! (Phew!)
  return sqrt(d11.xy); // F1, F2
#endif
}



















// Simplex Noise

//
// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

#define RAMP(r) r >= 1.0f ? 1.0f : (r <= -1.0f ? -1.0f : 15.0f/8.0f * r - 10.0f / 8.0f * pow(r,3) + 3.0f / 8.0f * pow(r,5) )

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0; }

float mod289(float x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0; }

vec4 permute(vec4 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float permute(float x) {
  return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float taylorInvSqrt(float r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

vec4 grad4(float j, vec4 ip)
{
  const vec4 ones = vec4(1.0, 1.0, 1.0, -1.0);
  vec4 p,s;

  p.xyz = floor( fract (vec3(j) * ip.xyz) * 7.0) * ip.z - 1.0;
  p.w = 1.5 - dot(abs(p.xyz), ones.xyz);
  s = vec4(lessThan(p, vec4(0.0)));
  p.xyz = p.xyz + (s.xyz*2.0 - 1.0) * s.www; 

  return p;
}
            
// (sqrt(5) - 1)/4 = F4, used once below
#define F4 0.309016994374947451

float snoise(vec4 v)
{
  const vec4  C = vec4( 0.138196601125011,  // (5 - sqrt(5))/20  G4
                        0.276393202250021,  // 2 * G4
                        0.414589803375032,  // 3 * G4
                       -0.447213595499958); // -1 + 4 * G4

  // First corner
  vec4 i  = floor(v + dot(v, vec4(F4)) );
  vec4 x0 = v -   i + dot(i, C.xxxx);

  // Other corners

  // Rank sorting originally contributed by Bill Licea-Kane, AMD (formerly ATI)
  vec4 i0;
  vec3 isX = step( x0.yzw, x0.xxx );
  vec3 isYZ = step( x0.zww, x0.yyz );
  //  i0.x = dot( isX, vec3( 1.0 ) );
  i0.x = isX.x + isX.y + isX.z;
  i0.yzw = 1.0 - isX;
  //  i0.y += dot( isYZ.xy, vec2( 1.0 ) );
  i0.y += isYZ.x + isYZ.y;
  i0.zw += 1.0 - isYZ.xy;
  i0.z += isYZ.z;
  i0.w += 1.0 - isYZ.z;

  // i0 now contains the unique values 0,1,2,3 in each channel
  vec4 i3 = clamp( i0, 0.0, 1.0 );
  vec4 i2 = clamp( i0-1.0, 0.0, 1.0 );
  vec4 i1 = clamp( i0-2.0, 0.0, 1.0 );

  //  x0 = x0 - 0.0 + 0.0 * C.xxxx
  //  x1 = x0 - i1  + 1.0 * C.xxxx
  //  x2 = x0 - i2  + 2.0 * C.xxxx
  //  x3 = x0 - i3  + 3.0 * C.xxxx
  //  x4 = x0 - 1.0 + 4.0 * C.xxxx
  vec4 x1 = x0 - i1 + C.xxxx;
  vec4 x2 = x0 - i2 + C.yyyy;
  vec4 x3 = x0 - i3 + C.zzzz;
  vec4 x4 = x0 + C.wwww;

  // Permutations
  i = mod289(i); 
  float j0 = permute( permute( permute( permute(i.w) + i.z) + i.y) + i.x);
  vec4 j1 = permute( permute( permute( permute (
             i.w + vec4(i1.w, i2.w, i3.w, 1.0 ))
           + i.z + vec4(i1.z, i2.z, i3.z, 1.0 ))
           + i.y + vec4(i1.y, i2.y, i3.y, 1.0 ))
           + i.x + vec4(i1.x, i2.x, i3.x, 1.0 ));

  // Gradients: 7x7x6 points over a cube, mapped onto a 4-cross polytope
  // 7*7*6 = 294, which is close to the ring size 17*17 = 289.
  vec4 ip = vec4(1.0/294.0, 1.0/49.0, 1.0/7.0, 0.0) ;

  vec4 p0 = grad4(j0,   ip);
  vec4 p1 = grad4(j1.x, ip);
  vec4 p2 = grad4(j1.y, ip);
  vec4 p3 = grad4(j1.z, ip);
  vec4 p4 = grad4(j1.w, ip);

  // Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;
  p4 *= taylorInvSqrt(dot(p4,p4));

  // Mix contributions from the five corners
  vec3 m0 = max(0.6 - vec3(dot(x0,x0), dot(x1,x1), dot(x2,x2)), 0.0);
  vec2 m1 = max(0.6 - vec2(dot(x3,x3), dot(x4,x4)            ), 0.0);
  m0 = m0 * m0;
  m1 = m1 * m1;
  return 49.0 * ( dot(m0*m0, vec3( dot( p0, x0 ), dot( p1, x1 ), dot( p2, x2 )))
               + dot(m1*m1, vec2( dot( p3, x3 ), dot( p4, x4 ) ) ) ) ;
}

float smoothstep(float edge0, float edge1, float x)
{
  // Scale, bias and saturate x to 0..1 range
  x = clamp((x - edge0)/(edge1 - edge0), 0.0, 1.0); 
  // Evaluate polynomial
  return x*x*(3 - 2*x);
}

// Acceleration, velocity and position of previous time step
uniform sampler2D acceleration_sampler_2D;
uniform sampler2D velocity_sampler_2D;
uniform sampler2D position_sampler_2D;

uniform float dt; // Time step
uniform float time; // Global time 

// Properties of the particle system
uniform float field_speed;
uniform float curl;
uniform float progression_rate;
uniform float length_scale;
uniform vec3  field_main_direction;

// Output
out vec4 acceleration_out;

// The vector field potential has three components
vec3 potential(vec3 p)
{
  float L;      // Length scale as described by Bridson
  float speed;  // field speed
  float alpha;  // Alpha as described by Bridson
  float beta;   // amount of curl noise compared to the constant field
  vec3  n;       // Normal of closest surface
  vec3  pot;     // Output potential
  
  L = length_scale;
  speed = field_speed;
  beta = curl;

  // Start with an empty field
  pot = vec3(0,0,0);
  // Add Noise in each direction
  pot += L * beta * speed * vec3(
    snoise(vec4(p.x, p.y,       p.z,      time * progression_rate * 0.1) / L),
    snoise(vec4(p.x, p.y + 43,  p.z,      time * progression_rate * 0.1) / L),
    snoise(vec4(p.x, p.y,       p.z + 43, time * progression_rate * 0.1) / L));
  
  //pot += 0.2 * L * beta * speed * vec3(
  //  snoise(vec4(p.x, p.y,       p.z,      time * progression_rate * 0.1) / L * 2),
  //  snoise(vec4(p.x, p.y + 43,  p.z,      time * progression_rate * 0.1) / L * 2),
  //  snoise(vec4(p.x, p.y,       p.z + 43, time * progression_rate * 0.1) / L * 2));
  

  //vec2 F = cellular(vec3(p.x, p.y, p.z) / L);
  //float n1 = F.x;
  //F = cellular(vec3(p.x, p.y + 43, p.z) / L);
  //float n2 = F.x;
  //F = cellular(vec3(p.x, p.y, p.z + 43) / L);
  //float n3 = F.x;
  //vec3 hej = vec3(n1, n2, n3);

  // Add Noise in each direction
  //pot += L * beta * speed * hej;
  

  // External directional field
  // Rotational potential gives a constant velocity field
  vec3 p_parallel = dot(field_main_direction, p) * field_main_direction;
  vec3 p_orthogonal = p - p_parallel;
  vec3 pot_directional = cross(p_orthogonal, field_main_direction);

  // Add the rotational potential
  pot += (1 - beta) * speed * pot_directional;

  // Affect the field by a sphere
  // The closer to the sphere, the less of the original potential
  // and the more of a tangental potential
  alpha = abs((smoothstep(0.5, 0.5+L, length(p))));
  n = normalize(p);
  pot = (alpha - 0.1) * pot + (1 - (alpha - 0.1)) * n * dot(n, pot);

  return pot;
}

void main(){
  // Get state from previous time step
  vec3 a = texelFetch( acceleration_sampler_2D, ivec2(gl_FragCoord.xy), 0).xyz;
  vec3 v = texelFetch( velocity_sampler_2D,     ivec2(gl_FragCoord.xy), 0).xyz;
  vec3 p = texelFetch( position_sampler_2D,     ivec2(gl_FragCoord.xy), 0).xyz;

  // Step length for approximating derivatives
  float epsilon = 0.00001;
  vec3 pot = potential(p);

  // Partial derivatives of different components of the potential
  float dp3_dy = (pot.z - potential(vec3(p.x, p.y + epsilon, p.z))).z / epsilon;
  float dp2_dz = (pot.y - potential(vec3(p.x, p.y, p.z + epsilon))).y / epsilon;
  float dp1_dz = (pot.x - potential(vec3(p.x, p.y, p.z + epsilon))).x / epsilon;
  float dp3_dx = (pot.z - potential(vec3(p.x + epsilon, p.y, p.z))).z / epsilon;
  float dp2_dx = (pot.y - potential(vec3(p.x + epsilon, p.y, p.z))).y / epsilon;
  float dp1_dy = (pot.x - potential(vec3(p.x, p.y + epsilon, p.z))).x / epsilon;

  // vel = nabla x potential
  // Since this vecotor field has only a vector potential component
  // it is divergent free and hence contains no sources
  vec3 vel = vec3(dp3_dy - dp2_dz, dp1_dz - dp3_dx, dp2_dx - dp1_dy);
  vec3 v_goal = vel;

  // Update the acceleration for particles to follow the velocity field
  float friction = 1;
  a = friction * (v_goal - v);

  acceleration_out = vec4(a.xyz,1);
}
