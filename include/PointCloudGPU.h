#ifndef POINT_CLOUD_GPU_H
#define POINT_CLOUD_GPU_H

#include "../include/Object3D.h"
#include "../include/Mesh.h"

class PointCloudGPU : public Object3D {
public:
  PointCloudGPU(unsigned long size);
  ~PointCloudGPU();
  void render(glm::mat4 M);
  void update(float dt);
private:
  void updateAccelerations(float dt);
  void updateVelocities(float dt);
  void updatePositions(float dt);
  void swapTextures();
  
  const unsigned long size_;
  PointCloudMesh* mesh_;
  PointCloudMaterial* material_;

  GLuint update_accelerations_program_ID_;
  GLuint update_velocities_program_ID_;
  GLuint update_positions_program_ID_;
  
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
};

#endif