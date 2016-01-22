#include "../include/AntGui.h"

AntGui::AntGui()
{
	// Initialize AntTweakBar
  TwInit(TW_OPENGL_CORE, NULL);

  //// Create a tweak bar
  //bar = TwNewBar("TweakBar");
  //TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' "); // Message added to the help bar.


  //// Add 'speed' to 'bar': it is a modifable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [s] and [S].
  //TwAddVarRW(bar, "speed", TW_TYPE_DOUBLE, &speed, 
  //           " label='Rot speed' min=0 max=2 step=0.01 keyIncr=s keyDecr=S help='Rotation speed (turns/second)' ");
}

AntGui::~AntGui()
{
  // Delete all tweak bars
  for (
    std::map<std::string, TwBar*>::iterator it = tweak_bars_.begin();
    it != tweak_bars_.end();
    ++it)
  {
    TwDeleteBar(it->second);
  }
  TwTerminate();
}

void AntGui::createParticleSystemPropertiesTwBar(
  ParticleSystemProperties* ps_properties,
  const char* name)
{
  // Create a tweak bar
  TwBar* bar = TwNewBar(name);
  // Message added to the help bar.
  TwDefine(" GLOBAL help='Properties of the particle system' ");

  // Add the variables
  TwAddVarRW(
    bar,
    "field_speed",
    TW_TYPE_FLOAT,
    &ps_properties->field_speed, 
    " label='Field Speed' min=0 max=1 step=0.01 keyIncr=s keyDecr=S help='Speed or flow of the vector field' ");
  TwAddVarRW(
    bar,
    "curl",
    TW_TYPE_FLOAT,
    &ps_properties->curl, 
    " label='Curl' min=0 max=1 step=0.01 keyIncr=d keyDecr=D help='The amount of noise used in the vector potential' ");
  TwAddVarRW(
    bar,
    "progression_rate",
    TW_TYPE_FLOAT,
    &ps_properties->progression_rate, 
    " label='Progression Rate' min=0 max=1 step=0.01 keyIncr=s keyDecr=S help='Speed of time varying noise' ");
  TwAddVarRW(
    bar,
    "length_scale",
    TW_TYPE_FLOAT,
    &ps_properties->length_scale, 
    " label='Length Scale' min=0 max=1 step=0.01 keyIncr=s keyDecr=S help='Overall size of the motions throug the fluid' ");
  TwAddVarRW(
    bar,
    "life_length_factor",
    TW_TYPE_FLOAT,
    &ps_properties->life_length_factor, 
    " label='Life Length Factor' min=0 max=1 step=0.01 keyIncr=s keyDecr=S help='Factor of avarage life length for the particles' ");
  TwAddVarRW(
    bar,
    "emission_area_factor",
    TW_TYPE_FLOAT,
    &ps_properties->emission_area_factor, 
    " label='Emission Area Factor' min=0 max=1 step=0.01 keyIncr=s keyDecr=S help='Factor of emission area' ");
  //TwAddVarRW(
  //  bar,
  //  "particle_color",
  //  TW_TYPE_COLOR3F,
  //  &ps_properties->color,
  //  " label='Particle Color' ");

  // Push the tweak bar to the map
  tweak_bars_[name] = bar;
}

void AntGui::createPointCloundRenderingPropertiesTwBar(
  PointCloudRenderingProperties* pc_rendering_properties,
  const char* name)
{
  // Create a tweak bar
  TwBar* bar = TwNewBar(name);
  // Message added to the help bar.
  TwDefine(" GLOBAL help='Rendering properties of the particle system' ");

  // Add the variables
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
    " label='Particle Radius' min=1 max=20 step=0.1 keyIncr=s keyDecr=S help='Radius of each particle' ");
  
  // Push the tweak bar to the map
  tweak_bars_[name] = bar;
}

void AntGui::deleteTwBar(const char* name)
{
  TwBar* bar = tweak_bars_[name];
  if (bar)
  {
    TwDeleteBar(bar);
    tweak_bars_.erase (name);
  }
}

void AntGui::render(int width, int height)
{
  TwWindowSize(width * 2, height * 2);
  // Draw tweak bars
  TwDraw();
}