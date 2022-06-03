#pragma once

#include <cstdint>

#include <windows.h>

#include "Event.hpp"

namespace EventSystem {

   class MouseMoveEvent : public Event {
   public:
      MouseMoveEvent(short px, short py, unsigned flags)
         : m_px(px), m_py(py), m_flags(flags), Event(EventType::MOUSE_MOVE) {}

   private:
      short m_px, m_py;
      unsigned m_flags;

   public:
      short GetMousePosX() { return m_px; }
      short GetMousePosY() { return m_py; }
      unsigned GetFlags() { return m_flags; } 

      bool IsCtrlPressed()          { return m_flags & MK_CONTROL; }
      bool IsLeftMousePressed()     { return m_flags & MK_LBUTTON; }
      bool IsMiddleMousePressed()   { return m_flags & MK_MBUTTON; }
      bool IsRigthMousePressed()    { return m_flags & MK_RBUTTON; }
      bool IsShiftPressed()         { return m_flags & MK_SHIFT; }
      bool IsXButton1Pressed()      { return m_flags & MK_XBUTTON1; }
      bool IsXButton2Pressed()      { return m_flags & MK_XBUTTON2; }
   };


   class VirtualKeyPressEvent : public Event {
   public:
      VirtualKeyPressEvent(uint8_t vkeyCode)
         : m_vkeyCode(vkeyCode), Event(EventType::VKEY_PRESS) {}

   private:
      uint8_t m_vkeyCode;

   public:
      uint8_t GetVkeyCode() { return m_vkeyCode; }
   };

}