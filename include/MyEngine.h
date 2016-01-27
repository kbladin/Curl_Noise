#ifndef MY_ENGINE_H
#define MY_ENGINE_H

#include "SGE/SimpleGraphicsEngine.h"
#include "ParticleSystem.h"
#include "AntGui.h"

#include <iostream>
#include <sstream>

#include <gl/glew.h>


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

class MyLightSource : public Object3D {
public:
  MyLightSource();
  ~MyLightSource();
  void render(glm::mat4 M);
  void setIntensity(float intensity);
  void setColor(glm::vec3 color);
protected:
  // Need one light source for each shader it is used in
  LightSource* phong_light_source;
  LightSource* particle_light_source;
};

//! The actual program extending SimpleGraphicsEngine
/*!
  Here, all objects can be added to the scene_ and input is handled outside of
  SimpleGraphicsEngine.
*/
class MyEngine : public SimpleGraphicsEngine {
public:
  MyEngine(int window_width, int window_height, double time);
  ~MyEngine();
  void update(float time);

  void mousePosCallback(double x, double y);
  void mouseButtonPress();
  void mouseButtonRelease();
  void mouseScrollCallback(double dx, double dy);
  void keyCallback(
    int key,
    int scancode,
    int action,
    int mods);

  ParticleSystemProperties* getParticleSystemPropertiesPointer();
  PointCloudRenderingProperties* getPointCloudRenderingPropertiesPointer();

  void setWindowResolution(int width, int height);
private:
  void updateParticleEmitterPosition();
  // Objects to put in the scene
  MyBGObject3D* background_;
  MyObject3D* sphere_;
  ParticleSystem* point_cloud_;
  MyLightSource* lamp_;
  
  // One camera for each render shader
  // (probably a bit faster than to change shader for one camera)
  PerspectiveCamera* basic_cam_;
  PerspectiveCamera* one_color_cam_;
  PerspectiveCamera* point_cloud_phong_cam_;
  PerspectiveCamera* point_cloud_additive_cam_;
  OrthoCamera* background_ortho_cam_;
  
  // Materials
  BackgroundMaterial* background_material_;
  OneColorMaterial* grid_mesh_material_;

  // User interfacing
  float mouse_x_;
  float mouse_y_;
  bool mouse_down_;
};

#endif