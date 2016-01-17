#include "MyEngine.h"
#include <iostream>
#include <sstream>

MyEngine::MyEngine() : SimpleGraphicsEngine()
{
  //rotation_point = glm::vec3(0,0,-3);
  //camera_->transform_.translate(rotation_point);


  background_material_ = new BackgroundMaterial();
  grid_mesh_material_ = new OneColorMaterial();
  
  basic_cam_ = new PerspectiveCamera(ShaderManager::instance()->getShader("SHADER_PHONG"), window_);
  one_color_cam_ = new PerspectiveCamera(ShaderManager::instance()->getShader("SHADER_ONE_COLOR"), window_);
  background_ortho_cam_ = new OrthoCamera(ShaderManager::instance()->getShader("SHADER_BACKGROUND"), window_);
  point_cloud_cam_ = new PerspectiveCamera(ShaderManager::instance()->getShader("SHADER_RENDER_POINT_CLOUD"), window_);
  
  camera_->addChild(basic_cam_);
  camera_->addChild(one_color_cam_);
  camera_->addChild(point_cloud_cam_);
  viewspace_ortho_camera_->addChild(background_ortho_cam_);
  
  background_plane_ = new TriangleMesh(background_material_);
  background_plane_->initPlane(glm::vec3(0,0,0), glm::vec3(0,0,1), glm::vec3(10,2,2));
  
  point_cloud_ = new PointCloudGPU(1000);
  
  
  scene_->addChild(point_cloud_);
  background_space_->addChild(background_plane_);



  // Set callback functions
  glfwSetScrollCallback(window_, mouseScrollCallback);
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
  delete background_material_;

    delete background_plane_;

}

void MyEngine::update()
{
  SimpleGraphicsEngine::update();
  
  point_cloud_->update(dt_);


  frame_counter_ ++;
  delay_counter_ += dt_;
  
  if (delay_counter_ >= 1.0) {
    std::stringstream title;
    title << frame_counter_ << " FPS";
    glfwSetWindowTitle(window_, title.str().c_str());
    frame_counter_ = 0;
    delay_counter_ = 0;
  }
}

void MyEngine::mouseScrollCallback(GLFWwindow * window, double dx, double dy)
{

  /*
  glm::vec3 prev_position = camera_->transform_.getPosition();

  // Transform back
  camera_->transform_.translate(-prev_position);

  camera_->transform_.rotateX(- 5 * dy);
  
  float current_x_rotation = camera_->transform_.getEulerRotationXYZ().x;
  camera_->transform_.rotateX(-current_x_rotation);
  
  camera_->transform_.rotateY(- 5 * dx);

  camera_->transform_.rotateX(current_x_rotation);
  
  // Do transform again
  camera_->transform_.translate(prev_position);
  */
}