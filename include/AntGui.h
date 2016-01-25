#ifndef ANT_GUI_H
#define ANT_GUI_H

#include "../include/ParticleSystem.h"
#include "../include/Material.h"

#include <map>
#include <string>
#include <vector>

#include <AntTweakBar.h>

//! A class that handels user interfacing thorough the GUI library AntTweakBar
class AntGui
{
public:
  AntGui(int window_width, int window_height);
  ~AntGui();

  void createParticleSystemPropertiesTwBar(
    ParticleSystemProperties* ps_properties,
    PointCloudRenderingProperties* pc_rendering_properties,
    const char* name);
  void deleteTwBar(const char* name);
  void render();

  int getWindowWidth();
  int getWindowHeight();

  void setWindowResolution(int width, int height);
private:
  int window_width_;
  int window_height_;

  std::map<std::string, TwBar*> tweak_bars_;
};

#endif