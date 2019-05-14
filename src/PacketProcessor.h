#ifndef PACKETPROCESSOR_H_INCLUDED
#define PACKETPROCESSOR_H_INCLUDED

#include <map>
#include <functional>
#include "PacketTypes.h"

//This class is mainly meant to be used with NetworkManagerClient and
//NetworkManagerServer. It will basically serve as a registry to which
//packet types (both TCP and UDP) can be mapped to functions. The func-
//tions can be called when a packet is sent or received as appropriate,
//and can be given an argument (typical use case is sf::Packet).
//
//We're going to have four maps to help us determine what functions
//should be called depending on what packets arrive or are sent.
//
//For each protocol (TCP, UDP), and for both sending and receiving,
//we have a map with the packet type as its key and a function we'll
//call when that packet is sent or received.
//
//Note that not all packets will have defined behaviours. For example,
//the client will never send DATA_WORLD to the server, so attempting
//to do so will not cause anything to happen.

template <typename...> class PacketProcessor;

template<typename A>
class PacketProcessor<A> {

  private:
  	template<typename Argument>
  	using Functor = std::function<void(Argument)>;

	template<typename Argument>
  	using TCPMap = std::map<Packet::TCP, Functor<Argument>>;

  	template<typename Argument>
  	using UDPMap = std::map<Packet::UDP, Functor<Argument>>;

  	TCPMap<A> m_TCPSender;
	TCPMap<A> m_TCPReceiver;

	UDPMap<A> m_UDPSender;
	UDPMap<A> m_UDPReceiver;

  public:
	PacketProcessor() :
		m_TCPSender{},
		m_TCPReceiver{},
		m_UDPSender{},
		m_UDPReceiver{} {
	}

	void performSendingAction(Packet::TCP _p, A _a) {
		if(m_TCPSender.count(_p)) {
			m_TCPSender[_p](_a);
		}
	}

	void performSendingAction(Packet::UDP _p, A _a) {
		if(m_UDPSender.count(_p)) {
			m_UDPSender[_p](_a);
		}
	}

	void performReceivingAction(Packet::TCP _p, A _a) {
		if(m_TCPReceiver.count(_p)) {
			m_TCPReceiver[_p](_a);
		}

	}

	void performReceivingAction(Packet::UDP _p, A _a) {
		if(m_UDPReceiver.count(_p)) {
			m_UDPReceiver[_p](_a);
		}
	}



	void addSendingAction(Packet::TCP _p, Functor<A> _f) {
		m_TCPSender.insert({_p, _f});
	}

	void addSendingAction(Packet::UDP _p, Functor<A> _f) {
		m_UDPSender.insert({_p, _f});
	}

	void addReceivingAction(Packet::TCP _p, Functor<A> _f) {
		m_TCPReceiver.insert({_p, _f});
	}

	void addReceivingAction(Packet::UDP _p, Functor<A> _f) {
		m_UDPReceiver.insert({_p, _f});
	}

};

#endif // PACKETPROCESSOR_H_INCLUDED
