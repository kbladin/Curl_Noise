#ifndef CAMERA_H
#define CAMERA_H

#include "../include/Object3D.h"

#include <gl/glew.h>
#include <gl/glfw3.h>

#include "../ext/glm/include/glm/glm.hpp"
#include "../ext/glm/include/glm/gtc/matrix_transform.hpp"
#include "../ext/glm/include/glm/gtx/transform.hpp"

//! A camera defined in 3D space
class AbstractCamera : public Object3D {
public:
  AbstractCamera(GLuint program_ID, GLFWwindow* window);
  virtual void render(glm::mat4 M) = 0;
  void setShader(GLuint program_ID);
protected:
  GLuint program_ID_;
  GLuint view_matrix_ID_;
  GLuint projection_matrix_ID_;
  
  glm::mat4 projection_transform_;
  GLFWwindow* window_;
};

//! A perspective camera defined in 3D space
class PerspectiveCamera : public AbstractCamera {
public:
  PerspectiveCamera(GLuint program_ID, GLFWwindow* window);
  virtual void render(glm::mat4 M);
};

//! An orthographic camera defined in 3D space
class OrthoCamera : public AbstractCamera {
public:
  OrthoCamera(GLuint program_ID, GLFWwindow* window);
  virtual void render(glm::mat4 M);
};


#endif