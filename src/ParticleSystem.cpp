#include "ParticleSystem.h"

ParticleProgram::ParticleProgram()
{
  _update_accelerations_program_ID = 0;
  _update_velocities_program_ID = 0;
  _update_positions_program_ID = 0;
}

ParticleProgram::ParticleProgram(
    GLuint update_accelerations_program_ID,
    GLuint update_velocities_program_ID,
    GLuint update_positions_program_ID)
{
  _update_accelerations_program_ID = update_accelerations_program_ID;
  _update_velocities_program_ID = update_velocities_program_ID;
  _update_positions_program_ID = update_positions_program_ID;
}

ParticleProgram::~ParticleProgram()
{

}

GLuint ParticleProgram::getUpdateAccelerationsProgramID()
{
  return _update_accelerations_program_ID;
}

GLuint ParticleProgram::getUpdateVelocitiesProgramID()
{
  return _update_velocities_program_ID;
}

GLuint ParticleProgram::getUpdatePositionsProgramID()
{
  return _update_positions_program_ID;
}

ParticleSystem::ParticleSystem(
  unsigned long size,
  ParticleProgramEnum program) :
  
  _size(int(sqrt(size)))
{
  // Insert all the possible programs in the map of the update programs
  _programs.insert(
    std::pair<ParticleProgramEnum, ParticleProgram>(
      CURL_NOISE, ParticleProgram(
        0,
        ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_VELOCITIES"),
        ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_POSITIONS")
      )));
  _programs.insert(
    std::pair<ParticleProgramEnum, ParticleProgram>(
      CURL_NOISE_VORTEX, ParticleProgram(
        0,
        ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_VELOCITIES_VORTEX"),
        ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_POSITIONS")
      )));
  _programs.insert(
    std::pair<ParticleProgramEnum, ParticleProgram>(
      ATTRACTOR, ParticleProgram(
        ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_ACCELERATIONS_GRAVITY"),
        ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_VELOCITIES_STEP"),
        ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_POSITIONS_STEP")
      )));

  _properties.field_speed = 1.0;
  _properties.noise_strength = 0.3;
  _properties.progression_rate = 1.0;
  _properties.length_scale = 0.5;
  _properties.life_length_factor = 0.07;
  _properties.emitter_size = 0.4;
  _properties.vortex_radius = 3;
  _properties.emitter_position = glm::vec3(0,-2,0);
  _properties.field_main_direction = glm::vec3(0,1,0);
  _properties.program = program;

  _material = new PointCloudMaterial(_size);
  _mesh = new PointCloudMesh(_size);
  _time = 0;
  
  // We want to render to three textures as well. One for each attrubute.
  // When updating. We sample from previous state and update these textures.
  glGenTextures(1, &_acceleration_texture_to_render);
  glBindTexture(GL_TEXTURE_2D, _acceleration_texture_to_render);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    _size,
    _size,
    0,
    GL_RGB,
    GL_FLOAT,
    0);
  // Need mipmap for some reason......
  glGenerateMipmap(GL_TEXTURE_2D);
  
  glGenTextures(1, &_velocity_texture_to_render);
  glBindTexture(GL_TEXTURE_2D, _velocity_texture_to_render);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    _size,
    _size,
    0,
    GL_RGB,
    GL_FLOAT,
    0);
  // Need mipmap for some reason......
  glGenerateMipmap(GL_TEXTURE_2D);
  
  glGenTextures(1, &_position_texture_to_render);
  glBindTexture(GL_TEXTURE_2D, _position_texture_to_render);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA,
    _size,
    _size,
    0,
    GL_RGBA,
    GL_FLOAT,
    0);
  // Need mipmap for some reason......
  glGenerateMipmap(GL_TEXTURE_2D);
  
  // Creating a quad. We render a quad to render to a framebuffer which is
  // linked to a texture. One framebuffer for each texture
  _quad_vertices.resize(4);
  _quad_elements.resize(6);
  
  _quad_vertices[0] = glm::vec3(1.f, 1.f, 0.0f);
  _quad_vertices[1] = glm::vec3(1.f, -1.f, 0.0f);
  _quad_vertices[2] = glm::vec3(-1.f, -1.f, 0.0f);
  _quad_vertices[3] = glm::vec3(-1.f, 1.f, 0.0f);
  
  _quad_elements[0] = 0;
  _quad_elements[1] = 2;
  _quad_elements[2] = 1;
  _quad_elements[3] = 0;
  _quad_elements[4] = 3;
  _quad_elements[5] = 2;
  
  glGenVertexArrays(1, &_quad_VAO);
  glBindVertexArray(_quad_VAO);
  
  glGenBuffers(1, &_quad_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _quad_VBO);
  glBufferData(
    GL_ARRAY_BUFFER,
    _quad_vertices.size() * sizeof(glm::vec3),
    &_quad_vertices[0],
    GL_STATIC_DRAW);
  
  glGenBuffers(1, &_quad_element_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quad_element_buffer);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    _quad_elements.size() * sizeof(unsigned short),
    &_quad_elements[0],
    GL_STATIC_DRAW);

  // The frame buffers. One for each attribute
  // Acceleration
  glGenFramebuffers(1, &_acceleration_frame_buffer);
  glBindFramebuffer(GL_FRAMEBUFFER, _acceleration_frame_buffer);
  glFramebufferTexture(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    _acceleration_texture_to_render,
    0);

  // Check that the framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR : Frame buffer not ok!" << std::endl;
  
  // Velocity
  glGenFramebuffers(1, &_velocity_frame_buffer);
  glBindFramebuffer(GL_FRAMEBUFFER, _velocity_frame_buffer);
  glFramebufferTexture(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    _velocity_texture_to_render,
    0);

  // Check that the framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR : Frame buffer not ok!" << std::endl;
  
  // Position
  glGenFramebuffers(1, &_position_frame_buffer);
  glBindFramebuffer(GL_FRAMEBUFFER, _position_frame_buffer);
  glFramebufferTexture(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    _position_texture_to_render,
    0);

  // Check that the framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR : Frame buffer not ok!" << std::endl;
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ParticleSystem::~ParticleSystem()
{
  delete _material;
  delete _mesh;
  glDeleteFramebuffers(1, &_position_frame_buffer);
  glDeleteFramebuffers(1, &_velocity_frame_buffer);
  glDeleteFramebuffers(1, &_acceleration_frame_buffer);
  glDeleteTextures(1, &_acceleration_texture_to_render);
  glDeleteTextures(1, &_velocity_texture_to_render);
  glDeleteTextures(1, &_position_texture_to_render);
}

