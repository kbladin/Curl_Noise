#include "Material.h"

//! Create a material which is bound to a specific shader.
/*!
  \param program_ID can be one of the shaders in ShaderManager.
*/
Material::Material(GLuint program_ID) : program_ID_(program_ID)
{

}

//! Returns the program ID of the shader of the Material.
GLuint Material::getProgramID() const
{
  return program_ID_;
};

//! Create a material which is bound to the Phong shader.
PhongMaterial::PhongMaterial() :
  Material(ShaderManager::instance()->getShader("SHADER_PHONG"))
{
  diffuse_color_ = glm::vec3(1.0, 1.0, 1.0);
  specular_color_ = glm::vec3(1.0, 1.0, 1.0);
  specularity_ = 0.5;
  shinyness_ = 16;
  
  glUseProgram(program_ID_);
  diffuseColor_ID_ =  glGetUniformLocation(program_ID_, "material_diffise_color");
  specularColor_ID_ = glGetUniformLocation(program_ID_, "material_specularColor");
  specularity_ID_ =   glGetUniformLocation(program_ID_, "material_specularity");
  shinyness_ID_ =     glGetUniformLocation(program_ID_, "material_shinyness");
}

//! Updating the shader parameters.
/*!
  This function is automatically called when a mesh with this material is
  rendered.
*/
void PhongMaterial::use() const
{
  // Use our shader
  glUseProgram(program_ID_);
  glUniform3f(
    diffuseColor_ID_,
    diffuse_color_.r,
    diffuse_color_.g,
    diffuse_color_.b);
  glUniform3f(
    specularColor_ID_,
    specular_color_.r,
    specular_color_.g,
    specular_color_.b);
  glUniform1f(specularity_ID_, specularity_);
  glUniform1i(shinyness_ID_, shinyness_);
}

//! Create a material which is bound to the OneColor shader.
OneColorMaterial::OneColorMaterial() :
  Material(ShaderManager::instance()->getShader("SHADER_ONE_COLOR"))
{
  diffuse_color_ = glm::vec3(1.0, 1.0, 1.0);
  
  glUseProgram(program_ID_);
  diffuseColor_ID_ = glGetUniformLocation(program_ID_, "material_diffise_color");
}

//! Updating the shader parameters.
/*!
  This function is automatically called when a mesh with this material is
  rendered.
*/
void OneColorMaterial::use() const
{
  // Use our shader
  glUseProgram(program_ID_);
  glUniform3f(
    diffuseColor_ID_,
    diffuse_color_.r,
    diffuse_color_.g,
    diffuse_color_.b);
}

//! Create a material which is bound to the Background shader.
BackgroundMaterial::BackgroundMaterial() :
  Material(ShaderManager::instance()->getShader("SHADER_BACKGROUND"))
{
  glUseProgram(program_ID_);
}

//! Updating the shader parameters.
/*!
  This function is automatically called when a mesh with this material is
  rendered.
*/
void BackgroundMaterial::use() const
{
  // Use our shader
  glUseProgram(program_ID_);
}

//! Create a material which is bound to the UpdatePointCloud shader.
PointCloudMaterial::PointCloudMaterial(unsigned long size)
{
  rendering_properties_.particle_color1 = glm::vec3(0.5,0.5,1);
  rendering_properties_.particle_color2 = glm::vec3(1,0.5,0.2);
  rendering_properties_.particle_radius = 2;
  rendering_properties_.shader = ADDITIVE;

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
  
  glGenTextures(1, &acceleration_texture_to_sample_);
  glBindTexture(GL_TEXTURE_2D, acceleration_texture_to_sample_);
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
  
  glGenTextures(1, &velocity_texture_to_sample_);
  glBindTexture(GL_TEXTURE_2D, velocity_texture_to_sample_);
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
  
  glGenTextures(1, &position_texture_to_sample_);
  glBindTexture(GL_TEXTURE_2D, position_texture_to_sample_);
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

  updateUniformIDs();
}

PointCloudMaterial::~PointCloudMaterial()
{
  glDeleteTextures(1, &acceleration_texture_to_sample_);
  glDeleteTextures(1, &velocity_texture_to_sample_);
  glDeleteTextures(1, &position_texture_to_sample_);
}

//! Swap the current handle with a new one for ping-ponging.
GLuint PointCloudMaterial::swapAccelerationTexture(GLuint texture_ID)
{
  GLuint tmp = acceleration_texture_to_sample_;
  acceleration_texture_to_sample_ = texture_ID;
  return tmp;
}

//! Swap the current handle with a new one for ping-ponging.
GLuint PointCloudMaterial::swapVelocityTexture(GLuint texture_ID)
{
  GLuint tmp = velocity_texture_to_sample_;
  velocity_texture_to_sample_ = texture_ID;
  return tmp;
}

//! Swap the current handle with a new one for ping-ponging.
GLuint PointCloudMaterial::swapPositionTexture(GLuint texture_ID)
{
  GLuint tmp = position_texture_to_sample_;
  position_texture_to_sample_ = texture_ID;
  return tmp;
}

void PointCloudMaterial::updateUniformIDs()
{
  GLuint program_ID;
  switch (rendering_properties_.shader)
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
  acceleration_texture_sampler2D_ID_ =
    glGetUniformLocation(program_ID, "acceleration_sampler_2D");
  velocity_texture_sampler2D_ID_ =
    glGetUniformLocation(program_ID, "velocity_sampler_2D");
  position_texture_sampler2D_ID_ =
    glGetUniformLocation(program_ID, "position_sampler_2D");
  particle_color1_ID_ =
    glGetUniformLocation(program_ID, "particle_color1");
  particle_color2_ID_ =
    glGetUniformLocation(program_ID, "particle_color2");
  particle_radius_ID_ =
    glGetUniformLocation(program_ID, "particle_radius");
}

//! Updating the shader parameters.
void PointCloudMaterial::use()
{
  // Update IDs and use shader
  updateUniformIDs();

  // Bind textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, acceleration_texture_to_sample_);
  glUniform1i(acceleration_texture_sampler2D_ID_, 0);
  
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, velocity_texture_to_sample_);
  glUniform1i(velocity_texture_sampler2D_ID_, 1);
  
  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_2D, position_texture_to_sample_);
  glUniform1i(position_texture_sampler2D_ID_, 2);

  // Write uniforms
  glUniform3f(
    particle_color1_ID_,
    rendering_properties_.particle_color1.r,
    rendering_properties_.particle_color1.g,
    rendering_properties_.particle_color1.b);
  glUniform3f(
    particle_color2_ID_,
    rendering_properties_.particle_color2.r,
    rendering_properties_.particle_color2.g,
    rendering_properties_.particle_color2.b);
  glUniform1f(
    particle_radius_ID_,
    rendering_properties_.particle_radius);

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
  switch (rendering_properties_.shader)
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
  return &rendering_properties_;
}

