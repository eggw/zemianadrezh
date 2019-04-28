#include "WorldTime.h"

WorldTime::WorldTime(HHMM _t) : m_time{0, 0}, m_paused{false} {
	set(_t);
}

void WorldTime::set(HHMM _t) {
	if (_t.minutes >= 60) {
		_t.minutes = 0;
		_t.hours++;
	}

	if (_t.hours >= 24) {
		_t.hours = 0;
	}
	m_time = _t;
}

//increment by one minute
void WorldTime::tick() {
	if (m_paused) {
		return;
	}
	set({m_time.hours, m_time.minutes + 1});
}

HHMM WorldTime::get() const {
	return m_time;
}

const std::string WorldTime::getString() const {
	std::string minutesStr{std::to_string(m_time.minutes)};
	if (m_time.minutes < 10) {
		minutesStr = "0" + minutesStr;
	}
	return std::to_string(m_time.hours) + ":" + minutesStr;
}

void WorldTime::pause() const {
	m_paused = true;
}

void WorldTime::unpause() const {
	m_paused = false;
}
