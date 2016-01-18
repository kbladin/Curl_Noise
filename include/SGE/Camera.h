#ifndef CAMERA_H
#define CAMERA_H

#include "../../include/SGE/Object3D.h"

#include <gl/glew.h>
#include <gl/glfw3.h>

#include "../../ext/glm/include/glm/glm.hpp"
#include "../../ext/glm/include/glm/gtc/matrix_transform.hpp"
#include "../../ext/glm/include/glm/gtx/transform.hpp"

//! A camera defined in 3D space
class AbstractCamera : public Object3D {
public:
  AbstractCamera(GLuint program_ID, GLFWwindow* window);
  virtual void render(glm::mat4 M) = 0;
  void setShader(GLuint program_ID);
  void setNearClippingPlane(float near);
  void setFarClippingPlane(float far);
protected:
  GLuint program_ID_;
  GLuint view_matrix_ID_;
  GLuint projection_matrix_ID_;
  
  glm::mat4 projection_transform_;
  GLFWwindow* window_;

  float near_;
  float far_;
};

//! A perspective camera defined in 3D space
class PerspectiveCamera : public AbstractCamera {
public:
  PerspectiveCamera(
    GLuint program_ID,
    GLFWwindow* window,
    float fov,
    float near,
    float far);
  virtual void render(glm::mat4 M);
  void setFOV(float fov);
private:
  float fov_;
};

//! An orthographic camera defined in 3D space
class OrthoCamera : public AbstractCamera {
public:
  OrthoCamera(
    GLuint program_ID,
    GLFWwindow* window,
    float near,
    float far);
  virtual void render(glm::mat4 M);
};


#endif