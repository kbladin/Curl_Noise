#ifndef SIMPLE_GRAPHICS_ENGINE_H
#define SIMPLE_GRAPHICS_ENGINE_H

#include "../include/PointCloudGPU.h"
#include "../include/ShaderManager.h"
#include "../include/Material.h"
#include "../include/Object3D.h"
#include "../include/Mesh.h"
#include "../include/Camera.h"

#include <vector>
#include <map>

#include "../ext/glm/include/glm/glm.hpp"
#include "../ext/glm/include/glm/gtc/matrix_transform.hpp"
#include "../ext/glm/include/glm/gtx/transform.hpp"
#include <gl/glew.h>
#include <gl/glfw3.h>

//! A light source defined in 3D space
class LightSource : public Object3D {
public:
  LightSource(GLuint program_ID);
  virtual void render(glm::mat4 M);
  
  float intensity_;
  glm::vec3 color_;
private:
  GLuint program_ID_;
  GLuint light_position_ID_;
  GLuint light_intensity_ID_;
  GLuint light_color_ID_;
};

//! This class manages all objects in the engine.
/*!
 This class has the scene_ which can be used to add more objects by adding
 children to the scene.
 The scene_ has some predefined children such as a grid plane and axes.
 Extend this class to create a program to run.
 */
class SimpleGraphicsEngine {
public:
  SimpleGraphicsEngine();
  virtual ~SimpleGraphicsEngine();
  void run();
protected:
  virtual void update() = 0;
  
  // Probably should be private...
  GLFWwindow* window_;
  double dt_;
  Object3D* scene_;

  Object3D* view_space_;
  Object3D* background_space_;
  
  static Object3D* camera_;
  static Object3D* viewspace_ortho_camera_;
  
private:
  bool initialize();
  double time_;
};

#endif