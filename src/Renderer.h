#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include <map>

#include "SFML/Graphics.hpp"

template <class T>
class Renderer {
    public:
        Renderer(sf::RenderWindow& _window);
        void addObject(const T* _obj) const;

        void getInput(sf::Event& _event);
        void update();
        void draw(bool _keepView = false) const;

    private:
        void onResize(sf::Vector2f _newSize);
    private:
        sf::RenderWindow& m_window;
        mutable std::vector<const T*> m_objects;
        sf::View m_view;

};

#endif // RENDERER_H_INCLUDED
