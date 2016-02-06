#include "Material.h"

//! Create a material which is bound to a specific shader.
/*!
  \param program_ID can be one of the shaders in ShaderManager.
*/
Material::Material(GLuint program_ID) : _program_ID(program_ID)
{

}

//! Returns the program ID of the shader of the Material.
GLuint Material::getProgramID() const
{
  return _program_ID;
};

//! Create a material which is bound to the Phong shader.
PhongMaterial::PhongMaterial() :
  Material(ShaderManager::instance()->getShader("SHADER_PHONG"))
{
  diffuse_color = glm::vec3(1.0, 1.0, 1.0);
  specular_color = glm::vec3(1.0, 1.0, 1.0);
  specularity = 0.5;
  shinyness = 16;
  
  glUseProgram(_program_ID);
  _diffuse_color_ID =  glGetUniformLocation(_program_ID, "material_diffise_color");
  _specular_color_ID = glGetUniformLocation(_program_ID, "material_specularColor");
  _specularity_ID =   glGetUniformLocation(_program_ID, "material_specularity");
  _shinyness_ID =     glGetUniformLocation(_program_ID, "material_shinyness");
}

//! Updating the shader parameters.
/*!
  This function is automatically called when a mesh with this material is
  rendered.
*/
void PhongMaterial::use() const
{
  // Use our shader
  glUseProgram(_program_ID);
  glUniform3f(
    _diffuse_color_ID,
    diffuse_color.r,
    diffuse_color.g,
    diffuse_color.b);
  glUniform3f(
    _specular_color_ID,
    specular_color.r,
    specular_color.g,
    specular_color.b);
  glUniform1f(_specularity_ID, specularity);
  glUniform1i(_shinyness_ID, shinyness);
}

//! Create a material which is bound to the OneColor shader.
OneColorMaterial::OneColorMaterial() :
  Material(ShaderManager::instance()->getShader("SHADER_ONE_COLOR"))
{
  diffuse_color = glm::vec3(1.0, 1.0, 1.0);
  
  glUseProgram(_program_ID);
  _diffuse_color_ID = glGetUniformLocation(_program_ID, "material_diffise_color");
}

//! Updating the shader parameters.
/*!
  This function is automatically called when a mesh with this material is
  rendered.
*/
void OneColorMaterial::use() const
{
  // Use our shader
  glUseProgram(_program_ID);
  glUniform3f(
    _diffuse_color_ID,
    diffuse_color.r,
    diffuse_color.g,
    diffuse_color.b);
}

//! Create a material which is bound to the Background shader.
BackgroundMaterial::BackgroundMaterial() :
  Material(ShaderManager::instance()->getShader("SHADER_BACKGROUND"))
{
  glUseProgram(_program_ID);
}

//! Updating the shader parameters.
/*!
  This function is automatically called when a mesh with this material is
  rendered.
*/
void BackgroundMaterial::use() const
{
  // Use our shader
  glUseProgram(_program_ID);
}

//! Create a material which is bound to the UpdatePointCloud shader.
PointCloudMaterial::PointCloudMaterial(unsigned long size)
{
  _rendering_properties.particle_color1 = glm::vec3(0.5,0.5,1);
  _rendering_properties.particle_color2 = glm::vec3(1,0.5,0.2);
  _rendering_properties.particle_radius = 2;
  _rendering_properties.shader = ADDITIVE;

  std::vector<glm::vec3> accelerations;
  std::vector<glm::vec3> velocities;
  std::vector<glm::vec4> positions;
  accelerations.resize(size * size);
  velocities.resize(size * size);
  positions.resize(size * size);
  
  // Seed random number generator.
  srand(clock());
  
  for (int i=0; i<positions.size(); i++) {
    float r = (float(rand()) / RAND_MAX) + 0.4;
    float t = (float(rand()) / RAND_MAX) * 360;
    
    positions[i] = glm::vec4(
      float(rand()) / RAND_MAX,
      100 + float(rand()) / RAND_MAX - 0.5f,
      float(rand()) / RAND_MAX - 0.5f,
      float(rand()) / RAND_MAX);
    velocities[i] =     glm::vec3(0,0,0);
    accelerations[i] =  glm::vec3(0,0,0);
  }
  
  glGenTextures(1, &_acceleration_texture_to_sample);
  glBindTexture(GL_TEXTURE_2D, _acceleration_texture_to_sample);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    size,
    size,
    0,
    GL_RGB,
    GL_FLOAT,
    &accelerations[0]);
  
  // Need mipmap for some reason......
  glGenerateMipmap(GL_TEXTURE_2D);
  
  glGenTextures(1, &_velocity_texture_to_sample);
  glBindTexture(GL_TEXTURE_2D, _velocity_texture_to_sample);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    size,
    size,
    0,
    GL_RGB,
    GL_FLOAT,
    &velocities[0]);
  
  // Need mipmap for some reason......
  glGenerateMipmap(GL_TEXTURE_2D);
  
  glGenTextures(1, &_position_texture_to_sample);
  glBindTexture(GL_TEXTURE_2D, _position_texture_to_sample);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA,
    size,
    size,
    0,
    GL_RGBA,
    GL_FLOAT,
    &positions[0]);
  
  // Need mipmap for some reason......
  glGenerateMipmap(GL_TEXTURE_2D);

  _updateUniformIDs();
}

