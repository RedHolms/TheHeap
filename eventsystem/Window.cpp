#include "Window.hpp"

#include <cassert>

#include <windows.h>

#include "Dispatcher.hpp"
#include "Events.hpp"

#if defined(_WIN64)
#define SWLP_TYPE LONG_PTR
#else
#define SWLP_TYPE LONG
#endif

namespace EventSystem {

   Window::Window(std::string title, int width, int height) {
      WNDCLASSA wndCls;
      ZeroMemory(&wndCls, sizeof(wndCls));
      wndCls.hInstance = GetModuleHandleA(NULL);
      wndCls.lpszClassName = "EventSystemWindowClass";
      wndCls.lpfnWndProc = _WindowProcedure;

      RegisterClassA(&wndCls);

      m_handle = CreateWindowA(
         "EventSystemWindowClass",
         title.c_str(),
         WS_OVERLAPPEDWINDOW,
         CW_USEDEFAULT, CW_USEDEFAULT,
         width, height,
         NULL, NULL,
         GetModuleHandleA(NULL),
         NULL
      );
      assert(m_handle);

      m_dispatcher = new Dispatcher;

      m_active = false;
   }

   Window::~Window() {
      delete m_dispatcher;
   }

   int Window::Run() {
      ShowWindow(m_handle, SW_SHOW);
      SetWindowLongPtrA(m_handle, GWLP_USERDATA, (SWLP_TYPE)this);

      m_active = true;

      MSG msg;
      while (m_active) {
         while (PeekMessageA(&msg, m_handle, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
            if (msg.message == WM_QUIT) {
               m_active = false;
               break;
            }
         }
         if (!m_active) break; // if window have destroyied we have no need to update/render anything

         // do something... (for example, updating and rendering)
         Sleep(16); // emulate 60 fps
      }
      return msg.wParam; // if we get here, last message is WM_QUIT and wParam is exit code
   }

   /* static */
   LRESULT WINAPI Window::_WindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
      Window* win = (Window*)GetWindowLongPtrA(handle, GWLP_USERDATA);
      if (win) {
         switch (message) {
         case WM_MOUSEMOVE:
            win->m_dispatcher->Dispatch(MouseMoveEvent(LOWORD(lParam), HIWORD(lParam), wParam)); return 0;
         case WM_KEYDOWN:
         case WM_SYSKEYDOWN:
            if (!(lParam & 0x40000000))
               win->m_dispatcher->Dispatch(VirtualKeyPressEvent(wParam));
            return 0;
         case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
         }
      }
      return DefWindowProcA(handle, message, wParam, lParam);
   }

}