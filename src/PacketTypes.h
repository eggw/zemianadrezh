#ifndef PACKETTYPES_H_INCLUDED
#define PACKETTYPES_H_INCLUDED

namespace Packet {

    enum class Type
    {
        CHAT_MESSAGE,
        REQUEST_WORLD,
        DATA_WORLD,
        DATA_PLAYER
    };

    constexpr auto toInt(Type _t) {
        return static_cast<std::underlying_type_t<Type>>(_t);
    }

    constexpr Type toType(int _i) {
        return static_cast<Type>(_i);
    }

}

#endif // PACKETTYPES_H_INCLUDED
