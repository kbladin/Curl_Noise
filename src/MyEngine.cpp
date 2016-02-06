#include "MyEngine.h"

MyBGObject3D::MyBGObject3D()
{
  _background_plane = new TriangleMesh();
  _material = new BackgroundMaterial();
  _background_plane->initPlane(
    glm::vec3(0,0,0),
    glm::vec3(0,0,1),
    glm::vec3(10,2,2));
}

MyBGObject3D::~MyBGObject3D()
{
  delete _background_plane;
  delete _material;
}

void MyBGObject3D::render(glm::mat4 M)
{
  _material->use();
  _background_plane->render(M * transform_matrix, _material->getProgramID());
}

FieldBlockerSphere::FieldBlockerSphere()
{
  // Create the mesh
  _mesh = new TriangleMesh("../data/meshes/icosphere.obj");
  _mesh->transform_matrix = glm::scale(glm::mat4(), 0.5f * glm::vec3(1.0f));
  _material = new PhongMaterial();
}

FieldBlockerSphere::~FieldBlockerSphere()
{
  delete _mesh;
  delete _material;
}

void FieldBlockerSphere::render(glm::mat4 M)
{
  _material->use();
  // Bind shader to send sphere position and size
  glUseProgram(ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_VELOCITIES"));
  glUniform3f(glGetUniformLocation(
    ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_VELOCITIES"),
    "sphere_position"), 0,0,0);
  glUniform1f(glGetUniformLocation(
    ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_VELOCITIES"),
    "sphere_radius"), 0.5);

  glUseProgram(ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_VELOCITIES_VORTEX"));
  glUniform3f(glGetUniformLocation(
    ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_VELOCITIES_VORTEX"),
    "sphere_position"), 0,0,0);
  glUniform1f(glGetUniformLocation(
    ShaderManager::instance()->getShader("SHADER_UPDATE_POINT_CLOUD_VELOCITIES_VORTEX"),
    "sphere_radius"), 0.5);
  _mesh->render(M * transform_matrix, _material->getProgramID());
}

MyLightSource::MyLightSource()
{
  _phong_light_source = new LightSource(
    ShaderManager::instance()->getShader("SHADER_PHONG"),
    10,
    glm::vec3(1,1,1));
  _particle_light_source = new LightSource(
    ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_PHONG"),
    10,
    glm::vec3(1,1,1));
}

MyLightSource::~MyLightSource()
{
  delete _phong_light_source;
  delete _particle_light_source;
}

void MyLightSource::render(glm::mat4 M)
{
  _phong_light_source->render(M * transform_matrix);
  _particle_light_source->render(M * transform_matrix);
}

void MyLightSource::setIntensity(float intensity)
{
  _phong_light_source->setIntensity(intensity);
  _particle_light_source->setIntensity(intensity);
}

void MyLightSource::setColor(glm::vec3 color)
{
  _phong_light_source->setColor(color);
  _particle_light_source->setColor(color);
}

MyEngine::MyEngine(int window_width, int window_height, double time) :
  SimpleGraphicsEngine()
{
  SimpleGraphicsEngine::setWindowResolution(window_width, window_height);
  _time = time;
  // Load shaders
  // Standard rendering shaders
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

  // Point cloud update programs
  ShaderManager::instance()->loadShader(
    "SHADER_UPDATE_POINT_CLOUD_POSITIONS",
    "../shaders/point_cloud_programs/quad_passthrough.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/point_cloud_programs/update_positions_and_die.frag");
  ShaderManager::instance()->loadShader(
    "SHADER_UPDATE_POINT_CLOUD_VELOCITIES",
    "../shaders/point_cloud_programs/quad_passthrough.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/point_cloud_programs/update_velocities_curl_noise.frag");
  ShaderManager::instance()->loadShader(
    "SHADER_UPDATE_POINT_CLOUD_VELOCITIES_VORTEX",
    "../shaders/point_cloud_programs/quad_passthrough.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/point_cloud_programs/update_velocities_curl_noise_vortex.frag");
  ShaderManager::instance()->loadShader(
    "SHADER_UPDATE_POINT_CLOUD_ACCELERATIONS_GRAVITY",
    "../shaders/point_cloud_programs/quad_passthrough.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/point_cloud_programs/update_accelerations_gravity.frag");
  ShaderManager::instance()->loadShader(
    "SHADER_UPDATE_POINT_CLOUD_VELOCITIES_STEP",
    "../shaders/point_cloud_programs/quad_passthrough.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/point_cloud_programs/update_velocities_step.frag");
  ShaderManager::instance()->loadShader(
    "SHADER_UPDATE_POINT_CLOUD_POSITIONS_STEP",
    "../shaders/point_cloud_programs/quad_passthrough.vert",
    NULL,
    NULL,
    NULL,
    "../shaders/point_cloud_programs/update_positions_step.frag");
  
  // Point cloud rendering programs
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
  _basic_cam = new PerspectiveCamera(
    ShaderManager::instance()->getShader("SHADER_PHONG"),
    window_width,
    window_height,
    45,
    0.01,
    100);
  _one_color_cam = new PerspectiveCamera(
    ShaderManager::instance()->getShader("SHADER_ONE_COLOR"),
    window_width,
    window_height,
    45,
    0.01,
    100);
  _background_ortho_cam = new OrthoCamera(
    ShaderManager::instance()->getShader("SHADER_BACKGROUND"),
    window_width,
    window_height,
    100,
    100);
  _point_cloud_phong_cam = new PerspectiveCamera(
    ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_PHONG"),
    window_width,
    window_height,
    45,
    0.01,
    100);
  _point_cloud_additive_cam = new PerspectiveCamera(
    ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD_ADDITIVE"),
    window_width,
    window_height,
    45,
    0.01,
    100);
  
  // Create objects
  _background = new MyBGObject3D();
  _sphere = new FieldBlockerSphere();
  _point_cloud = new ParticleSystem(200000, CURL_NOISE);
  _lamp = new MyLightSource();
  
  // Connect nodes
  camera->addChild(_basic_cam);
  camera->addChild(_one_color_cam);
  camera->addChild(_point_cloud_phong_cam);
  camera->addChild(_point_cloud_additive_cam);
  viewspace_ortho_camera->addChild(_background_ortho_cam);

  // Add to scene
  scene->addChild(_point_cloud);
  scene->addChild(_sphere);
  scene->addChild(_lamp);
  background_space->addChild(_background);

  // Set camera transform
  camera->transform_matrix = glm::translate(glm::vec3(0.0f,0.0f,-3.0f));
  camera->transform_matrix =
    camera->transform_matrix *
    glm::rotate(glm::mat4(), 0.3f, glm::vec3(1,0,0));

  // Set lamp transform
  _lamp->transform_matrix = glm::translate(glm::vec3(3.0f,3.0f,3.0f));
}

MyEngine::~MyEngine()
{
  delete _point_cloud;
  
  delete _basic_cam;
  delete _one_color_cam;
  delete _background_ortho_cam;
  delete _point_cloud_phong_cam;
  delete _point_cloud_additive_cam;
  
  delete _grid_mesh_material;

  delete _background;
  delete _sphere;
  delete _lamp;
}

void MyEngine::update(float time)
{
  _dt = time - _time;
  _time = time;

  // Update particle system
  if (_mouse_down)
    _updateParticleEmitterPosition();
  _point_cloud->update(_dt);

  // Render
  SimpleGraphicsEngine::render();
}

void MyEngine::mousePosCallback(double x, double y)
{
  _mouse_x = x;
  _mouse_y = y;
}

void MyEngine::mouseButtonPress()
{
  _mouse_down = true;
}

void MyEngine::mouseButtonRelease()
{
  _mouse_down = false;
}

void MyEngine::mouseScrollCallback(double dx, double dy)
{
  camera->transform_matrix =
    camera->transform_matrix *
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
  return _point_cloud->getPropertiesPointer();
}

PointCloudRenderingProperties* MyEngine::getPointCloudRenderingPropertiesPointer()
{
  return _point_cloud->getPointCloudRenderingPropertiesPointer();
}

float MyEngine::getDt()
{
  return _dt;
}

void MyEngine::setWindowResolution(int width, int height)
{
  SimpleGraphicsEngine::setWindowResolution(width, height);
  _basic_cam->setResolution(width, height);
  _one_color_cam->setResolution(width, height);
  _point_cloud_phong_cam->setResolution(width, height);
  _point_cloud_additive_cam->setResolution(width, height);
  _background_ortho_cam->setResolution(width, height);
}

void MyEngine::_updateParticleEmitterPosition()
{
  // The unProject() function returns a vector in world-space which
  // defines a direction out of the frustum depending on which pixel
  // we shoot the ray from. "from" will be on the near-viewplane
  // and "to" will be on the far-viewplane.
  
  glm::mat4 P = _basic_cam->getProjectionTransform();
  glm::mat4 V = camera->transform_matrix;

  int width = SimpleGraphicsEngine::getWindowWidth();
  int height = SimpleGraphicsEngine::getWindowHeight();

  glm::vec3 from = glm::unProject(
    glm::vec3(_mouse_x, height - _mouse_y, 0.0f),
    V,
    P,
    glm::vec4(0, 0, width, height));
  glm::vec3 to = glm::unProject(
    glm::vec3(_mouse_x, height - _mouse_y, 1.0f),
    V,
    P,
    glm::vec4(0, 0, width, height));

  ParticleSystemProperties* particle_properties =
    _point_cloud->getPropertiesPointer();

  particle_properties->emitter_position = from + glm::normalize((to - from)) * 3.5f;
}