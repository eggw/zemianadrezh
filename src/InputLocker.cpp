#include "InputLocker.h"

InputLocker::InputLocker()
    :m_locked{false} {

}

bool InputLocker::isLocked() const {
    return m_locked;
}

void InputLocker::lock() {
    m_locked = true;
}

void InputLocker::unlock() {
    m_locked = false;
}
