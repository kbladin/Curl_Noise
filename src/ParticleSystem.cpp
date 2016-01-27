#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(unsigned long size) : size_(int(sqrt(size)))
{
  properties_.field_speed = 0.1;
  properties_.curl = 0.3;
  properties_.progression_rate = 0.5;
  properties_.length_scale = 0.5;
  properties_.life_length_factor = 0.2;
  properties_.emitter_size = 0.2;
  properties_.emitter_position = glm::vec3(0,-2,0);
  properties_.field_main_direction = glm::vec3(0,1,0);

  material_ = new PointCloudMaterial(size_);
  mesh_ = new PointCloudMesh(size_);
  time = 0;
  
  // Three shaders. One for each attribute
  update_accelerations_program_ID_ =
    ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_ACCELERATIONS");
  update_velocities_program_ID_ =
    ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_VELOCITIES");
  update_positions_program_ID_ =
    ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_POSITIONS");
  
  // We want to render to three textures as well. One for each attrubute.
  // When updating. We sample from previous state and update these textures.
  glGenTextures(1, &acceleration_texture_to_render_);
  glBindTexture(GL_TEXTURE_2D, acceleration_texture_to_render_);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    size_,
    size_,
    0,
    GL_RGB,
    GL_FLOAT,
    0);
  // Need mipmap for some reason......
  glGenerateMipmap(GL_TEXTURE_2D);
  
  glGenTextures(1, &velocity_texture_to_render_);
  glBindTexture(GL_TEXTURE_2D, velocity_texture_to_render_);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    size_,
    size_,
    0,
    GL_RGB,
    GL_FLOAT,
    0);
  // Need mipmap for some reason......
  glGenerateMipmap(GL_TEXTURE_2D);
  
  glGenTextures(1, &position_texture_to_render_);
  glBindTexture(GL_TEXTURE_2D, position_texture_to_render_);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA,
    size_,
    size_,
    0,
    GL_RGBA,
    GL_FLOAT,
    0);
  // Need mipmap for some reason......
  glGenerateMipmap(GL_TEXTURE_2D);
  
  // Creating a quad. We render a quad to render to a framebuffer which is
  // linked to a texture. One framebuffer for each texture
  quad_vertices_.resize(4);
  quad_elements_.resize(6);
  
  quad_vertices_[0] = glm::vec3(1.f, 1.f, 0.0f);
  quad_vertices_[1] = glm::vec3(1.f, -1.f, 0.0f);
  quad_vertices_[2] = glm::vec3(-1.f, -1.f, 0.0f);
  quad_vertices_[3] = glm::vec3(-1.f, 1.f, 0.0f);
  
  quad_elements_[0] = 0;
  quad_elements_[1] = 2;
  quad_elements_[2] = 1;
  quad_elements_[3] = 0;
  quad_elements_[4] = 3;
  quad_elements_[5] = 2;
  
  glGenVertexArrays(1, &quad_VAO_);
  glBindVertexArray(quad_VAO_);
  
  glGenBuffers(1, &quad_VBO_);
  glBindBuffer(GL_ARRAY_BUFFER, quad_VBO_);
  glBufferData(
    GL_ARRAY_BUFFER,
    quad_vertices_.size() * sizeof(glm::vec3),
    &quad_vertices_[0],
    GL_STATIC_DRAW);
  
  glGenBuffers(1, &quad_element_buffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_element_buffer_);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    quad_elements_.size() * sizeof(unsigned short),
    &quad_elements_[0],
    GL_STATIC_DRAW);

  // The frame buffers. One for each attribute
  // Acceleration
  glGenFramebuffers(1, &acceleration_frame_buffer_);
  glBindFramebuffer(GL_FRAMEBUFFER, acceleration_frame_buffer_);
  glFramebufferTexture(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    acceleration_texture_to_render_,
    0);

  // Check that the framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR : Frame buffer not ok!" << std::endl;
  
  // Velocity
  glGenFramebuffers(1, &velocity_frame_buffer_);
  glBindFramebuffer(GL_FRAMEBUFFER, velocity_frame_buffer_);
  glFramebufferTexture(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    velocity_texture_to_render_,
    0);

  // Check that the framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR : Frame buffer not ok!" << std::endl;
  
  // Position
  glGenFramebuffers(1, &position_frame_buffer_);
  glBindFramebuffer(GL_FRAMEBUFFER, position_frame_buffer_);
  glFramebufferTexture(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    position_texture_to_render_,
    0);

  // Check that the framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR : Frame buffer not ok!" << std::endl;
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ParticleSystem::~ParticleSystem()
{
  delete material_;
  delete mesh_;
  glDeleteFramebuffers(1, &position_frame_buffer_);
  glDeleteFramebuffers(1, &velocity_frame_buffer_);
  glDeleteFramebuffers(1, &acceleration_frame_buffer_);
  glDeleteTextures(1, &acceleration_texture_to_render_);
  glDeleteTextures(1, &velocity_texture_to_render_);
  glDeleteTextures(1, &position_texture_to_render_);
  glDeleteProgram(update_positions_program_ID_);
}

