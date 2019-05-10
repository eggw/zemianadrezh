# About

This is a personal project inspired by Terraria, written in modern C++. Special thanks to [mwaning](https://github.com/mwaning) and Frank for the original idea, asset contribution, and ongoing invaluable input.

The long term goal is to implement a variety of mechanics inherent to a game of this genre, including (but in no way limited to):

* Random world generation

* Enemies and friendly NPCs (AI)

* A day-night cycle

* An inventory system

* Combat (including equipment, weapons, and other items)

All this should work seamlessly over a network connection.


## What works so far?

As of 10/5/2019, the following features are in:

* A basic Client-Server architecture

* Some basic program states, including the main menu, the multiplayer hosting/joining menu, and the in-game playing state

* The ability to connect to a local or remote host and have the world (only dirt blocks), in addition to the attributes of the host and remote players, synced between both clients*

* A fully functional chat box

* A visual day-night cycle that uses a basic OpenGL shader to color the sky

*at the moment, there is no client-side latency prediction or compensation of any sort, leading to somewhat jittery movement of remote players.


## Dependencies

This project makes use of [SFML 2.5.1](https://www.sfml-dev.org/) and [entt](https://github.com/skypjack/entt). The latter has been included directly in the source in the dependencies dir, as it is header-only.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[MIT](https://choosealicense.com/licenses/mit/)
