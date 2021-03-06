# About

This is a personal project inspired by Terraria, written in modern C++. Special thanks to [mwaning](https://github.com/mwaning) and Frank for the original idea, asset contribution, and ongoing invaluable input.

The long term goal is to implement a variety of mechanics inherent to a game of this genre, including (but in no way limited to):

* Random world generation

* Enemies and friendly NPCs (AI)

* A day-night cycle with effects beyond just aesthetic ones (such as enemies spawning at night)

* An inventory system

* Combat (including equipment, weapons, and other items)

* Destroyable and placeable blocks and wall tiles 

All this should work seamlessly over a network connection.


## What works so far?

As of 12/7/2019, the following features are in:

* A basic Client-Server architecture

* Some basic program states, including the main menu, the multiplayer hosting/joining menu, and the in-game playing state

* The ability to connect to a local or remote host and have the world (only dirt and grass blocks at the moment), in addition to the attributes of the host and remote players, synced between both clients*

* A fully functional chat box

* A visual day-night cycle that uses a basic OpenGL shader to color the sky

*at the moment, there is no client-side latency prediction or compensation of any sort, leading to somewhat jittery movement of remote players.

An image showing the game in its current state:

![img](https://i.imgur.com/fOQjsZ6.png)


## Dependencies

This project makes use of [SFML 2.5.1](https://www.sfml-dev.org/), [SPSS](https://github.com/eggw/spss), and [entt](https://github.com/skypjack/entt), as well as [doctest](https://github.com/onqtam/doctest) for testing. The latter two have been included directly in the dependencies dir, as they are header-only.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[MIT](https://choosealicense.com/licenses/mit/)
