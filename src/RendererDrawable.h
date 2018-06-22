#ifndef RENDERERDRAWABLE_H_INCLUDED
#define RENDERERDRAWABLE_H_INCLUDED

#include "RendererBase.h"

template <class T>
class RendererDrawable : public RendererBase<T> {
    public:
        RendererDrawable(sf::RenderWindow& _window);
        ~RendererDrawable();

        void addObject(const T* _obj) const;

        void getInput(sf::Event& _event);
        void update();
        void draw() const;

    private:
        void onResize(sf::Vector2f _newSize);
    private:
        mutable std::vector<const T*> m_objects;
};

#endif // RENDERERDRAWABLE_H_INCLUDED