void ParticleSystem::render(glm::mat4 M)
{
  _material->use();

  _mesh->render(M, _material->getProgramID());

  // Reset state
  glDisable(GL_BLEND);
}

ParticleSystemProperties* ParticleSystem::getPropertiesPointer()
{
  return &_properties;
}

PointCloudRenderingProperties* ParticleSystem::getPointCloudRenderingPropertiesPointer()
{
  return _material->getPropertiesPointer();
}

void ParticleSystem::updateAccelerations(float dt)
{
  glBindFramebuffer(GL_FRAMEBUFFER, _acceleration_frame_buffer);
  glViewport(0,0,_size,_size);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  GLuint update_accelerations_program_ID =
    _programs[_properties.program].getUpdateAccelerationsProgramID();
  
  glUseProgram(update_accelerations_program_ID);
  
  // These should not be done every time....
  glUniform1f(
    glGetUniformLocation(update_accelerations_program_ID,"dt"),
    dt);
  glUniform1f(
    glGetUniformLocation(update_accelerations_program_ID,"time"),
    _time);
  glUniform1i(
    glGetUniformLocation(update_accelerations_program_ID,"size"),
    _size);
  glUniform1i(
    glGetUniformLocation(update_accelerations_program_ID,"acceleration_sampler_2D"),
    0);
  glUniform1i(
    glGetUniformLocation(update_accelerations_program_ID,"velocity_sampler_2D"),
    1);
  glUniform1i(
    glGetUniformLocation(update_accelerations_program_ID,"position_sampler_2D"),
    2);
  
  // Properties of the particle system
  glUniform1f(
    glGetUniformLocation(update_accelerations_program_ID, "field_speed"),
    _properties.field_speed);
  glUniform1f(
    glGetUniformLocation(update_accelerations_program_ID, "noise_strength"),
    _properties.noise_strength);
  glUniform1f(
    glGetUniformLocation(update_accelerations_program_ID, "progression_rate"),
    _properties.progression_rate);
  glUniform1f(
    glGetUniformLocation(update_accelerations_program_ID, "length_scale"),
    _properties.length_scale);
  glUniform3f(
    glGetUniformLocation(update_accelerations_program_ID, "emitter_position"),
    _properties.emitter_position.x,
    _properties.emitter_position.y,
    _properties.emitter_position.z);
  
  if (_properties.field_main_direction != glm::vec3(0,0,0))
    _properties.field_main_direction =
      glm::normalize(_properties.field_main_direction);
  else
    _properties.field_main_direction = glm::vec3(0,1,0);
  glUniform3f(
    glGetUniformLocation(update_accelerations_program_ID, "field_main_direction"),
    _properties.field_main_direction.x,
    _properties.field_main_direction.y,
    _properties.field_main_direction.z);

  // Textures we want to sample from. All from previous state
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _material->getAccelerationTextureToSample());
  
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, _material->getVelocityTextureToSample());
  
  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_2D, _material->getPositionTextureToSample());
  
  glBindVertexArray(_quad_VAO);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _quad_VBO);
  glVertexAttribPointer(
    0,          // attribute
    3,          // size
    GL_FLOAT,   // type
    GL_FALSE,   // normalized?
    0,          // stride
    (void*)0);  // array buffer offset
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quad_element_buffer);

  glDisable(GL_BLEND);
  // Draw the quad
  glDrawElements(
    GL_TRIANGLES,           // mode
    _quad_elements.size(),  // count
    GL_UNSIGNED_SHORT,      // type
    (void*)0);              // element array buffer offset
  
  glDisableVertexAttribArray(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ParticleSystem::updateVelocities(float dt)
{
  glBindFramebuffer(GL_FRAMEBUFFER, _velocity_frame_buffer);
  glViewport(0,0,_size,_size);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GLuint update_velocities_program_ID =
    _programs[_properties.program].getUpdateVelocitiesProgramID();
  
  glUseProgram(update_velocities_program_ID);

  glUniform1f(
    glGetUniformLocation(update_velocities_program_ID,"dt"),
    dt);
  glUniform1f(
    glGetUniformLocation(update_velocities_program_ID,"time"),
    _time);
  glUniform1i(
    glGetUniformLocation(update_velocities_program_ID,"size"),
    _size);
  glUniform1i(
    glGetUniformLocation(update_velocities_program_ID,"acceleration_sampler_2D"),
    0);
  glUniform1i(
    glGetUniformLocation(update_velocities_program_ID,"velocity_sampler_2D"),
    1);
  glUniform1i(
    glGetUniformLocation(update_velocities_program_ID,"position_sampler_2D"),
    2);
  
  // Properties of the particle system
  glUniform1f(
    glGetUniformLocation(update_velocities_program_ID, "field_speed"),
    _properties.field_speed);
  glUniform1f(
    glGetUniformLocation(update_velocities_program_ID, "noise_strength"),
    _properties.noise_strength);
  glUniform1f(
    glGetUniformLocation(update_velocities_program_ID, "progression_rate"),
    _properties.progression_rate);
  glUniform1f(
    glGetUniformLocation(update_velocities_program_ID, "length_scale"),
    _properties.length_scale);
  glUniform1f(
    glGetUniformLocation(update_velocities_program_ID, "vortex_radius"),
    _properties.vortex_radius);
  
  if (_properties.field_main_direction != glm::vec3(0,0,0))
    _properties.field_main_direction =
      glm::normalize(_properties.field_main_direction);
  else
    _properties.field_main_direction = glm::vec3(0,1,0);
  glUniform3f(
    glGetUniformLocation(update_velocities_program_ID, "field_main_direction"),
    _properties.field_main_direction.x,
    _properties.field_main_direction.y,
    _properties.field_main_direction.z);

  // Acceleration from current state (so that we can integrate it, solve diff eq)
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _acceleration_texture_to_render);
  
  // These attributes are from previous state
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, _material->getVelocityTextureToSample());
  
  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_2D, _material->getPositionTextureToSample());

  glBindVertexArray(_quad_VAO);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _quad_VBO);
  glVertexAttribPointer(
    0,          // attribute
    3,          // size
    GL_FLOAT,   // type
    GL_FALSE,   // normalized?
    0,          // stride
    (void*)0);  // array buffer offset
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quad_element_buffer);

  glDisable(GL_BLEND);
  // Draw the quad
  glDrawElements(
    GL_TRIANGLES,           // mode
    _quad_elements.size(),  // count
    GL_UNSIGNED_SHORT,      // type
    (void*)0);              // element array buffer offset
  
  glDisableVertexAttribArray(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ParticleSystem::updatePositions(float dt)
{
  glBindFramebuffer(GL_FRAMEBUFFER, _position_frame_buffer);
  glViewport(0,0,_size,_size);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  GLuint update_positions_program_ID =
    _programs[_properties.program].getUpdatePositionsProgramID();

  glUseProgram(update_positions_program_ID);
  
  glUniform1f(
    glGetUniformLocation(update_positions_program_ID, "dt"),
    dt);
  glUniform1i(
    glGetUniformLocation(update_positions_program_ID, "size"),
    _size);
  glUniform1i(
    glGetUniformLocation(update_positions_program_ID, "acceleration_sampler_2D"),
    0);
  glUniform1i(
    glGetUniformLocation(update_positions_program_ID, "velocity_sampler_2D"),
    1);
  glUniform1i(
    glGetUniformLocation(update_positions_program_ID, "position_sampler_2D"),
    2);
  
  // Properties of the particle system
  glUniform1f(
    glGetUniformLocation(update_positions_program_ID, "inv_life_length_factor"),
    1 / _properties.life_length_factor);
  glUniform1f(
    glGetUniformLocation(update_positions_program_ID, "emitter_size"),
    _properties.emitter_size);
  glUniform3f(
    glGetUniformLocation(update_positions_program_ID, "emitter_position"),
    _properties.emitter_position.x,
    _properties.emitter_position.y,
    _properties.emitter_position.z);

  // Acceleration and velocity are from current state
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _acceleration_texture_to_render);
  
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, _velocity_texture_to_render);
  
  // Position from previous state
  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_2D, _material->getPositionTextureToSample());

  glBindVertexArray(_quad_VAO);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _quad_VBO);
  glVertexAttribPointer(
    0,          // attribute
    3,          // size
    GL_FLOAT,   // type
    GL_FALSE,   // normalized?
    0,          // stride
    (void*)0);  // array buffer offset

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quad_element_buffer);
  
  glDisable(GL_BLEND);
  // Draw the quad
  glDrawElements(
    GL_TRIANGLES,           // mode
    _quad_elements.size(),  // count
    GL_UNSIGNED_SHORT,      // type
    (void*)0);              // element array buffer offset
  
  glDisableVertexAttribArray(0);
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*!
  If the variable program in ParticleSystemProperties has been changed, the
  programs that updates the particle system are updated.
