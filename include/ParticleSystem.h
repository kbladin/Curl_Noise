#ifndef PARTICLE_SYSTEM_GPU_H
#define PARTICLE_SYSTEM_GPU_H

#include "SGE/Object3D.h"
#include "SGE/Mesh.h"
#include "Material.h"

#include <map>

typedef enum {
  CURL_NOISE,
  CURL_NOISE2,
} ParticleProgramEnum;

struct ParticleSystemProperties {
  ParticleProgramEnum program;
  float field_speed;
  float curl;
  float progression_rate;
  float length_scale;
  float life_length_factor;
  float emitter_size;
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
  GLuint update_accelerations_program_ID_;
  GLuint update_velocities_program_ID_;
  GLuint update_positions_program_ID_;
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

  ParticleSystemProperties properties_;
  
  const unsigned long size_;
  PointCloudMesh* mesh_;
  PointCloudMaterial* material_;
  PointCloudMaterial* shadow_material_;

  std::map<ParticleProgramEnum, ParticleProgram> programs_;

  GLuint acceleration_frame_buffer_;
  GLuint velocity_frame_buffer_;
  GLuint position_frame_buffer_;
  
  GLuint acceleration_texture_to_render_;
  GLuint velocity_texture_to_render_;
  GLuint position_texture_to_render_;
  
  GLuint quad_VAO_;
  GLuint quad_VBO_;
  GLuint quad_element_buffer_;
  
  std::vector<glm::vec3> quad_vertices_;
  std::vector<unsigned short> quad_elements_;

  float time;
};

#endif