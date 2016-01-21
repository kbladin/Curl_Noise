#include "../include/AntGui.h"

AntGui::AntGui()
{
	// Initialize AntTweakBar
  TwInit(TW_OPENGL_CORE, NULL);

  // Create a tweak bar
  bar = TwNewBar("TweakBar");
  TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' "); // Message added to the help bar.


  //// Add 'speed' to 'bar': it is a modifable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [s] and [S].
  //TwAddVarRW(bar, "speed", TW_TYPE_DOUBLE, &speed, 
  //           " label='Rot speed' min=0 max=2 step=0.01 keyIncr=s keyDecr=S help='Rotation speed (turns/second)' ");
}

AntGui::~AntGui()
{
  TwTerminate();
}

void AntGui::render(int width, int height)
{
  TwWindowSize(width * 2, height * 2);
  // Draw tweak bars
  TwDraw();
}