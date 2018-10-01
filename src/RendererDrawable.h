#ifndef RENDERERDRAWABLE_H_INCLUDED
#define RENDERERDRAWABLE_H_INCLUDED

#include "RendererBase.h"

template <class T>
class RendererDrawable : public RendererBase<T> {
    public:
        RendererDrawable(sf::RenderWindow& _window)
            :RendererBase<T>(_window) {
        };

        ~RendererDrawable() {
        };

        void addObject(const T* _obj) const {
            auto it = std::find(m_objects.begin(), m_objects.end(), _obj);

            if(it == m_objects.end()) {
                m_objects.push_back(_obj);
            }
        };

        void draw() const {
            for(auto& object : m_objects) {
                RendererBase<T>::m_window.draw(*object);
            }
        };

    private:
        mutable std::vector<const T*> m_objects;
};

#endif // RENDERERDRAWABLE_H_INCLUDED
