#pragma once

#include <vector>

#include "Layer.hpp"
#include "Event.hpp"

namespace EventSystem {

   class Dispatcher {
   public:
      Dispatcher();

   private:
      std::vector<Layer*> m_layers;

   public:
      Layer* AddLayer(Layer* layer);
      bool RemoveLayer(Layer* layer);

      void Dispatch(Event& e);
   };

};