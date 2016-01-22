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
  background_plane_->render(M, material_->getProgramID());
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
  mesh_->render(M, material_->getProgramID());
}

MyEngine::MyEngine(double time) : SimpleGraphicsEngine(time)
{
  // First initialize this class to create OpenGL context with glfw
  initialize();
  // Now initialize objects in scene
  SimpleGraphicsEngine::initialize();
  // Load shaders
  ShaderManager::instance()->loadShader(
    "SHADER_PHONG",
    "../shaders/simple.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/simple.frag");
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
    "SHADER_RENDER_POINT_CLOUD",
    "../shaders/point_cloud_programs/render.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/point_cloud_programs/render.frag");
  
  // Create cameras
  basic_cam_ = new PerspectiveCamera(
    ShaderManager::instance()->getShader("SHADER_PHONG"),
    100,
    100,
    45,
    0.01,
    100);
  one_color_cam_ = new PerspectiveCamera(
    ShaderManager::instance()->getShader("SHADER_ONE_COLOR"),
    100,
    100,
    45,
    0.01,
    100);
  background_ortho_cam_ = new OrthoCamera(
    ShaderManager::instance()->getShader("SHADER_BACKGROUND"),
    100,
    100,
    100,
    100);
  point_cloud_cam_ = new PerspectiveCamera(
    ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD"),
    100,
    100,
    45,
    0.01,
    100);
  
  // Create objects
  background_ = new MyBGObject3D();
  sphere_ = new MyObject3D("../data/meshes/icosphere.obj");
  point_cloud_ = new ParticleSystem(200000);
  
  // Connect nodes
  camera_->addChild(basic_cam_);
  camera_->addChild(one_color_cam_);
  camera_->addChild(point_cloud_cam_);
  viewspace_ortho_camera_->addChild(background_ortho_cam_);

  // Add to scene
  scene_->addChild(point_cloud_);
  scene_->addChild(sphere_);
  background_space_->addChild(background_);

  // Set camera transform
  camera_->transform_matrix_ = glm::translate(glm::vec3(0.0f,0.0f,-3.0f));
  camera_->transform_matrix_ =
    camera_->transform_matrix_ *
    glm::rotate(glm::mat4(), 0.3f, glm::vec3(1,0,0));
}

MyEngine::~MyEngine()
{
  delete point_cloud_;
  
  delete basic_cam_;
  delete one_color_cam_;
  delete viewspace_ortho_camera_;
  delete background_ortho_cam_;
  delete point_cloud_cam_;
  
  delete grid_mesh_material_;

  delete background_;
  delete sphere_;
}

void MyEngine::update(int width, int height, float time)
{
  dt_ = time - time_;
  time_ = time;
  
  basic_cam_->setResolution(width, height);
  one_color_cam_->setResolution(width, height);
  point_cloud_cam_->setResolution(width, height);
  background_ortho_cam_->setResolution(width, height);

  SimpleGraphicsEngine::update(width, height);
  
  // Update particle system
  point_cloud_->update(dt_);
}

void MyEngine::mousePosCallback(double x, double y)
{

}

void MyEngine::mouseButtonCallback(int button, int action, int mods)
{

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