#ifndef INPUTLOCKER_H_INCLUDED
#define INPUTLOCKER_H_INCLUDED

#include "Singleton.h"

/*
This class serves one simple but vital purpose:

There needs to be a way to "lock" input so that the
player won't move around while typing a message in
the chat box
*/

class InputLocker : public Singleton<InputLocker> {
    public:
        InputLocker();

        bool isLocked() const;

        void lock();
        void unlock();
    private:
    	bool m_locked;
};


#endif // INPUTLOCKER_H_INCLUDED
