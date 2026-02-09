# AChoiEngine & A Knight's Legacy
Welcome to AChoiEngine!  
and A Knight's Legacy, the game built on top of AChoiEngine!

#### AChoiEngine
--------------
AChoiEngine, a custom 2D game engine, provides an open source framework for developers to build games utilizing modular build philosophy. ACE_GameObjects contain only data while the engine provides AI behavior and physics systems. Under this architecture, the player, enemies, and weapons are all considered ACE_GameObjects. Systems are meant to be modular, and decisions on how ACE_GameObjects interact within sysems is meant to be dictated and centralized within the main gameplay state. The main gameplay state decides which objects will interact with which systems and then calls the appropriate API. ACE_GameObjects keep track of their own data. Systems modify ACE_GameObjects. The main game state orchestrates the order of execution. 

I was inspired by ECS design principles, and so I wanted to build a hybrid engine that tried to stay true to the essence of those principles, while leveraging OOP to abstract away some complexity for the everyday game developer. I started this project in January 2026 and plan to continue building out on this core philosophy!

<!-- <p align="center">
  <img src="engine_game_diagram.png" width="700">
</p> -->

#### A Knight's Legacy
--------------
Imagine League of Legends meets Slay the Spire. This is the vision for A Knight's Legacy. A single player 2D top down game where as the player progresses, they are able to choose from a plethora of weapons, spells, and stat upgrades. Player experience can range from straightfoward hack and slash, to putting together huge combos. The direction of progression being dictaed by the random items the player picks up along the way. 

<!-- ## VIDEO
(I want to add a couple videos here. One describing gameplay, and one describing some of the tech.) -->

## Key Features
#### AChoiEngine

- Modular OOP component system architecture.  
- Dumb and Smart AI Behavior system. Smart AI Behavior utilizes vector fields to determine player position.  
- Overlapping Hitbox Detection and Collision Resolution systems.  
- Camera system.  
- World Map Loader that parses LDTK level editor files. (https://ldtk.io/)  
- Txt Map Loader  

#### A Knight's Legacy
### Controls

| Combat            | Movement         | Misc |  
|:------------------|:-----------------|:-----------------------|  
| `Space` – Attack  | `↑` – Move Up    | `Esc` – Return to Menu |  
| `Q` – Slide       | `↓` – Move Down  |                        |  
| `W` – Stun Hammer | `←` – Move Left  |                        |  
| `E` – Roll        | `→` – Move Right |                        |     
| `R` – Ult         |                  |                        |  
  

## Installation Instructions for Windows
<b>Requirements:</b>  
Windows 10 or 11  
Git  
CMake  
Visual Studio 2022

<b>Dependency note:</b>  
2/6/2026 - SDL3_mixer is a new release and therefore currently unavailable in vcpkg, so it is manually fetched directly from its Git Repo during CMake config.  
GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git  
GIT_Tag prerelease-3.1.2  


### Download and Install
#### 1) Open bash/developer powershell, clone the repository & navigate to project root  
```bash
git clone git@github-aschoi:aschoi/aKnightsLegacy.git  
```
```bash
cd aKnightsLegacy  
```
#### 2) Install dependencies with vcpkg   
If vcpkg is already installed on your machine:  
```bash
vcpkg install  
```
else:  
```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake  
```
#### 3) Configure build with CMake
```bash
cmake -S . -B build  
```

### Build and Run
#### 4) Build project  
```bash
cmake --build build  
```
or  
```bash
cmake --build build --config Release  
```

#### 5) Run executable 
```bash
./build/Debug/aKnightsLegacy.exe  
```
or  
```bash
./build/Release/aKnightsLegacy.exe  
```



## Design Decisions and Tradeoffs
As of Feb 6, 2026:  
Why SDL3 and SDL3_mixer? Why not SDL2_mixer?  
  
I chose SDL3 as my renderer for its updated and modernized API and rendering capabilities. After building out a majority of the engine and game logic, I found that after attempting to add audio, SDL3_mixer was unavailable on vcpkg, because it had just been released. As it turns out, SDL2_mixer is incompatable with SDL3, so my options were:  
1) Have CMake manually fetch SDL3_mixer from its repo  
or  
2) Large refactor of the entire codebase. (SDL2 uses ints for its rectangle coordinates and rendering, while SDL3 uses floats.) This would be a huge engineering cost.  

I went with option 1. 😅

## Known Issues and Planned Improvements
### Installation
2/6/2026 - SDL3_mixer is currently unavailable in vcpkg. My CMakeLists compensates by manually fetching from the SDL3_mixer repo.  
I plan to implement an option to turn off audio with CMakeLists if so desired.  

### AChoiEngine
- Wider variety of AI behavior.  
- Reduce Animation jitter.  
- Varied Collision Response behavior.  
- Mouse integration.  
- Support for different map loaders.  
- Refactor ACE_GameObject: shrink parameter clutter. Utilize custom structs that hold x, y, w, h data

### A Knight's Legacy
- More enemy types and more levels.  
- A great deal more weapons.  
- Unique game mechanics through the building of combos via getting different types of items.  
- Better artistic assets.  

## Contributions
Contributors are more than welcome! Pull request or contact me directly. 

## Contact and Acknowledgements
Alexander S Choi  
choi.alexander.s@gmail.com  
https://bsky.app/profile/aschoi.bsky.social  
https://github.com/aschoi/  


Special thanks to Jon Sourbeer!

## License
MIT License

Copyright (c) 2026 Alexander Choi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
