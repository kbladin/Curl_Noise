#ifndef MATERIAL_H
#define MATERIAL_H

#include <gl/glew.h>
#include <gl/glfw3.h>
#include "../ext/glm/include/glm/glm.hpp"

#include "../include/ShaderManager.h"


//! Every Mesh has a material which specifies parameters for shading.
  /*
    This class is extended for every shader program that needs specific shader
    uniforms.
  */
class Material {
public:
  Material(GLuint program_ID);
  virtual ~Material(){};
  virtual void render()const = 0;
  GLuint getProgramID()const;
protected:
  const GLuint program_ID_;
};

//! Every Mesh has a material which specifies parameters for shading.
class PhongMaterial : public Material {
public:
  PhongMaterial();
  ~PhongMaterial(){};
  void render()const;
  
  glm::vec3 diffuse_color_;
  glm::vec3 specular_color_;
  float specularity_;
  int shinyness_;
private:
  GLuint diffuseColor_ID_;
  GLuint specularColor_ID_;
  GLuint specularity_ID_;
  GLuint shinyness_ID_;
};

//! Every Mesh has a material which specifies parameters for shading.
class OneColorMaterial : public Material {
public:
  OneColorMaterial();
  ~OneColorMaterial(){};
  void render()const;
  
  glm::vec3 diffuse_color_;
  private:
  GLuint diffuseColor_ID_;
};

//! Every Mesh has a material which specifies parameters for shading.
class BackgroundMaterial : public Material {
public:
  BackgroundMaterial();
  ~BackgroundMaterial(){};
  void render()const;
private:
};

//! Every Mesh has a material which specifies parameters for shading.
class PointCloudMaterial : public Material {
public:
  PointCloudMaterial(unsigned long size);
  ~PointCloudMaterial();
  GLuint swapAccelerationTexture(GLuint texture_ID);
  GLuint swapVelocityTexture(GLuint texture_ID);
  GLuint swapPositionTexture(GLuint texture_ID);
  GLuint getAccelerationTextureToSample(){return acceleration_texture_to_sample_;};
  GLuint getVelocityTextureToSample(){return velocity_texture_to_sample_;};
  GLuint getPositionTextureToSample(){return position_texture_to_sample_;};
  void render()const;
private:
  GLuint acceleration_texture_sampler1D_ID_;
  GLuint velocity_texture_sampler1D_ID_;
  GLuint position_texture_sampler1D_ID_;
  
  GLuint acceleration_texture_to_sample_;
  GLuint velocity_texture_to_sample_;
  GLuint position_texture_to_sample_;
};

#endif