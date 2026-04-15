Eternal Quest – Rebirth of Civilization

Game Description
Eternal Quest – Rebirth of Civilization is a single-player 2D action-adventure game developed using the iGraphics library in C/C++.
The game takes the player through three unique stages combining platforming, combat, and aerial gameplay.

Game Overview
- Intro Level (Platformer): Climb moving stairs and battle enemies
- Level 1 (Ground Combat): Side-scrolling combat and treasure collection
- Level 2 (Aerial Shooter): Fly a plane, dodge obstacles, collect health gems

Features
- Smooth player movement (run, jump, back movement)
- Shooting mechanics and combat system
- Enemy AI with different behaviors
- Health, shield, and magic bar system
- Collectibles (keys, stones, treasure chests)
- Save & Load system (binary file handling)
- High score leaderboard (Top 5 scores saved)
- Multi-level gameplay with different mechanics

Project Details
Course: CSE-1200 – Software Development I
Language: C / C++
Graphics Library: iGraphics
IDE: Visual Studio 2010 / 2013
Platform: Windows PC
Genre: 2D Action-Adventure

How to Run the Project
Requirements:
- Visual Studio 2013
- iGraphics Library
- MinGW (if required)

Steps:
1. Open Visual Studio 2013
2. File → Open → Project/Solution
3. Select the .sln file
4. Build → Build Solution
5. Debug → Start Without Debugging

How to Play
Controls:
- Move Left: A
- Move Right: D
- Jump: W
- Shoot: (defined in code)

Game Mechanics
- Player loses health when hit by enemies or obstacles
- Game ends when health reaches zero
- Points earned by defeating enemies
- Progress can be saved and resumed
- High scores are stored using file system

Project Structure
- EternalQuest.cpp – Main game loop
- Player.h – Player movement & animation
- Enemy.h – Enemy logic
- IntroLevels.h – Intro system
- Collision.h – Collision detection
- GameSave.h / ScoreSave.h – Save & leaderboard

Challenges & Solutions
- Collision issues → Adjusted hitboxes
- Animation flicker → Used multiple timers
- Save corruption → Structured binary file system

Development Workflow
- Used GitHub for version control
- Worked with branches and pull requests
- Managed collaboration through commits and merging

Future Improvements
- Add sound effects and music
- Multiplayer mode
- Additional levels and improvements
