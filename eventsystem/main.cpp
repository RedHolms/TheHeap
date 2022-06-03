#include <cstdio>

#include <windows.h>

#include "Window.hpp"
#include "Layer.hpp"

class MainLayer : public EventSystem::Layer {
public:

   void onMouseMoveEvent(EventSystem::MouseMoveEvent& e) {
      printf("Mouse moved to %i, %i. %s\n", e.GetMousePosX(), e.GetMousePosY(), e.IsLeftMousePressed() ? "Dragged" : "Not Dragged");
   }

   void onVirtualKeyPressEvent(EventSystem::VirtualKeyPressEvent& e) {
      printf("Virtual Key pressed: %u\n", e.GetVkeyCode());
   }

};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
   AllocConsole();
   freopen("CON", "w", stdout);

   EventSystem::Window* win = new EventSystem::Window("Event System", 800, 600);

   MainLayer* layer = new MainLayer;
   win->AddLayer(layer);

   int status = win->Run();

   delete win;

   return status;
}