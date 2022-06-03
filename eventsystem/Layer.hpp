#pragma once

#include "Events.hpp"

namespace EventSystem {

   /* abstract */
   class Layer {
   public:

      virtual void onMouseMoveEvent(MouseMoveEvent&) {}
      virtual void onVirtualKeyPressEvent(VirtualKeyPressEvent&) {}
      // we can add more events

   };

}