#include "RendererDrawable.h"

template <class T>
RendererDrawable<T>::RendererDrawable(sf::RenderWindow& _window)
    :RendererBase<T>(_window){
}


template <class T>
void RendererDrawable<T>::addObject(const T* _obj) const {
	auto it = std::find(m_objects.begin(), m_objects.end(), _obj);

	if(it == m_objects.end()) {
		m_objects.push_back(_obj);
	}
}

template <class T>
RendererDrawable<T>::~RendererDrawable() {

}

template <class T>
void RendererDrawable<T>::getInput(sf::Event& _event) {

}

template <class T>
void RendererDrawable<T>::update() {

}

template <class T>
void RendererDrawable<T>::draw() const {
	for(auto& object : m_objects) {
		RendererBase<T>::m_window.draw(*object);
	}
}

template <class T>
void RendererDrawable<T>::onResize(sf::Vector2f _newSize) {

}
