#ifndef FUNCTIONBINDER_H_INCLUDED
#define FUNCTIONBINDER_H_INCLUDED

#include <map>
#include <functional>
#include "PacketTypes.h"

//This class initially arose out of the need to bind the send/receive
//functions in the NetworkManagers to the Packet types (Packet::TCP/
//Packet::UDP). However, this template can now be used to bind any
//function to any key type.

template<typename...>
class FunctionBinder;

template<typename MapKey, typename FuncReturnType, typename... FuncArguments>
class FunctionBinder<MapKey, FuncReturnType, FuncArguments...> {
  private:
	template<typename R, typename... A>
	using Functor = std::function<R(A...)>;

	template<typename K, typename R, typename... A>
	using Map = std::map<K, Functor<R, A...>>;

	Map<MapKey, FuncReturnType, FuncArguments...> m_map;

  public:
	FunctionBinder()
	            : m_map{} {
	}

	//when called, call() will execute the function mapped to the
	//given key value using the given arguments, if it exists in
	//the map.
	void call(MapKey _key, FuncArguments... _arg) {
		if (m_map.count(_key)) {
			m_map[_key](_arg...);
		}
	}

	void add(MapKey _key, Functor<FuncReturnType, FuncArguments...> _func) {
		m_map.insert({_key, _func});
	}
};

#endif // FUNCTIONBINDER_H_INCLUDED
