#ifndef _windowMANAGER_H
#define _windowMANAGER_H

#include "MyEngine.h"

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
  // Functions
  bool initOpenGLContext();
  static void mousePosCallback(
    GLFWwindow * window,
    double x,
    double y);
  static void mouseButtonCallback(
    GLFWwindow * window,
    int button,
    int action,
    int mods);
  static void mouseScrollCallback(
    GLFWwindow * window,
    double dx,
    double dy);
  static void keyCallback(
    GLFWwindow * window,
    int key,
    int scancode,
    int action,
    int mods);
  static void windowSizeCallback(
    GLFWwindow* window,
    int width,
    int height);

  // Data
  GLFWwindow*       _window;
  static AntGui*    _ant_gui;
  static MyEngine*  _engine;

  float _delay_counter;
  int   _frame_counter;
};

#endif