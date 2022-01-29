#include "Engine3D.cpp"
int main()
{
   auto* engine = new Engine3D(screenResolution::RES_1920x1080,"3D_Showcase",lockedFPS::FPS_60,displayForm::FULLSCREEN,true, true, true,Colours::RED);
   engine->run();
}
