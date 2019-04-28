#pragma once

#include <string>

struct HHMM {
	HHMM(int _h, int _m) :
		hours{ _h },
		minutes{ _m }
	{};

	HHMM(const HHMM& _t) :
		hours	{ _t.hours },
		minutes	{ _t.minutes }
	{};

	int hours;
	int minutes;
};

class WorldTime {
	public:
		WorldTime(HHMM _t);
		void set(HHMM _t);
		//increment by one minute
		void tick();

		HHMM get() const;
		const std::string getString() const;

		void pause() const;
		void unpause() const;
	private:
		HHMM m_time;
		mutable bool m_paused;
};