PointCloudMaterial::~PointCloudMaterial()
{
  glDeleteTextures(1, &_acceleration_texture_to_sample);
  glDeleteTextures(1, &_velocity_texture_to_sample);
  glDeleteTextures(1, &_position_texture_to_sample);
}

//! Swap the current handle with a new one for ping-ponging.
GLuint PointCloudMaterial::swapAccelerationTexture(GLuint texture_ID)
{
  GLuint tmp = _acceleration_texture_to_sample;
  _acceleration_texture_to_sample = texture_ID;
  return tmp;
}

//! Swap the current handle with a new one for ping-ponging.
GLuint PointCloudMaterial::swapVelocityTexture(GLuint texture_ID)
{
  GLuint tmp = _velocity_texture_to_sample;
  _velocity_texture_to_sample = texture_ID;
  return tmp;
}

//! Swap the current handle with a new one for ping-ponging.
GLuint PointCloudMaterial::swapPositionTexture(GLuint texture_ID)
{
  GLuint tmp = _position_texture_to_sample;
  _position_texture_to_sample = texture_ID;
  return tmp;
}

void PointCloudMaterial::_updateUniformIDs()
{
  GLuint program_ID;
  switch (_rendering_properties.shader)
  {
    case ADDITIVE :
      program_ID =
        ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_ADDITIVE");
      break;
    case PHONG :
      program_ID =
        ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_PHONG");
      break;
    default :
      program_ID =
        ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_ADDITIVE");
  }

  glUseProgram(program_ID);
  _acceleration_texture_sampler2D_ID =
    glGetUniformLocation(program_ID, "acceleration_sampler_2D");
  _velocity_texture_sampler2D_ID =
    glGetUniformLocation(program_ID, "velocity_sampler_2D");
  _position_texture_sampler2D_ID =
    glGetUniformLocation(program_ID, "position_sampler_2D");
  _particle_color1_ID =
    glGetUniformLocation(program_ID, "particle_color1");
  _particle_color2_ID =
    glGetUniformLocation(program_ID, "particle_color2");
  _particle_radius_ID =
    glGetUniformLocation(program_ID, "particle_radius");
}

//! Updating the shader parameters.
void PointCloudMaterial::use()
{
  // Update IDs and use shader
  _updateUniformIDs();

  // Bind textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _acceleration_texture_to_sample);
  glUniform1i(_acceleration_texture_sampler2D_ID, 0);
  
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, _velocity_texture_to_sample);
  glUniform1i(_velocity_texture_sampler2D_ID, 1);
  
  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_2D, _position_texture_to_sample);
  glUniform1i(_position_texture_sampler2D_ID, 2);

  // Write uniforms
  glUniform3f(
    _particle_color1_ID,
    _rendering_properties.particle_color1.r,
    _rendering_properties.particle_color1.g,
    _rendering_properties.particle_color1.b);
  glUniform3f(
    _particle_color2_ID,
    _rendering_properties.particle_color2.r,
    _rendering_properties.particle_color2.g,
    _rendering_properties.particle_color2.b);
  glUniform1f(
    _particle_radius_ID,
    _rendering_properties.particle_radius);

  // Set blending mode depending on shader
  if (getProgramID() == 
      ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_PHONG"))
    glDisable(GL_BLEND);
  else if (getProgramID() == 
      ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_ADDITIVE"))
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  }
}

//! Returns the program ID of the shader of the Material.
GLuint PointCloudMaterial::getProgramID() const
{
  GLuint program_ID;
  switch (_rendering_properties.shader)
  {
    case ADDITIVE :
      program_ID =
        ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_ADDITIVE");
      break;
    case PHONG :
      program_ID =
        ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_PHONG");
      break;
    default :
      program_ID =
        ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_ADDITIVE");
  }
  return program_ID;
};

PointCloudRenderingProperties* PointCloudMaterial::getPropertiesPointer()
{
  return &_rendering_properties;
}

