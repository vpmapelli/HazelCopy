#pragma once

#include "Hazel/Core.h"

namespace Hazel {
    class Layer; //forward declare Layer (only pointers)

    class HAZEL_API LayerStack 
    {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end()   { return m_Layers.end(); }
    private:
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };
}
