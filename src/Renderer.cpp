#include "Renderer.h"

template <class T>
Renderer<T>::Renderer(sf::RenderWindow& _window) {

}

template <class T>
void Renderer<T>::addObject(const T* _obj) const {
	auto it = std::find(m_objects.begin(), m_objects.end(), _obj);

    if(it == m_objects.end()) {
        m_objects.push_back(_obj);
    }
}

template <class T>
void Renderer<T>::getInput(sf::Event& _event){

}

template <class T>
void Renderer<T>::update(){

}

template <class T>
void Renderer<T>::draw(bool _keepView) const{
	sf::View previousView = m_window.getView();

	m_window.setView(m_view);

    for(auto& object : m_objects) {
        m_window.draw(*object);
    }

    if(_keepView){
        m_window.setView(previousView);
    }
}

template <class T>
void Renderer<T>::onResize(sf::Vector2f _newSize){

}
