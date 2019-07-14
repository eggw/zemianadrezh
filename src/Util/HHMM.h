#ifndef HHMM_H_INCLUDED
#define HHMM_H_INCLUDED

struct HHMM {
	HHMM(int _h, int _m)
	            : hours{_h}, minutes{_m} {};
	HHMM(const HHMM& _t)
	            : hours{_t.hours}, minutes{_t.minutes} {};

	bool operator==(const HHMM& t2) {
		return (hours == t2.hours &&
		        minutes == t2.minutes);
	}

	int hours;
	int minutes;
};

#endif // HHMM_H_INCLUDED
