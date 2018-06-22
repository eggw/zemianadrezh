#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include <map>

#include "SFML/Graphics.hpp"

template <class T>
class RendererBase {
    public:
        RendererBase(sf::RenderWindow& _window);
        ~RendererBase()                                 = 0;

        virtual void addObject(const T* _obj) const     = 0;

        virtual void getInput(sf::Event& _event)        = 0;
        virtual void update()                           = 0;
        virtual void draw() const = 0;

    protected:
        virtual void onResize(sf::Vector2f _newSize)    = 0;
    protected:
        sf::RenderWindow& m_window;
};

#endif // RENDERER_H_INCLUDED
