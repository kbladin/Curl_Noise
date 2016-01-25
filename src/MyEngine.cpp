#include "../include/MyEngine.h"

MyBGObject3D::MyBGObject3D()
{
  background_plane_ = new TriangleMesh();
  material_ = new BackgroundMaterial();
  background_plane_->initPlane(
    glm::vec3(0,0,0),
    glm::vec3(0,0,1),
    glm::vec3(10,2,2));
}

MyBGObject3D::~MyBGObject3D()
{
  delete background_plane_;
  delete material_;
}

void MyBGObject3D::render(glm::mat4 M)
{
  material_->use();
  background_plane_->render(M * transform_matrix_, material_->getProgramID());
}

MyObject3D::MyObject3D(const char* file_path)
{
  mesh_ = new TriangleMesh(file_path);
  mesh_->transform_matrix_ = glm::scale(glm::mat4(), 0.5f * glm::vec3(1.0f));
  material_ = new PhongMaterial();
}

MyObject3D::~MyObject3D()
{
  delete mesh_;
  delete material_;
}

void MyObject3D::render(glm::mat4 M)
{
  material_->use();
  mesh_->render(M * transform_matrix_, material_->getProgramID());
}

MyLightSource::MyLightSource()
{
  phong_light_source = new LightSource(
    ShaderManager::instance()->getShader("SHADER_PHONG"));
  particle_light_source = new LightSource(
    ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_PHONG"));
}

MyLightSource::~MyLightSource()
{
  delete phong_light_source;
  delete particle_light_source;
}

void MyLightSource::render(glm::mat4 M)
{
  phong_light_source->render(M * transform_matrix_);
  particle_light_source->render(M * transform_matrix_);
}

void MyLightSource::setIntensity(float intensity)
{
  phong_light_source->setIntensity(intensity);
  particle_light_source->setIntensity(intensity);
}

void MyLightSource::setColor(glm::vec3 color)
{
  phong_light_source->setColor(color);
  particle_light_source->setColor(color);
}

MyEngine::MyEngine(int window_width, int window_height, double time) :
  SimpleGraphicsEngine(time)
{
  window_width_ = window_width;
  window_height_ = window_height; 
  // Now initialize objects in scene
  SimpleGraphicsEngine::initialize();
  // Load shaders
  ShaderManager::instance()->loadShader(
    "SHADER_PHONG",
    "../shaders/phong.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/phong.frag");
  ShaderManager::instance()->loadShader(
    "SHADER_ONE_COLOR",
    "../shaders/one_color.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/one_color.frag" );
  ShaderManager::instance()->loadShader(
    "SHADER_BACKGROUND",
    "../shaders/background.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/background.frag" );
  ShaderManager::instance()->loadShader(
    "SHADER_UPDATE_POINT_CLOUD_POSITIONS",
    "../shaders/point_cloud_programs/quad_passthrough.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/point_cloud_programs/update_positions.frag");
  ShaderManager::instance()->loadShader(
    "SHADER_UPDATE_POINT_CLOUD_VELOCITIES",
    "../shaders/point_cloud_programs/quad_passthrough.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/point_cloud_programs/update_velocities.frag");
  ShaderManager::instance()->loadShader(
    "SHADER_UPDATE_POINT_CLOUD_ACCELERATIONS",
    "../shaders/point_cloud_programs/quad_passthrough.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/point_cloud_programs/update_accelerations_curl_noise.frag");
  ShaderManager::instance()->loadShader(
    "SHADER_RENDER_POINT_CLOUD_PHONG",
    "../shaders/point_cloud_programs/render_particles_phong.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/point_cloud_programs/render_particles_phong.frag");
ShaderManager::instance()->loadShader(
    "SHADER_RENDER_POINT_CLOUD_ADDITIVE",
    "../shaders/point_cloud_programs/render_particles_additive.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/point_cloud_programs/render_particles_additive.frag");
  
  // Create cameras
  basic_cam_ = new PerspectiveCamera(
    ShaderManager::instance()->getShader("SHADER_PHONG"),
    window_width,
    window_height,
    45,
    0.01,
    100);
  one_color_cam_ = new PerspectiveCamera(
    ShaderManager::instance()->getShader("SHADER_ONE_COLOR"),
    window_width,
    window_height,
    45,
    0.01,
    100);
  background_ortho_cam_ = new OrthoCamera(
    ShaderManager::instance()->getShader("SHADER_BACKGROUND"),
    window_width,
    window_height,
    100,
    100);
  point_cloud_phong_cam_ = new PerspectiveCamera(
    ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_PHONG"),
    window_width,
    window_height,
    45,
    0.01,
    100);
  point_cloud_additive_cam_ = new PerspectiveCamera(
    ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_ADDITIVE"),
    window_width,
    window_height,
    45,
    0.01,
    100);
  
  // Create objects
  background_ = new MyBGObject3D();
  sphere_ = new MyObject3D("../data/meshes/icosphere.obj");
  point_cloud_ = new ParticleSystem(200000);
  lamp_ = new MyLightSource();
  
  // Connect nodes
  camera_->addChild(basic_cam_);
  camera_->addChild(one_color_cam_);
  camera_->addChild(point_cloud_phong_cam_);
  camera_->addChild(point_cloud_additive_cam_);
  viewspace_ortho_camera_->addChild(background_ortho_cam_);

  // Add to scene
  scene_->addChild(point_cloud_);
  scene_->addChild(sphere_);
  scene_->addChild(lamp_);
  background_space_->addChild(background_);

  // Set camera transform
  camera_->transform_matrix_ = glm::translate(glm::vec3(0.0f,0.0f,-3.0f));
  camera_->transform_matrix_ =
    camera_->transform_matrix_ *
    glm::rotate(glm::mat4(), 0.3f, glm::vec3(1,0,0));

  // Set lamp transform
  lamp_->transform_matrix_ = glm::translate(glm::vec3(3.0f,3.0f,3.0f));
}

MyEngine::~MyEngine()
{
  delete point_cloud_;
  
  delete basic_cam_;
  delete one_color_cam_;
  delete viewspace_ortho_camera_;
  delete background_ortho_cam_;
  delete point_cloud_phong_cam_;
  delete point_cloud_additive_cam_;
  
  delete grid_mesh_material_;

  delete background_;
  delete sphere_;
  delete lamp_;
}

void MyEngine::update(float time)
{
  dt_ = time - time_;
  time_ = time;

  SimpleGraphicsEngine::update();
  
  // Update particle system
  if (mouse_down_)
    updateParticleEmitterPosition();
  point_cloud_->update(dt_);
}

void MyEngine::mousePosCallback(double x, double y)
{
  mouse_x_ = x;
  mouse_y_ = y;
}

void MyEngine::mouseButtonPress()
{
  mouse_down_ = true;
}

void MyEngine::mouseButtonRelease()
{
  mouse_down_ = false;
}

void MyEngine::mouseScrollCallback(double dx, double dy)
{
  camera_->transform_matrix_ =
    camera_->transform_matrix_ *
    glm::rotate(glm::mat4(), 0.1f * float(dx), glm::vec3(0,1,0));
}

void MyEngine::keyCallback(
  int key,
  int scancode,
  int action,
  int mods)
{

}

ParticleSystemProperties* MyEngine::getParticleSystemPropertiesPointer()
{
  return point_cloud_->getPropertiesPointer();
}

PointCloudRenderingProperties* MyEngine::getPointCloudRenderingPropertiesPointer()
{
  return point_cloud_->getPointCloudRenderingPropertiesPointer();
}

void MyEngine::setWindowResolution(int width, int height)
{
  SimpleGraphicsEngine::setWindowResolution(width, height);
  basic_cam_->setResolution(width, height);
  one_color_cam_->setResolution(width, height);
  point_cloud_phong_cam_->setResolution(width, height);
  point_cloud_additive_cam_->setResolution(width, height);
  background_ortho_cam_->setResolution(width, height);
}

void MyEngine::updateParticleEmitterPosition()
{
  // Unproject camera here

  // The unProject() function returns a vector in world-space which
  // defines a direction out of the frustum depending on which pixel
  // we shoot the ray from. "from" will be on the near-viewplane
  // and "to" will be on the far-viewplane.
  
  glm::mat4 P = basic_cam_->getProjectionTransform();
  glm::mat4 V = camera_->transform_matrix_;

  glm::vec3 from = glm::unProject(
    glm::vec3(mouse_x_, window_height_ - mouse_y_, 0.0f),
    V,
    P,
    glm::vec4(0, 0, window_width_, window_height_));
  glm::vec3 to = glm::unProject(
    glm::vec3(mouse_x_, window_height_ - mouse_y_, 1.0f),
    V,
    P,
    glm::vec4(0, 0, window_width_, window_height_));

  ParticleSystemProperties* particle_properties =
    point_cloud_->getPropertiesPointer();

  particle_properties->emitter_position = from + glm::normalize((to - from)) * 3.5f;
}