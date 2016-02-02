#include "ApplicationWindowGLFW.h"

MyEngine* ApplicationWindowGLFW::engine_;
AntGui* ApplicationWindowGLFW::ant_gui_;

ApplicationWindowGLFW::ApplicationWindowGLFW()
{
  delay_counter_ = 0;
  frame_counter_ = 0;

  // First an OpenGL context needs to be created
  if (!initOpenGLContext())
  {
    std::cout << "ERROR : Failed to initialize OpenGL" << std::endl;
  }

  int width;
  int height;
  glfwGetWindowSize(window_, &width, &height);

  // Create engine
  engine_ = new MyEngine(width, height, glfwGetTime());

  // create the gui
  ant_gui_ = new AntGui(width, height);

  // Add tweak bars to control properties in the engine
  ant_gui_->createParticleSystemPropertiesTwBar(
    engine_->getParticleSystemPropertiesPointer(),
    engine_->getPointCloudRenderingPropertiesPointer(),
    "Particle System Properties");
  
  // Set callback functions
  glfwSetCursorPosCallback(window_, mousePosCallback);
  glfwSetMouseButtonCallback(window_, mouseButtonCallback);
  glfwSetScrollCallback(window_, mouseScrollCallback);
  glfwSetKeyCallback(window_, keyCallback);
  glfwSetWindowSizeCallback(window_, windowSizeCallback);
}

ApplicationWindowGLFW::~ApplicationWindowGLFW()
{
  delete ant_gui_;
  glfwTerminate();
}

bool ApplicationWindowGLFW::initOpenGLContext()
{
  // Initialize glfw
  if (!glfwInit())
    return false;
  // Modern OpenGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Create a windowed mode window and its OpenGL context
  window_ = glfwCreateWindow(720, 480, "Curl Noise", NULL, NULL);
  if (!window_)
  {
    glfwTerminate();
    return false;
  }
  // Make the window's context current
  glfwMakeContextCurrent(window_);
  printf("%s\n", glGetString(GL_VERSION));
  return true;
}

//! Starts the main loop
void ApplicationWindowGLFW::run()
{
  while (!glfwWindowShouldClose(window_))
  {
    int width;
    int height;

    engine_->update(glfwGetTime());

    // Draw the gui
    ant_gui_->render();

    // Print FPS on window
    frame_counter_ ++;
    delay_counter_ += engine_->getDt();  

    if (delay_counter_ >= 1.0) {
      std::stringstream title;
      title << "Curl Noise Demo by Kalle Bladin. " << frame_counter_ << " FPS";
      glfwSetWindowTitle(window_, title.str().c_str());
      frame_counter_ = 0;
      delay_counter_ = 0;
    }

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}

void ApplicationWindowGLFW::mousePosCallback(
  GLFWwindow * window,
  double x,
  double y)
{
  if (!TwEventMousePosGLFW(x * 2,y * 2))
  {
    engine_->mousePosCallback(x,y);
  }
}

void ApplicationWindowGLFW::mouseButtonCallback(
  GLFWwindow * window,
  int button,
  int action,
  int mods)
{
  if (!TwEventMouseButtonGLFW(button, action))
  {
    if (action == GLFW_PRESS)
      engine_->mouseButtonPress();
    else if (action == GLFW_RELEASE)
      engine_->mouseButtonRelease();
  }
}

void ApplicationWindowGLFW::mouseScrollCallback(
  GLFWwindow * window,
  double dx,
  double dy)
{
  engine_->mouseScrollCallback(dx, dy);
}

void ApplicationWindowGLFW::keyCallback(
  GLFWwindow * window,
  int key,
  int scancode,
  int action,
  int mods)
{
  //if (!TwEventKeyGLFW(key, action))
  //{
  //  if (action == GLFW_PRESS)
  //  {
  //    if (key == GLFW_KEY_ESCAPE)
  //    {
  //      glfwSetWindowShouldClose(window, GL_TRUE);
  //    }
  //  }
  //}
  engine_->keyCallback(key, scancode, action, mods);
}

void ApplicationWindowGLFW::windowSizeCallback(
  GLFWwindow* window,
  int width,
  int height)
{
  engine_->setWindowResolution(width, height);
  ant_gui_->setWindowResolution(width, height);
}
