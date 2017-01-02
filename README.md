## Gameboy Emulator

An emulator for the first version of the Nintendo Gameboy. Made for higher learning purposes, written from scratch in C++ using the [SFML library](http://www.sfml-dev.org/) for windowing & IO.

## Gameplay

![Blargg CPU tests](http://i.imgur.com/E0nMTgH.png)
![Tetris](http://i.imgur.com/FHt1keh.gif)
![Pokemon Red](http://i.imgur.com/PUo40EL.gif)
![Super Mario Land](http://i.imgur.com/Z9AWO9v.gif)   
![Legend of Zelda](http://i.imgur.com/Hbl72G9.gif)
![Kirby's Dream Land](http://i.imgur.com/vbEQ3Qk.gif)
![Minesweeper](http://i.imgur.com/Hv2fjPt.gif)
![Castellian](http://i.imgur.com/V8jyx4H.gif)

## Features
* Accurate CPU and Memory emulation
* 4-bit Grayscale Palette
* Plays most .gb games
* Game save states (up to 12 for each game)
* Ability to overclock CPU x100
* 60fps Display

## Folder Structure

> ./   
> &nbsp;&nbsp;&nbsp;&nbsp;/saves/   
> &nbsp;&nbsp;&nbsp;&nbsp;/roms/   
> &nbsp;&nbsp;&nbsp;&nbsp;\<emulator executable>

## Controls

| Keyboard Key | Function |
| :-: | :-------: |
| A | A |
| S | B |
| Z | Start |
| X | Select |
| Up | Up |
| Down | Down |
| Left | Left |
| Right | Right |
| LShift + F1 thru F12 | Save State |
| F1 thru 12 | Load Saved State |
| Space (hold) | Overclock CPU x100 |

## Future Plans

Although I already met my main goal of getting the emulator to a point where it could play my favorite childhood game (Pokemon Red), The following are things I would like to add to this project in the future when I have more free time:

* Implement Audio
* Add MBC2 support
* Add Gameboy Color support
* Add support for Gameshark memory editing cheats

## Missing Features / Known Bugs
* No audio support
* Currently the ROM is hardcoded into the program, and will need to be re-compiled to play a different ROM, this will be changed once I finish polishing the emulator up.
* Sprites are not rendered at each scanline, but all at once when each frame is rendered. this *may* cause visual glitches with sprites in some games.
* Sprites sometimes overlap each other with garbage background data. There are still a few conditional sprite rendering issues to work out, but they don't affect gameplay very much at all.
* Missing MBC2 support
* Loading save states sometimes freezes the emulator, can be fixed by restarting and trying to load it again. The save states are always valid, the emulator just has trouble loading them sometimes.
