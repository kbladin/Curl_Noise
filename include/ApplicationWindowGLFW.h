#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <iostream>

#include <gl/glew.h>
#include <gl/glfw3.h>

#include <AntTweakBar.h>
#include "../include/MyEngine.h"

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

	AntGui *ant_gui_;
	GLFWwindow* window_;
  static MyEngine* engine_;

  float delay_counter_;
  int frame_counter_;
};

#endif