void ParticleSystem::render(glm::mat4 M)
{
  material_->use();

  if (material_->getProgramID() == 
      ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_PHONG"))
    glDisable(GL_BLEND);
  else if (material_->getProgramID() == 
      ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_ADDITIVE"))
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  }

  mesh_->render(M, material_->getProgramID());

  // Reset state
  glDisable(GL_BLEND);
}

ParticleSystemProperties* ParticleSystem::getPropertiesPointer()
{
  return &properties_;
}

PointCloudRenderingProperties* ParticleSystem::getPointCloudRenderingPropertiesPointer()
{
  return material_->getPropertiesPointer();
}

void ParticleSystem::updateAccelerations(float dt)
{
  glBindFramebuffer(GL_FRAMEBUFFER, acceleration_frame_buffer_);
  glViewport(0,0,size_,size_);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glUseProgram(update_accelerations_program_ID_);
  
  // These should not be done every time....
  glUniform1f(
    glGetUniformLocation(update_accelerations_program_ID_,"dt"),
    dt);
  glUniform1f(
    glGetUniformLocation(update_accelerations_program_ID_,"time"),
    time);
  glUniform1i(
    glGetUniformLocation(update_accelerations_program_ID_,"size"),
    size_);
  glUniform1i(
    glGetUniformLocation(update_accelerations_program_ID_,"acceleration_sampler_2D"),
    0);
  glUniform1i(
    glGetUniformLocation(update_accelerations_program_ID_,"velocity_sampler_2D"),
    1);
  glUniform1i(
    glGetUniformLocation(update_accelerations_program_ID_,"position_sampler_2D"),
    2);
  
  // Properties of the particle system
  glUniform1f(
    glGetUniformLocation(update_accelerations_program_ID_, "field_speed"),
    properties_.field_speed);
  glUniform1f(
    glGetUniformLocation(update_accelerations_program_ID_, "curl"),
    properties_.curl);
  glUniform1f(
    glGetUniformLocation(update_accelerations_program_ID_, "progression_rate"),
    properties_.progression_rate);
  glUniform1f(
    glGetUniformLocation(update_accelerations_program_ID_, "length_scale"),
    properties_.length_scale);
  
  if (properties_.field_main_direction != glm::vec3(0,0,0))
    properties_.field_main_direction =
      glm::normalize(properties_.field_main_direction);
  else
    properties_.field_main_direction = glm::vec3(0,1,0);
  glUniform3f(
    glGetUniformLocation(update_accelerations_program_ID_, "field_main_direction"),
    properties_.field_main_direction.x,
    properties_.field_main_direction.y,
    properties_.field_main_direction.z);

  // Textures we want to sample from. All from previous state
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, material_->getAccelerationTextureToSample());
  
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, material_->getVelocityTextureToSample());
  
  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_2D, material_->getPositionTextureToSample());
  
  glBindVertexArray(quad_VAO_);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, quad_VBO_);
  glVertexAttribPointer(
    0,          // attribute
    3,          // size
    GL_FLOAT,   // type
    GL_FALSE,   // normalized?
    0,          // stride
    (void*)0);  // array buffer offset
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_element_buffer_);

  glDisable(GL_BLEND);
  // Draw the quad
  glDrawElements(
    GL_TRIANGLES,           // mode
    quad_elements_.size(),  // count
    GL_UNSIGNED_SHORT,      // type
    (void*)0);              // element array buffer offset
  
  glDisableVertexAttribArray(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ParticleSystem::updateVelocities(float dt)
{
  glBindFramebuffer(GL_FRAMEBUFFER, velocity_frame_buffer_);
  glViewport(0,0,size_,size_);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glUseProgram(update_velocities_program_ID_);


  glUniform1f(
    glGetUniformLocation(update_velocities_program_ID_,"dt"),
    dt);
  glUniform1f(
    glGetUniformLocation(update_velocities_program_ID_,"time"),
    time);
  glUniform1i(
    glGetUniformLocation(update_velocities_program_ID_,"size"),
    size_);
  glUniform1i(
    glGetUniformLocation(update_velocities_program_ID_,"acceleration_sampler_2D"),
    0);
  glUniform1i(
    glGetUniformLocation(update_velocities_program_ID_,"velocity_sampler_2D"),
    1);
  glUniform1i(
    glGetUniformLocation(update_velocities_program_ID_,"position_sampler_2D"),
    2);
  
  // Properties of the particle system
  glUniform1f(
    glGetUniformLocation(update_velocities_program_ID_, "field_speed"),
    properties_.field_speed);
  glUniform1f(
    glGetUniformLocation(update_velocities_program_ID_, "curl"),
    properties_.curl);
  glUniform1f(
    glGetUniformLocation(update_velocities_program_ID_, "progression_rate"),
    properties_.progression_rate);
  glUniform1f(
    glGetUniformLocation(update_velocities_program_ID_, "length_scale"),
    properties_.length_scale);
  
  if (properties_.field_main_direction != glm::vec3(0,0,0))
    properties_.field_main_direction =
      glm::normalize(properties_.field_main_direction);
  else
    properties_.field_main_direction = glm::vec3(0,1,0);
  glUniform3f(
    glGetUniformLocation(update_velocities_program_ID_, "field_main_direction"),
    properties_.field_main_direction.x,
    properties_.field_main_direction.y,
    properties_.field_main_direction.z);




















  // Acceleration from current state (so that we can integrate it, solve diff eq)
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, acceleration_texture_to_render_);
  
  // These attributes are from previous state
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, material_->getVelocityTextureToSample());
  
  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_2D, material_->getPositionTextureToSample());

  glBindVertexArray(quad_VAO_);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, quad_VBO_);
  glVertexAttribPointer(
    0,          // attribute
    3,          // size
    GL_FLOAT,   // type
    GL_FALSE,   // normalized?
    0,          // stride
    (void*)0);  // array buffer offset
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_element_buffer_);

  glDisable(GL_BLEND);
  // Draw the quad
  glDrawElements(
    GL_TRIANGLES,           // mode
    quad_elements_.size(),  // count
    GL_UNSIGNED_SHORT,      // type
    (void*)0);              // element array buffer offset
  
  glDisableVertexAttribArray(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ParticleSystem::updatePositions(float dt)
{
  glBindFramebuffer(GL_FRAMEBUFFER, position_frame_buffer_);
  glViewport(0,0,size_,size_);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glUseProgram(update_positions_program_ID_);
  
  glUniform1f(
    glGetUniformLocation(update_positions_program_ID_, "dt"),
    dt);
  glUniform1i(
    glGetUniformLocation(update_positions_program_ID_, "size"),
    size_);
  glUniform1i(
    glGetUniformLocation(update_positions_program_ID_, "acceleration_sampler_2D"),
    0);
  glUniform1i(
    glGetUniformLocation(update_positions_program_ID_, "velocity_sampler_2D"),
    1);
  glUniform1i(
    glGetUniformLocation(update_positions_program_ID_, "position_sampler_2D"),
    2);
  
  // Properties of the particle system
  glUniform1f(
    glGetUniformLocation(update_positions_program_ID_, "inv_life_length_factor"),
    1 / properties_.life_length_factor);
  glUniform1f(
    glGetUniformLocation(update_positions_program_ID_, "emitter_size"),
    properties_.emitter_size);
  glUniform3f(
    glGetUniformLocation(update_positions_program_ID_, "emitter_position"),
    properties_.emitter_position.x,
    properties_.emitter_position.y,
    properties_.emitter_position.z);

  // Acceleration and velocity are from current state
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, acceleration_texture_to_render_);
  
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, velocity_texture_to_render_);
  
  // Position from previous state
  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_2D, material_->getPositionTextureToSample());

  glBindVertexArray(quad_VAO_);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, quad_VBO_);
  glVertexAttribPointer(
    0,          // attribute
    3,          // size
    GL_FLOAT,   // type
    GL_FALSE,   // normalized?
    0,          // stride
    (void*)0);  // array buffer offset

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_element_buffer_);
  
  glDisable(GL_BLEND);
  // Draw the quad
  glDrawElements(
    GL_TRIANGLES,           // mode
    quad_elements_.size(),  // count
    GL_UNSIGNED_SHORT,      // type
    (void*)0);              // element array buffer offset
  
  glDisableVertexAttribArray(0);
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ParticleSystem::update(float dt)
{
  const int sims_per_frame = 1;
  for (int i=0; i<sims_per_frame; i++) {
    // For curl noise, velocities are updated directly
    //updateAccelerations(dt / sims_per_frame);
    updateVelocities(dt / sims_per_frame);
    updatePositions(dt / sims_per_frame);
    swapTextures();
    time += dt / sims_per_frame;
  }
}

void ParticleSystem::swapTextures()
{
  // Swap so that the newly updated textures will be used for sampling. We can
  // render over the old ones now. They are not used any more.
  acceleration_texture_to_render_ =
    material_->swapAccelerationTexture(acceleration_texture_to_render_);
  velocity_texture_to_render_ =
    material_->swapVelocityTexture(velocity_texture_to_render_);
  position_texture_to_render_ =
    material_->swapPositionTexture(position_texture_to_render_);
  
  // We need to relink the texutes for rendering to the frame buffers.
  glBindFramebuffer(GL_FRAMEBUFFER, acceleration_frame_buffer_);
  glFramebufferTexture(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    acceleration_texture_to_render_,
    0);
  glBindFramebuffer(GL_FRAMEBUFFER, velocity_frame_buffer_);
  glFramebufferTexture(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    velocity_texture_to_render_,
    0);
  glBindFramebuffer(GL_FRAMEBUFFER, position_frame_buffer_);
  glFramebufferTexture(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    position_texture_to_render_,
    0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}