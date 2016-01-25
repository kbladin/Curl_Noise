#ifndef MATERIAL_H
#define MATERIAL_H

#include "../include/SGE/ShaderManager.h"

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
  GLuint getProgramID() const;
protected:
  GLuint program_ID_;
};

//! Every Mesh has a material which specifies parameters for shading.
class PhongMaterial : public Material {
public:
  PhongMaterial();
  ~PhongMaterial(){};
  void use() const;
  
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
  void use() const;
  
  glm::vec3 diffuse_color_;
private:
  GLuint diffuseColor_ID_;
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
  glm::vec3 particle_color;
  float particle_radius;
  PointCloudShader shader;
};

//! Every Mesh has a material which specifies parameters for shading.
class PointCloudMaterial {
public:
  PointCloudMaterial(unsigned long size);
  ~PointCloudMaterial();
  GLuint swapAccelerationTexture(GLuint texture_ID);
  GLuint swapVelocityTexture(GLuint texture_ID);
  GLuint swapPositionTexture(GLuint texture_ID);
  GLuint getAccelerationTextureToSample(){return acceleration_texture_to_sample_;};
  GLuint getVelocityTextureToSample(){return velocity_texture_to_sample_;};
  GLuint getPositionTextureToSample(){return position_texture_to_sample_;};
  
  PointCloudRenderingProperties* getPropertiesPointer();

  void use();
  GLuint getProgramID() const;
private:
  void updateUniformIDs();

  // IDs for uniforms that can be changed
  GLuint particle_color_ID_;
  GLuint particle_radius_ID_;

  // Textures to sample from
  GLuint acceleration_texture_sampler2D_ID_;
  GLuint velocity_texture_sampler2D_ID_;
  GLuint position_texture_sampler2D_ID_;
  
  // Textures to render to
  GLuint acceleration_texture_to_sample_;
  GLuint velocity_texture_to_sample_;
  GLuint position_texture_to_sample_;

  // Rendering properties that can be changed from the outside since the
  // function getPropertiesPointer() returns a pointer to this object
  PointCloudRenderingProperties rendering_properties_;
};

#endif