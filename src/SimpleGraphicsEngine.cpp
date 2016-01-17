#include "../include/SimpleGraphicsEngine.h"

//! Creates a LightSource bound to a specific shader.
/*!
  \param program_ID is the shader program that this LightSource will be bound
  to.
*/
LightSource::LightSource(GLuint program_ID)
{
  intensity_ = 5.0f;
  color_ = glm::vec3(1.0, 1.0, 1.0);
  
  program_ID_ = program_ID;
  
  glUseProgram(program_ID_);
  light_position_ID_ = glGetUniformLocation(program_ID_, "lightPosition");
  light_intensity_ID_ = glGetUniformLocation(program_ID_, "lightIntensity");
  light_color_ID_ = glGetUniformLocation(program_ID_, "lightColor");
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

//! Initializes OpenGL, creating context and adding all basic objects for the scene.
SimpleGraphicsEngine::SimpleGraphicsEngine()
{
  if( !initialize() )
    std::cout << "ERROR : Could not initialize SimpleGraphicsEngine!" << std::endl;
}

SimpleGraphicsEngine::~SimpleGraphicsEngine()
{
  glfwTerminate();
  delete scene_;
  delete view_space_;
  delete background_space_;  
  
  delete camera_;
}

bool SimpleGraphicsEngine::initialize()
{
  time_ = glfwGetTime();
  // Initialize the library
  if (!glfwInit())
    return false;
  // Modern OpenGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Create a windowed mode window and its OpenGL context
  window_ = glfwCreateWindow(720, 480, "Model Viewer", NULL, NULL);
  if (!window_)
  {
    glfwTerminate();
    return false;
  }
  // Make the window's context current
  glfwMakeContextCurrent(window_);
  printf("%s\n", glGetString(GL_VERSION));
  
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

//! Starts the main loop
void SimpleGraphicsEngine::run()
{
  while (!glfwWindowShouldClose(window_))
  {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    update();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.7, 0.7, 0.7, 1);
    
    int width;
    int height;
    glfwGetWindowSize(window_, &width, &height);
    float aspect = float(width)/height;
    glViewport(0,0,width * 2,height * 2);

    glDisable(GL_DEPTH_TEST);
    background_space_->render(glm::mat4());
    glEnable(GL_DEPTH_TEST);
    scene_->render(glm::mat4());
    view_space_->render(glm::mat4());

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}

//! This function needs to be implemented if extending this class.
void SimpleGraphicsEngine::update()
{
  dt_ = glfwGetTime() - time_;
  time_ = glfwGetTime();
}