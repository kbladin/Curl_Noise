#ifndef ANT_GUI_H
#define ANT_GUI_H

#include <AntTweakBar.h>

class AntGui
{
public:
	AntGui();
	~AntGui();

  void render(int width, int height);
private:
	TwBar *bar;         // Pointer to a tweak bar
};

#endif