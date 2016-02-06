#ifndef MY__engineH
#define MY__engineH

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
private:
  BackgroundMaterial* _material;
  TriangleMesh*       _background_plane;
};

class FieldBlockerSphere : public Object3D {
public:
  FieldBlockerSphere();
  ~FieldBlockerSphere();
  void render(glm::mat4 M);
private:
  Material*     _material;
  TriangleMesh* _mesh;
  
  float _radius;
};

class MyLightSource : public Object3D {
public:
  MyLightSource();
  ~MyLightSource();
  void render(glm::mat4 M);

  // Setters
  void setIntensity(float intensity);
  void setColor(glm::vec3 color);
private:
  // Need one light source for each shader it is used in
  LightSource* _phong_light_source;
  LightSource* _particle_light_source;
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

  // Getters
  ParticleSystemProperties* getParticleSystemPropertiesPointer();
  PointCloudRenderingProperties* getPointCloudRenderingPropertiesPointer();
  float getDt();

  // Setters
  void setWindowResolution(int width, int height);
private:
  void _updateParticleEmitterPosition();
  // Objects to put in the scene
  MyBGObject3D*       _background;
  FieldBlockerSphere* _sphere;
  ParticleSystem*     _point_cloud;
  MyLightSource*      _lamp;
  
  // One camera for each render shader
  // (probably a bit faster than to change shader for one camera)
  PerspectiveCamera*  _basic_cam;
  PerspectiveCamera*  _one_color_cam;
  PerspectiveCamera*  _point_cloud_phong_cam;
  PerspectiveCamera*  _point_cloud_additive_cam;
  OrthoCamera*        _background_ortho_cam;
  
  // Materials
  BackgroundMaterial* _background_material;
  OneColorMaterial*   _grid_mesh_material;

  // User interfacing
  float _mouse_x;
  float _mouse_y;
  bool  _mouse_down;

  // Time
  float _time;
  float _dt;
};

#endif