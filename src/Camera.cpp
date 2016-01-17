#include "../include/Camera.h"

//! Creates camera to render objects with the defined shader program attached.
  /*!
    The shader program can later be changed so that other shaders can be
    used for this particular camera. It is also possible to have several
    cameras and switching between them to use separate shaders.
    \param program_ID is the shader program that this Camera will render.
    \param window is the GLFWwindow* to render to.
  */
AbstractCamera::AbstractCamera(GLuint program_ID, GLFWwindow* window)
{
  window_ = window;
  program_ID_ = program_ID;
  glUseProgram(program_ID_);

  view_matrix_ID_ = glGetUniformLocation(program_ID_, "V");
  projection_matrix_ID_ = glGetUniformLocation(program_ID_, "P");
}
//! Render the Camera.
  /*!
    Sets the view and projection matrices of the cameras shader to the
    corresponding transforms for the camera.
    \param M is the transformation matrix of the parent.
  */
void AbstractCamera::render(glm::mat4 M)
{
  glm::mat4 V = M * transform_matrix_;
  Object3D::render(V);
  
  glUseProgram(program_ID_);

  glUniformMatrix4fv(view_matrix_ID_, 1, GL_FALSE, &V[0][0]);
  glUniformMatrix4fv(projection_matrix_ID_, 1, GL_FALSE, &projection_transform_[0][0]);
}

//! Set the camera to be attached to the shader provided
  /*!
    The camera will set the needed view and projection matrices when rendering.
    \param program_ID is the shader program containing the V and P matrices
    to be set by the camera
  */
void AbstractCamera::setShader(GLuint program_ID)
{
  program_ID_ = program_ID;
  glUseProgram(program_ID_);

  view_matrix_ID_ = glGetUniformLocation(program_ID_, "V");
  projection_matrix_ID_ = glGetUniformLocation(program_ID_, "P");
}

//! Creates camera to render objects with the defined shader program attached.
  /*!
    The shader program can later be changed so that other shaders can be
    used for this particular camera. It is also possible to have several
    cameras and switching between them to use separate shaders.
    The aspect ratio is set from the parameter \param window.
    \param program_ID is the shader program that this Camera will render.
    \param window is the GLFWwindow* to render to.
  */
PerspectiveCamera::PerspectiveCamera(GLuint program_ID, GLFWwindow* window) :
AbstractCamera(program_ID, window)
{
  int width;
  int height;
  glfwGetWindowSize(window_, &width, &height);
  float aspect = float(width)/height;
  projection_transform_ = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
}

//! Render the Camera.
  /*!
    Sets the view and projection matrices of the cameras shader to the
    corresponding transforms for the camera.
    \param M is the transformation matrix of the parent.
  */
void PerspectiveCamera::render(glm::mat4 M)
{
  int width;
  int height;
  glfwGetWindowSize(window_, &width, &height);
  float aspect = float(width)/height;
  projection_transform_ = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
  
  AbstractCamera::render(M);
}

//! Creates camera to render objects with the defined shader program attached.
  /*!
    The shader program can later be changed so that other shaders can be
    used for this particular camera. It is also possible to have several
    cameras and switching between them to use separate shaders.
    The aspect ratio is set from the parameter \param window.
    \param program_ID is the shader program that this Camera will render.
    \param window is the GLFWwindow* to render to.
  */
OrthoCamera::OrthoCamera(GLuint program_ID, GLFWwindow* window) :
  AbstractCamera(program_ID, window)
{
  int width;
  int height;
  glfwGetWindowSize(window_, &width, &height);
  float aspect = float(width)/height;
  projection_transform_ = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -100.0f, 100.0f);
}

//! Render the Camera.
  /*!
    Sets the view and projection matrices of the cameras shader to the
    corresponding transforms for the camera.
    \param M is the transformation matrix of the parent.
  */
void OrthoCamera::render(glm::mat4 M)
{
  int width;
  int height;
  glfwGetWindowSize(window_, &width, &height);
  float aspect = float(width)/height;
  projection_transform_ = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -100.0f, 100.0f);
  
  AbstractCamera::render(M);
}
