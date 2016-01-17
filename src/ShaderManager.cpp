#include "../include/ShaderManager.h"

ShaderManager* ShaderManager::instance_;

ShaderManager::ShaderManager()
{
  shader_program_IDs.insert(
  	std::pair<std::string,GLuint>(
  		"SHADER_PHONG",
  		ShaderLoader::loadShaders(
  			"../data/shaders/simple.vert",
  			NULL,
  			NULL,
  			NULL,
  			"../data/shaders/simple.frag" )));
  shader_program_IDs.insert(
  	std::pair<std::string,GLuint>(
  		"SHADER_ONE_COLOR",
  		ShaderLoader::loadShaders(
  			"../data/shaders/one_color.vert",
  			NULL,
  			NULL,
  			NULL,
  			"../data/shaders/one_color.frag" )));
  shader_program_IDs.insert(
  	std::pair<std::string,GLuint>(
  		"SHADER_BACKGROUND",
  		ShaderLoader::loadShaders(
  			"../data/shaders/background.vert",
  			NULL,
  			NULL,
  			NULL,
  			"../data/shaders/background.frag" )));
  shader_program_IDs.insert(
  	std::pair<std::string,GLuint>(
  		"SHADER_UPDATE_POINT_CLOUD_POSITIONS",
  		ShaderLoader::loadShaders(
  			"../data/shaders/point_cloud_programs/quad_passthrough.vert",
  			NULL,
  			NULL,
  			NULL,
  			"../data/shaders/point_cloud_programs/update_positions.frag")));
  shader_program_IDs.insert(
  	std::pair<std::string,GLuint>(
  		"SHADER_UPDATE_POINT_CLOUD_VELOCITIES",
  		ShaderLoader::loadShaders(
  			"../data/shaders/point_cloud_programs/quad_passthrough.vert",
  			NULL,
  			NULL,
  			NULL,
  			"../data/shaders/point_cloud_programs/update_velocities.frag")));
  shader_program_IDs.insert(
  	std::pair<std::string,GLuint>(
  		"SHADER_UPDATE_POINT_CLOUD_ACCELERATIONS",
  		ShaderLoader::loadShaders(
  			"../data/shaders/point_cloud_programs/quad_passthrough.vert",
  			NULL,
  			NULL,
  			NULL,
  			"../data/shaders/point_cloud_programs/update_accelerations.frag")));
  shader_program_IDs.insert(
  	std::pair<std::string,GLuint>(
  		"SHADER_RENDER_POINT_CLOUD",
  		ShaderLoader::loadShaders(
  			"../data/shaders/point_cloud_programs/render.vert",
  			NULL,
  			NULL,
  			NULL,
  			"../data/shaders/point_cloud_programs/render.frag")));
}

//! Destructor
ShaderManager::~ShaderManager()
{
  for (std::map<std::string, GLuint>::const_iterator it =
  		shader_program_IDs.begin();
  	it != shader_program_IDs.end();
  	it++) {
    glDeleteProgram(it->second);
  }
}

//! Returning the instance that can be used for calling other functions.
/*!
  This function can not be called before an OpenGL context is created.
*/
ShaderManager* ShaderManager::instance()
{
  if (!instance_) {
    instance_ = new ShaderManager();
  }
  return instance_;
}

//! Returns a program ID based on a name.
/*!
  \param name must be one of the compiled shaders names. The options are:
  "SHADER_PHONG", "SHADER_ONE_COLOR" or "SHADER_BACKGROUND"
  (more might be added later).
*/
GLuint ShaderManager::getShader(std::string name)
{
  GLuint program_ID = shader_program_IDs[name];
  if (!program_ID)
  {
    std::cout <<
    "ERROR : This name, " <<
    name <<
    " is not a valid shader program name!" <<
    std::endl;
  }
  return program_ID;
}
