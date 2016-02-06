#ifndef PARTICLE_SYSTEM_GPU_H
#define PARTICLE_SYSTEM_GPU_H

#include "SGE/Object3D.h"
#include "SGE/Mesh.h"
#include "Material.h"

#include <map>

typedef enum {
  CURL_NOISE,
  CURL_NOISE_VORTEX,
  ATTRACTOR,
} ParticleProgramEnum;

struct ParticleSystemProperties {
  ParticleProgramEnum program;
  float field_speed;
  float noise_strength;
  float progression_rate;
  float length_scale;
  float life_length_factor;
  float emitter_size;
  float vortex_radius;
  glm::vec3 emitter_position;
  glm::vec3 field_main_direction;
};

class ParticleProgram {
public:
  ParticleProgram();
  ParticleProgram(
    GLuint update_accelerations_program_ID,
    GLuint update_velocities_program_ID,
    GLuint updatpositions_program_ID);
  ~ParticleProgram();

  GLuint getUpdateAccelerationsProgramID();
  GLuint getUpdateVelocitiesProgramID();
  GLuint getUpdatePositionsProgramID();
private:
  GLuint _update_accelerations_program_ID;
  GLuint _update_velocities_program_ID;
  GLuint _update_positions_program_ID;
};

class ParticleSystem : public Object3D {
public:
  ParticleSystem(
    unsigned long size,
    ParticleProgramEnum program);
  ~ParticleSystem();
  void render(glm::mat4 M);
  void update(float dt);
  ParticleSystemProperties* getPropertiesPointer();
  PointCloudRenderingProperties* getPointCloudRenderingPropertiesPointer();

private:
  void updateAccelerations(float dt);
  void updateVelocities(float dt);
  void updatePositions(float dt);
  void swapTextures();

  // OpenGL handles
  std::map<ParticleProgramEnum, ParticleProgram> _programs;

  GLuint _acceleration_frame_buffer;
  GLuint _velocity_frame_buffer;
  GLuint _position_frame_buffer;
  
  GLuint _acceleration_texture_to_render;
  GLuint _velocity_texture_to_render;
  GLuint _position_texture_to_render;
  
  GLuint _quad_VAO;
  GLuint _quad_VBO;
  GLuint _quad_element_buffer;
  
  // Data
  ParticleSystemProperties _properties;
  
  const unsigned long _size;
  PointCloudMesh*     _mesh;
  PointCloudMaterial* _material;
  
  std::vector<glm::vec3> _quad_vertices;
  std::vector<unsigned short> _quad_elements;

  float _time;
};

#endif