*/
  /*
void ParticleSystem::updateProgramIfNeeded()
{
  if (_properties.program_needs_update)
  {
    if (_properties.program == CURL_NOISE)
    {
      _update_accelerations_program_ID = ShaderManager::instance()->getShader(
        "SHADER_UPDATE_POINT_CLOUD_ACCELERATIONS");
      _update_velocities_program_ID = ShaderManager::instance()->getShader(
        "SHADER_UPDATE_POINT_CLOUD_VELOCITIES");
      _update_positions_program_ID = ShaderManager::instance()->getShader(
        "SHADER_UPDATE_POINT_CLOUD_POSITIONS");
    }
    _properties.program_needs_update = false;
  }
}
*/
void ParticleSystem::update(float dt)
{
  const int sims_per_frame = 1;
  for (int i=0; i<sims_per_frame; i++) {
    if (_programs[_properties.program].getUpdateAccelerationsProgramID() != 0)
      updateAccelerations(dt / sims_per_frame);
    if (_programs[_properties.program].getUpdateVelocitiesProgramID() != 0)
      updateVelocities(dt / sims_per_frame);
    if (_programs[_properties.program].getUpdatePositionsProgramID() != 0)
      updatePositions(dt / sims_per_frame);
    swapTextures();
    _time += dt / sims_per_frame;
  }
}

void ParticleSystem::swapTextures()
{
  // Swap so that the newly updated textures will be used for sampling. We can
  // render over the old ones now. They are not used any more.
  _acceleration_texture_to_render =
    _material->swapAccelerationTexture(_acceleration_texture_to_render);
  _velocity_texture_to_render =
    _material->swapVelocityTexture(_velocity_texture_to_render);
  _position_texture_to_render =
    _material->swapPositionTexture(_position_texture_to_render);
  
  // We need to relink the texutes for rendering to the frame buffers.
  glBindFramebuffer(GL_FRAMEBUFFER, _acceleration_frame_buffer);
  glFramebufferTexture(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    _acceleration_texture_to_render,
    0);
  glBindFramebuffer(GL_FRAMEBUFFER, _velocity_frame_buffer);
  glFramebufferTexture(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    _velocity_texture_to_render,
    0);
  glBindFramebuffer(GL_FRAMEBUFFER, _position_frame_buffer);
  glFramebufferTexture(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    _position_texture_to_render,
    0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}