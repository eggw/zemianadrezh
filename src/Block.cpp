#include "Block.h"

#include <random>
#include <chrono>

Block::Block()
	:m_visible(true){

	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	static std::mt19937 mt_rand(seed);

	std::uniform_int_distribution<int> uniform_dist(1, 20);
    int mean = uniform_dist(mt_rand);

    if(mean == 1){
		m_visible = false;
    }
}

bool Block::getVisibility() const{
	return m_visible;
}

void Block::setVisibility(bool _v){
	m_visible = _v;
}
