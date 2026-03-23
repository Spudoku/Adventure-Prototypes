# About

This project is a 2D open-world game engine written in C (with CC65) for the Atari 800(+) series of consoles founded by @Spudoku and @suspiciouslyBee. As a demonstrative project, development is working towards running a semi-faithful reimplementation of *Adventure* for the Atari 2600.

Originally, this project was intended to be a straight forward port of *Adventure*. However, there was a notible lack of resources regarding CC65 Atari 8-bit development. As there was insufficient resources or other code we could rely on, making an engine proved unavoidable.

The main goal of the project is to act as reference implementation for the wider retro gamedev community. As such, our code is prioritizing these pillars when possible:
* Make it clear
* Make it moddable
* And *if* needed, make it optimal

# Engine Features (as of 3/22/26)

The (yet untitled) engine we are building is striving towards an event-driven, pseudoobject-oriented paradigm. We believe that this will help those coming from more modern game engines learn how to use our skeleton to make their own creations.

Currently the engine supports:
* Antic Mode 7 display
* Combined Fine 2D Playfield Scrolling, that takes only 1-3% of a NTSC Frame
* Entity object system for the creation of new developer-defined entities
* Player singleton with input processing
* Basic sound generation 
* Single color PMG API
* Basic Collision
* Debug print support... via the printer

Our reference port also currently has:
* A recreated section of *Adventure*'s map, though without any barriers
* An animated dragon who chases the player
* Player invisible "item" that plays a noise when the fire button is pressed

# Building

Linux:
1. Please ensure that you have the latest version of CC65, preferably compiled from source. Some package repositories may have an outdated binary. 
2. Run `$ make`

Windows (pending update): 
1. Run the batch file



