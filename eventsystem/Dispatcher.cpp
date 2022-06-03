#include "Dispatcher.hpp"

#include "Layer.hpp"
#include "Event.hpp"
#include "Events.hpp"

#define _RaiseEvent(layers, eventType, eventObject) for (Layer* l : layers) l->on ## eventType((eventType&)eventObject)

namespace EventSystem {

   Dispatcher::Dispatcher() {}

   Layer* Dispatcher::AddLayer(Layer* layer) {
      m_layers.push_back(layer);
      return layer;
   }

   bool Dispatcher::RemoveLayer(Layer* layer) {
      for (auto i = m_layers.begin(); i != m_layers.end(); i++) {
         if (*i == layer) {
            m_layers.erase(i);
            return true;
         }
      }
      return false;
   }

   void Dispatcher::Dispatch(Event& e) {
      switch (e.GetType()) {
         case EventType::MOUSE_MOVE:
            _RaiseEvent(m_layers, MouseMoveEvent, e); break;
         case EventType::VKEY_PRESS:
            _RaiseEvent(m_layers, VirtualKeyPressEvent, e); break;
      }
   }

}