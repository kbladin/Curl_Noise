#include "../include/AntGui.h"

//! Initializes AntTweakBar so that gui bars can be added
AntGui::AntGui(int window_width, int window_height)
{
  // Rescale before initializing
  TwDefine(" GLOBAL fontscaling=2 "); // double the size of all fonts
  // Initialize AntTweakBar
  TwInit(TW_OPENGL_CORE, NULL);
  // Set properties
  TwDefine(" GLOBAL contained=true "); // bars cannot move outside of the window
  TwDefine(" GLOBAL help='...' ");
  TwDefine(" GLOBAL fontsize=2 "); // use large font
  TwDefine(" GLOBAL fontresizable=false "); // font cannot be resized

  window_width_ = window_width;
  window_height_ = window_height;
}

//! Destructor
AntGui::~AntGui()
{
  // Delete all tweak bars
  TwDeleteAllBars();
  TwTerminate();
}

//! Creates a tweak bar to change particle system properties
/*!
  \param ps_properties is a pointer to a data structure of type
  ParticleSystemProperties. Its values can be changed from the gui.
  \param pc_rendering_properties is a pointer to a data structure of type
  PointCloudRenderingProperties. Its values can be changed from the gui.
  \param name is the name of the tweak bar.
*/
void AntGui::createParticleSystemPropertiesTwBar(
  ParticleSystemProperties* ps_properties,
  PointCloudRenderingProperties* pc_rendering_properties,
  const char* name)
{
  // Create a tweak bar
  TwBar* bar = TwNewBar(name);
  std::string definitions;
  definitions =
  std::string("'") + name + std::string("' ") + 
  std::string("help='These properties defines the engine behavior' ") +
  std::string("size='470 770' ") +
  std::string("movable=false ") +
  std::string("resizable=false ");
  TwDefine(definitions.c_str());

  // Add a text (the way to do it with Ant Tweak Bar)
  TwAddButton(bar, "comment1", NULL, NULL, " label='Behavior Properties' ");

  // Add the variables
  TwAddVarRW(
    bar,
    "field_speed",
    TW_TYPE_FLOAT,
    &ps_properties->field_speed, 
    " label='Field Speed'"
    " min=0 max=1 step=0.01"
    " keyIncr=s keyDecr=S"
    " help='Speed or flow of the vector field' ");
  TwAddVarRW(
    bar,
    "curl",
    TW_TYPE_FLOAT,
    &ps_properties->curl, 
    " label='Curl'"
    " min=0 max=1 step=0.01"
    " keyIncr=d keyDecr=D"
    " help='The amount of noise used in the vector potential' ");
  TwAddVarRW(
    bar,
    "progression_rate",
    TW_TYPE_FLOAT,
    &ps_properties->progression_rate, 
    " label='Progression Rate'"
    " min=0 max=1 step=0.01"
    " keyIncr=s keyDecr=S"
    " help='Speed of time varying noise' ");
  TwAddVarRW(
    bar,
    "length_scale",
    TW_TYPE_FLOAT,
    &ps_properties->length_scale, 
    " label='Length Scale'"
    " min=0 max=1 step=0.01"
    " keyIncr=s keyDecr=S"
    " help='Overall size of the motions throug the fluid' ");
  TwAddVarRW(
    bar,
    "life_length_factor",
    TW_TYPE_FLOAT,
    &ps_properties->life_length_factor, 
    " label='Life Length Factor'"
    " min=0.05 max=1 step=0.01"
    " keyIncr=s keyDecr=S "
    "help='Factor of avarage life length for the particles' ");
  TwAddVarRW(
    bar,
    "emitter_size",
    TW_TYPE_FLOAT,
    &ps_properties->emitter_size, 
    " label='Emitter size'"
    " min=0.05 max=1 step=0.01"
    " keyIncr=s keyDecr=S"
    " help='Factor of emission area' ");
  TwAddVarRW(
    bar,
    "field_main_direction",
    TW_TYPE_DIR3F,
    &ps_properties->field_main_direction, 
    " label='Field Main Direction'"
    " help='Direction of field' ");

  TwAddSeparator(bar, NULL, NULL);
  // Add a text (the way to do it with Ant Tweak Bar)
  TwAddButton(bar, "comment2", NULL, NULL, " label='Rendering Properties' ");

  // Create enum values
  TwEnumVal shader_enum_val[] =
    {
      {ADDITIVE, "Additive"},
      {PHONG, "Phong"}
    };
  TwType shader_type;
  // Define enum type
  shader_type = TwDefineEnum("shader_type", shader_enum_val, 2);

  // Add the variables
  TwAddVarRW(
    bar,
    "Shader",
    shader_type,
    &pc_rendering_properties->shader,
    NULL);
  TwAddVarRW(
    bar,
    "particle_color",
    TW_TYPE_COLOR3F,
    &pc_rendering_properties->particle_color,
    " label='Particle Color' ");
  TwAddVarRW(
    bar,
    "particle_radius",
    TW_TYPE_FLOAT,
    &pc_rendering_properties->particle_radius, 
    " label='Particle Radius'"
    " min=1 max=20 step=0.1"
    " keyIncr=s keyDecr=S"
    " help='Radius of each particle' ");
  
  // Push the tweak bar to the map
  tweak_bars_[name] = bar;
}

//! Deletes the tweak bar defined by name
/*!
  \param name is the name of the tweak bar.
*/
void AntGui::deleteTwBar(const char* name)
{
  TwBar* bar = tweak_bars_[name];
  if (bar)
  {
    TwDeleteBar(bar);
    tweak_bars_.erase (name);
  }
}

//! Returns the window width in pixels
int AntGui::getWindowWidth()
{
  return window_width_;
}

//! Returns the window height in pixels
int AntGui::getWindowHeight()
{
  return window_height_;
}

//! Set resolution to new window width and height
/*!
  \param width is the new window width.
  \param height is the new window height.
*/
void AntGui::setWindowResolution(int width, int height)
{
  window_height_ = height;
  window_width_ = width;
}

//! Renders the gui
void AntGui::render()
{
  TwWindowSize(window_width_ * 2, window_height_ * 2);
  // Draw tweak bars
  TwDraw();
}