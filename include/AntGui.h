#ifndef _ant_guiH
#define _ant_guiH

#include "ParticleSystem.h"
#include "Material.h"

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
    ParticleSystemProperties*       ps_properties,
    PointCloudRenderingProperties*  pc_rendering_properties,
    const char*                     name);
  void deleteTwBar(const char* name);
  void render();

  // Getters
  int getWindowWidth();
  int getWindowHeight();

  // Setters
  void setWindowResolution(int width, int height);
private:
  // Data
  int _window_width;
  int _window_height;
  std::map<std::string, TwBar*> _tweak_bars;
};

#endif