#ifndef MATERIAL_H
#define MATERIAL_H

#include "SGE/ShaderManager.h"

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

//! Every Mesh has a material which specifies parameters for shading.
/*
  This class is extended for every shader program that needs specific shader
  uniforms.
*/
class Material {
public:
  Material(GLuint program_ID);
  virtual ~Material(){};
  virtual void use() const = 0;

  // Getters
  GLuint getProgramID() const;
protected:
  GLuint _program_ID;
};

//! Every Mesh has a material which specifies parameters for shading.
class PhongMaterial : public Material {
public:
  PhongMaterial();
  ~PhongMaterial(){};
  void use() const;
  
  // Data
  glm::vec3 diffuse_color;
  glm::vec3 specular_color;
  float     specularity;
  int       shinyness;
private:
  // OpenGL handles
  GLuint _diffuse_color_ID;
  GLuint _specular_color_ID;
  GLuint _specularity_ID;
  GLuint _shinyness_ID;
};

//! Every Mesh has a material which specifies parameters for shading.
class OneColorMaterial : public Material {
public:
  OneColorMaterial();
  ~OneColorMaterial(){};
  void use() const;
  
  // Data
  glm::vec3 diffuse_color;
private:
  // OpenGL handles
  GLuint _diffuse_color_ID;
};

//! Every Mesh has a material which specifies parameters for shading.
class BackgroundMaterial : public Material {
public:
  BackgroundMaterial();
  ~BackgroundMaterial(){};
  void use() const;
private:
};

typedef enum {
  ADDITIVE, PHONG
} PointCloudShader;

struct PointCloudRenderingProperties {
  glm::vec3         particle_color1;
  glm::vec3         particle_color2;
  float             particle_radius;
  PointCloudShader  shader;
};

//! Every Mesh has a material which specifies parameters for shading.
class PointCloudMaterial {
public:
  PointCloudMaterial(unsigned long size);
  ~PointCloudMaterial();
  GLuint swapAccelerationTexture(GLuint texture_ID);
  GLuint swapVelocityTexture(GLuint texture_ID);
  GLuint swapPositionTexture(GLuint texture_ID);
  GLuint getAccelerationTextureToSample(){return _acceleration_texture_to_sample;};
  GLuint getVelocityTextureToSample(){return _velocity_texture_to_sample;};
  GLuint getPositionTextureToSample(){return _position_texture_to_sample;};
  
  PointCloudRenderingProperties* getPropertiesPointer();

  void use();
  GLuint getProgramID() const;
private:
  void _updateUniformIDs();

  // IDs for uniforms that can be changed
  GLuint _particle_color1_ID;
  GLuint _particle_color2_ID;
  GLuint _particle_radius_ID;

  // Textures to sample from, shader handles
  // (in case for example velocity determines color)
  GLuint _acceleration_texture_sampler2D_ID;
  GLuint _velocity_texture_sampler2D_ID;
  GLuint _position_texture_sampler2D_ID;
  
  // Textures to sample from
  GLuint _acceleration_texture_to_sample;
  GLuint _velocity_texture_to_sample;
  GLuint _position_texture_to_sample;

  // Rendering properties that can be changed from the outside since the
  // function getPropertiesPointer() returns a pointer to this object
  PointCloudRenderingProperties _rendering_properties;
};

#endif