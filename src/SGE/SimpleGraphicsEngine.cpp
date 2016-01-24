#include "../../include/SGE/SimpleGraphicsEngine.h"

//! Creates a LightSource bound to a specific shader.
/*!
  \param program_ID is the shader program that this LightSource will be bound
  to.
*/
LightSource::LightSource(GLuint program_ID)
{
  intensity_ = 10.0f;
  color_ = glm::vec3(1.0, 1.0, 1.0);
  
  program_ID_ = program_ID;
  
  glUseProgram(program_ID_);
  light_position_ID_ = glGetUniformLocation(program_ID_, "lightPosition");
  light_intensity_ID_ = glGetUniformLocation(program_ID_, "lightIntensity");
  light_color_ID_ = glGetUniformLocation(program_ID_, "lightColor");
}

void LightSource::setIntensity(float intensity)
{
  intensity_ = intensity;
}

void LightSource::setColor(glm::vec3 color)
{
  color_ = color;
}

//! Render the LightSource.
/*!
  Setting all the uniforms of the lightsource for rendering.
  \param M is the transformation matrix of the parent.
*/
void LightSource::render(glm::mat4 M)
{
  Object3D::render(M * transform_matrix_);
  
  glm::vec4 position = M * transform_matrix_ * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  
  glUseProgram(program_ID_);
  glUniform3f(light_position_ID_,position.x,position.y, position.z);
  glUniform1f(light_intensity_ID_, intensity_);
  glUniform3f(light_color_ID_, color_.r, color_.g, color_.b);
}

Object3D* SimpleGraphicsEngine::camera_;
Object3D* SimpleGraphicsEngine::viewspace_ortho_camera_;

SimpleGraphicsEngine::SimpleGraphicsEngine(double time)
{
  time_ = time;
  dt_ = 0;
}

SimpleGraphicsEngine::~SimpleGraphicsEngine()
{
  delete scene_;
  delete view_space_;
  delete background_space_;  
  
  delete camera_;
} 

//! Initializes OpenGL, creating context and adding all basic objects for the scene.
bool SimpleGraphicsEngine::initialize()
{
  glewExperimental = true; // Needed in core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return false;
  }
  
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  // Cull triangles which normal is not towards the camera
  glEnable(GL_CULL_FACE);
  // Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  scene_ = new Object3D();
  
  view_space_ = new Object3D();
  background_space_ = new Object3D();

  camera_ = new Object3D();
  viewspace_ortho_camera_ = new Object3D();

  scene_->addChild(camera_);  
  
  view_space_->addChild(viewspace_ortho_camera_);
  
  return true;
}

//! This function needs to be implemented if extending this class.
void SimpleGraphicsEngine::update()
{
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float aspect = float(window_width_) / window_height_;
  glViewport(0,0,window_width_ * 2, window_height_ * 2);

  glDisable(GL_DEPTH_TEST);
  background_space_->render(glm::mat4());
  glEnable(GL_DEPTH_TEST);
  scene_->render(glm::mat4());
  view_space_->render(glm::mat4());
}

float SimpleGraphicsEngine::getDt()
{
  return dt_;
}


int SimpleGraphicsEngine::getWindowWidth()
{
  return window_width_;
}

int SimpleGraphicsEngine::getWindowHeight()
{
  return window_height_;
}

void SimpleGraphicsEngine::setWindowResolution(int width, int height)
{
  window_width_ = width;
  window_height_ = height;
}
