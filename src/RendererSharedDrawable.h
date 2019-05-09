#ifndef RENDERERSHAREDDRAWABLE_H_INCLUDED
#define RENDERERSHAREDDRAWABLE_H_INCLUDED

#include "RendererBase.h"

/*
This class is functionally identical to RendererDrawable, except it's designed to
take in weak_ptrs. This is to be  used for drawing things owned by a shared_ptr that
may go out of scope. The weak_ptrs will detect that and remove it from m_objects when
appropriate.
*/

template <class T>
class RendererSharedDrawable : public RendererBase<T> {
	public:
		RendererSharedDrawable(sf::RenderWindow& _window)
			:RendererBase<T>(_window) {
		};

		~RendererSharedDrawable() {
		};

		void addObject(std::weak_ptr<T> _obj) const {
			auto pos = std::find_if(m_objects.begin(),
									m_objects.end(),
            [&_obj](const auto& o){
                return _obj.lock() == o.lock();
            });

			if (pos == m_objects.end()) {
				m_objects.push_back(_obj);
			}
		};

		void draw() const {
			auto it{m_objects.begin()};
			while (it != m_objects.end()) {
				if (!it->expired()) {
					RendererBase<T>::m_window.draw(*(it->lock()));
					++it;
				}
				else {
					it = m_objects.erase(it);
				}
			}
		};

	private:
	//Data members --------------------------------
	mutable std::vector<std::weak_ptr<T>> m_objects;
	//---------------------------------------------
};

#endif // RENDERERSHAREDDRAWABLE_H_INCLUDED
