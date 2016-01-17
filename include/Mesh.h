#ifndef MESH_H
#define MESH_H

#include "../include/Object3D.h"
#include "../include/Material.h"
#include "../include/MeshLoader.h"

#include <gl/glew.h>

#include <vector>
#include "../ext/glm/include/glm/glm.hpp"
#include "../ext/glm/include/glm/gtc/matrix_transform.hpp"
#include "../ext/glm/include/glm/gtx/transform.hpp"

//! This class serves as a base for the mesh classes.
class AbstractMesh : public Object3D{
public:
  AbstractMesh(Material* material);
  ~AbstractMesh();
  virtual void render(glm::mat4 M) = 0;
  const Material* material_;
protected:
  virtual void initialize() = 0;
  
  std::vector<glm::vec3> vertices_;

  GLuint vertex_array_ID_;
  GLuint vertex_buffer_;
  GLuint model_matrix_ID_;
};

//! This class extends AbstractMesh and renders triangles
  /*!
   The TriangleMesh also has a list of normals and elements.
  */
class TriangleMesh : public AbstractMesh{
public:
  TriangleMesh(const char *file_name, Material* material);
  TriangleMesh(std::vector<glm::vec3> vertices,
               std::vector<glm::vec3> normals,
               std::vector<unsigned short> elements,
               Material* material);
  TriangleMesh(Material* material);
  ~TriangleMesh();
  void initPlane(glm::vec3 position, glm::vec3 normal, glm::vec3 scale);
  void initBox(glm::vec3 max, glm::vec3 min, glm::vec3 position);
  void initCone(glm::vec3 position,
                glm::vec3 direction,
                glm::vec3 scale,
                int divisions);
  void initCylinder(glm::vec3 position,
                    glm::vec3 direction,
                    glm::vec3 scale,
                    int divisions);
  virtual void render(glm::mat4 M);
private:
  void initialize();
  // Maximum around 60000 vertices for unsigned short.
  std::vector<unsigned short> elements_;
  GLuint element_buffer_;
  GLuint normal_buffer_;
  std::vector<glm::vec3> normals_;
};

//! This class extends AbstractMesh and renders lines
/*!
  Currently, this mesh can not be loaded from file.
*/
class LineMesh : public AbstractMesh {
public:
  LineMesh(Material* material);
  ~LineMesh();
  void initLine(glm::vec3 start, glm::vec3 end);
  void initGridPlane(
                     glm::vec3 position,
                     glm::vec3 normal,
                     glm::vec3 scale,
                     unsigned int divisions);
  void initCircle(
                  glm::vec3 position,
                  glm::vec3 normal,
                  glm::vec3 scale,
                  unsigned int divisions);
  virtual void render(glm::mat4 M);
private:
  void initialize();
  // Maximum around 60000 vertices for unsigned short.
  std::vector<unsigned short> elements_;
  GLuint element_buffer_;
};

class PointCloudGPU;
//! This class extends AbstractMesh
class PointCloudMesh : public AbstractMesh {
public:
  PointCloudMesh(Material* material, int size);
  ~PointCloudMesh();
  virtual void render(glm::mat4 M);
private:
  const int size_;
  friend class PointCloudGPU;
  void initialize();
  GLuint index_buffer_;
};

#endif