#ifndef ANT_GUI_H
#define ANT_GUI_H

#include <map>

#include <AntTweakBar.h>
#include "../include/ParticleSystem.h"
#include "../include/Material.h"

class AntGui
{
public:
	AntGui();
	~AntGui();

  void createParticleSystemPropertiesTwBar(
    ParticleSystemProperties* ps_properties,
    const char* name);
  void createPointCloundRenderingPropertiesTwBar(
    PointCloudRenderingProperties* pc_rendering_properties,
    const char* name);
  void deleteTwBar(const char* name);

  void render(int width, int height);
private:
  std::map<std::string, TwBar*> tweak_bars_;
};

#endif