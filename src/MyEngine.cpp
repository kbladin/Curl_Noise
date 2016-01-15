#include "MyEngine.h"
#include "ModelLoader.h"
#include <iostream>
#include <sstream>

MyEngine::MyEngine() : SimpleGraphicsEngine()
{
  rotation_point = glm::vec3(0,0,-3);
  /*
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<unsigned short> elements;
  
  bool loaded;
  do{
    // Load file
    std::cout << "Load file: ";
    std::string file_name = "../../data/testmodels/bunny.m";
    //std::cin >> file_name;
    loaded = ModelLoader::load(file_name.c_str(), &vertices, &normals, &elements);
  } while (!loaded);
  
  // "../../data/testmodels/bunny.m"
  // "../../data/testmodels/gargoyle.m"
  
  // Initialize all objects
  bunny_material_ = new PhongMaterial();
  bunny_mesh_ = new TriangleMesh(vertices, normals, elements, bunny_material_);
  
  light_ = new LightSource(ShaderManager::instance()->getShader("SHADER_PHONG"));
  light_mesh_ = new LightMesh3D(1);
  bunny_ = new Object3D();
  bb_ = new BoundingBox(bunny_mesh_);
  
  // Change properties
  light_->transform_.translate(glm::vec3(4, 4, 4));
  light_->intensity_ = 20;
*/
  camera_->transform_.translate(rotation_point);
  
  // Hierarchies
  //bunny_mesh_->normalizeScale();
  //bunny_->addChild(bunny_mesh_);
  //bunny_mesh_->addChild(bb_);
  
  //light_->addChild(light_mesh_);

  //Add objects to scene
  //scene_->addChild(bunny_);
  //scene_->addChild(light_);
  
  // Set callback functions
  glfwSetScrollCallback(window_, mouseScrollCallback);
}

MyEngine::~MyEngine()
{
  delete bunny_material_;
  delete bunny_;
  delete bunny_mesh_;
  delete light_;
  delete bb_;
  delete light_mesh_;
}

void MyEngine::update()
{
  SimpleGraphicsEngine::update();
  
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
}