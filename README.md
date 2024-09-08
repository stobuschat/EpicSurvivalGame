# Zombie Survival Game (UE 5.4 + StateTree Experiment)

This is a **fork** of the [EpicSurvivalGame](https://github.com/tomlooman/EpicSurvivalGame) project, originally developed by [Tom Looman](https://www.tomlooman.com). I have updated the project to work with **Unreal Engine 5.4**, and migrated the AI system from **Behavior Trees** to **StateTrees** for experimenting with the new system in Unreal Engine.

## Disclaimer

This is my first Unreal Engine project, and I'm primarily using it to experiment with **StateTrees** and **Unreal Engine**. This project is **not** intended to be used as a production-ready example. It serves primarily as a sandbox for **learning** and **experimenting**. As such, you may encounter multiple approaches to similar tasks as I test and compare different methods. Treat this as a starting point, not as a reference for best practices.

## Key Changes

- **Updated to UE 5.4**: The project has been made compatible with Unreal Engine 5.4.
- **AI Migration**: The zombie AI has been migrated from Behavior Trees to StateTrees. This migration covers basic zombie AI behavior, such as patrolling, sensing, and attacking the player. The StateTrees use a custom StateTree Class/Schema, and StateTreeTasks are implemented in C++. The old AI is still part of the project and when Zombies are respawned it randomly selects between the old and new Zombie/AI.

## Original Project Overview

The original **EpicSurvivalGame** is a third-person survival game made entirely in C++, originally built as a tutorial series for Unreal Engine. It covers many aspects of C++ game development, including AI systems, multiplayer functionality, item systems, and more. For a detailed overview, see the [main documentation page](https://www.tomlooman.com/unreal-engine-cpp-survival-sample-game/).

### Original Project Features

- Character movement with animations and networking support
- Weapon systems, inventory, damage, and respawn mechanics
- AI Zombie enemies (originally using Behavior Trees)
- Full gameplay loop with items, scoring, and time of day
- Multiplayer networking

### Sections

- [Section 1: Character Movement, Interaction, Hunger System](https://www.tomlooman.com/survival-sample-game-for-ue4/section-one/)
- [Section 2: Weapon Systems, Flashlight, Inventory, Damage, Respawn](https://www.tomlooman.com/survival-sample-game-for-ue4/section-two/)
- [Section 3: AI "Zombie" Enemy with PawnSensing and Behavior Tree](https://www.tomlooman.com/survival-sample-game-for-ue4/section-three/)
- [Section 4: Gameloop with Enemies, Items, Scoring, and Time of Day](https://nerivec.github.io/old-ue4-wiki/pages/survival-sample-game-section-4.html)
- [Section 5: Object Carrying and Networking](https://nerivec.github.io/old-ue4-wiki/pages/survival-sample-game-section-5.html)
- [Section 6: Bug Fixes and Polish](https://nerivec.github.io/old-ue4-wiki/pages/survival-sample-game-section-6.html)

## How to Use

1. Clone the repository to your local machine.
2. Open the project in **Unreal Engine 5.4**.
3. Compile the project in **Editor** or **Visual Studio**.
4. Explore the changes in the AI logic for the Zombie, which now uses **StateTree** instead of the previous **Behavior Tree** system.

## Credits

- Original Project: [Tom Looman](https://github.com/tomlooman/EpicSurvivalGame)
