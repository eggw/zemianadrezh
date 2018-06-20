#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include "Entity.h"

constexpr float BLOCK_DIMENSIONS_X = 16;
constexpr float BLOCK_DIMENSIONS_Y = 16;

class Block : public Entity {
	public:
		Block();
		bool getVisibility() const;
		void setVisibility(bool _v);
	private:
		bool m_visible;
};


#endif // BLOCK_H_INCLUDED
