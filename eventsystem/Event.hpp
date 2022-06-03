#pragma once

#include <cstdint>

namespace EventSystem {

   enum class EventType : uint32_t {
      UNKNOWN = 0,

      MOUSE_MOVE,
      VKEY_PRESS,

      _MAX
   };

   class Event {
   protected:
      Event(EventType type)
         : m_type(type) {}

   private:
      EventType m_type = EventType::UNKNOWN;
      
   public:
      EventType GetType() { return m_type; }
   };

}