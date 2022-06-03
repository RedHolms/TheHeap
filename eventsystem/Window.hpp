#pragma once

#include <string>

#include <windows.h>

#include "Dispatcher.hpp"
#include "Layer.hpp"

namespace EventSystem {

   class Window {
      public:
         Window(std::string title, int width, int height);
         ~Window();
      
      private:
         HWND m_handle;
         bool m_active;
         Dispatcher* m_dispatcher;

      public:
         Layer* AddLayer(Layer* layer) { return m_dispatcher->AddLayer(layer); }
         bool RemoveLayer(Layer* layer) { return m_dispatcher->RemoveLayer(layer); }

         Dispatcher* GetDispatcher() { return m_dispatcher; }

         int Run();
      private:
         static LRESULT WINAPI _WindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
   };

}