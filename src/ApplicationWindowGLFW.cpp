#include "ApplicationWindowGLFW.h"

MyEngine* ApplicationWindowGLFW::_engine;
AntGui* ApplicationWindowGLFW::_ant_gui;

ApplicationWindowGLFW::ApplicationWindowGLFW()
{
  _delay_counter = 0;
  _frame_counter = 0;

  // First an OpenGL context needs to be created
  if (!initOpenGLContext())
  {
    std::cout << "ERROR : Failed to initialize OpenGL" << std::endl;
  }

  int width;
  int height;
  glfwGetWindowSize(_window, &width, &height);

  // Create engine
  _engine = new MyEngine(width, height, glfwGetTime());

  // create the gui
  _ant_gui = new AntGui(width, height);

  // Add tweak bars to control properties in the engine
  _ant_gui->createParticleSystemPropertiesTwBar(
    _engine->getParticleSystemPropertiesPointer(),
    _engine->getPointCloudRenderingPropertiesPointer(),
    "Particle System Properties");
  
  // Set callback functions
  glfwSetCursorPosCallback(_window, mousePosCallback);
  glfwSetMouseButtonCallback(_window, mouseButtonCallback);
  glfwSetScrollCallback(_window, mouseScrollCallback);
  glfwSetKeyCallback(_window, keyCallback);
  glfwSetWindowSizeCallback(_window, windowSizeCallback);
}

ApplicationWindowGLFW::~ApplicationWindowGLFW()
{
  delete _ant_gui;
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
  _window = glfwCreateWindow(720, 480, "Curl Noise", NULL, NULL);
  if (!_window)
  {
    glfwTerminate();
    return false;
  }
  // Make the window's context current
  glfwMakeContextCurrent(_window);
  printf("%s\n", glGetString(GL_VERSION));
  return true;
}

//! Starts the main loop
void ApplicationWindowGLFW::run()
{
  while (!glfwWindowShouldClose(_window))
  {
    int width;
    int height;

    _engine->update(glfwGetTime());

    // Draw the gui
    _ant_gui->render();

    // Print FPS on window
    _frame_counter ++;
    _delay_counter += _engine->getDt();  

    if (_delay_counter >= 1.0) {
      std::stringstream title;
      title << "Curl Noise Demo by Kalle Bladin. " << _frame_counter << " FPS";
      glfwSetWindowTitle(_window, title.str().c_str());
      _frame_counter = 0;
      _delay_counter = 0;
    }

    glfwSwapBuffers(_window);
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
    _engine->mousePosCallback(x,y);
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
      _engine->mouseButtonPress();
    else if (action == GLFW_RELEASE)
      _engine->mouseButtonRelease();
  }
}

void ApplicationWindowGLFW::mouseScrollCallback(
  GLFWwindow * window,
  double dx,
  double dy)
{
  _engine->mouseScrollCallback(dx, dy);
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
  _engine->keyCallback(key, scancode, action, mods);
}

void ApplicationWindowGLFW::windowSizeCallback(
  GLFWwindow* window,
  int width,
  int height)
{
  _engine->setWindowResolution(width, height);
  _ant_gui->setWindowResolution(width, height);
}
