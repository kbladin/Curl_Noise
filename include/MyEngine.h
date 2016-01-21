#ifndef MY_ENGINE_H
#define MY_ENGINE_H

#include "../include/SGE/SimpleGraphicsEngine.h"
#include "../include/ParticleSystem.h"

#include <gl/glfw3.h>

class MyBGObject3D : public Object3D {
public:
  MyBGObject3D();
  ~MyBGObject3D();
  void render(glm::mat4 M);
protected:
  BackgroundMaterial* material_;
  TriangleMesh* background_plane_;
};

class MyObject3D : public Object3D {
public:
  MyObject3D(const char* file_path);
  ~MyObject3D();
  void render(glm::mat4 M);
protected:
  Material* material_;
  TriangleMesh* mesh_;
};

//! The actual program extending SimpleGraphicsEngine
/*!
 Here, all objects can be added to the scene_ and input is handled outside of
 SimpleGraphicsEngine.
*/
class MyEngine : public SimpleGraphicsEngine {
public:
  MyEngine();
  ~MyEngine();
  virtual void run();
  void update();
  static void mouseScrollCallback(GLFWwindow * window, double dx, double dy);
private:
  virtual bool initialize();
  GLFWwindow* window_;

  // Objects to put in the scene
  MyBGObject3D* background_;
  MyObject3D* sphere_;
  ParticleSystem* point_cloud_;

  float delay_counter_;
  int frame_counter_;
  
  // One camera for each render shader
  // (probably a bit faster than to change shader for one camera)
  PerspectiveCamera* basic_cam_;
  PerspectiveCamera* one_color_cam_;
  PerspectiveCamera* point_cloud_cam_;
  OrthoCamera* background_ortho_cam_;
  
  // Materials
  BackgroundMaterial* background_material_;
  OneColorMaterial* grid_mesh_material_;
};

#endif