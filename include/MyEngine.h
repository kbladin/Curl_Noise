#ifndef MY_ENGINE_H
#define MY_ENGINE_H

#include "SimpleGraphicsEngine.h"

using namespace SGE;

//! The actual program extending SimpleGraphicsEngine
/*!
 Here, all objects can be added to the scene_ and input is handled outside of
 SimpleGraphicsEngine.
*/
class MyEngine : public SimpleGraphicsEngine {
public:
  MyEngine();
  ~MyEngine();
  virtual void update();
  static void mouseScrollCallback(GLFWwindow * window, double dx, double dy);
private:
  // Objects to put in the scene
  Object3D* bunny_;
  TriangleMesh* bunny_mesh_;
  PhongMaterial* bunny_material_;
  LightSource* light_;
  LightMesh3D* light_mesh_;
  BoundingBox* bb_;
  
  glm::vec3 rotation_point;
  
  float delay_counter_;
  int frame_counter_;
};

#endif