#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "../include/MyEngine.h"

#include <iostream>

#include <AntTweakBar.h>

#include <gl/glew.h>
#include <gl/glfw3.h>


//! A class that handles both user interfacing and has an instance of a MyEngine object.
class ApplicationWindowGLFW
{
public:
  ApplicationWindowGLFW();
  ~ApplicationWindowGLFW();

  void run();

private:
  bool initOpenGLContext();
  static void mousePosCallback(GLFWwindow * window, double x, double y);
  static void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
  static void mouseScrollCallback(GLFWwindow * window, double dx, double dy);
  static void keyCallback(
    GLFWwindow * window,
    int key,
    int scancode,
    int action,
    int mods);
  static void windowSizeCallback(GLFWwindow* window, int width, int height);

  GLFWwindow* window_;
  static AntGui *ant_gui_;
  static MyEngine* engine_;

  float delay_counter_;
  int frame_counter_;
};

#endif