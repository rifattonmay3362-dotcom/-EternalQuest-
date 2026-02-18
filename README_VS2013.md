# Eternal Quest: Rebirth of Civilization
## Resource Collection & Inventory System

### Game Overview
A 2D survival-adventure game where players explore 5 zones (Forest, Mountain, Desert, River, Final City), collect resources, and defeat enemies to restore civilization.

---

## 📁 Files Included

### Header Files (.h)
- `Resource.h` - Resource class definition (Wood, Stone, Food, Metal)
- `Inventory.h` - Inventory management system
- `Player.h` - Player character with movement and collection
- `Zone.h` - Zone/Level management

### Implementation Files (.cpp)
- `Resource.cpp` - Resource class implementation
- `Inventory.cpp` - Inventory system implementation  
- `Player.cpp` - Player class implementation
- `Zone.cpp` - Zone management implementation
- `main.cpp` - Full game with menu (Visual Studio 2013)
- `demo_main.cpp` - Cross-platform demo version

---

## 🎮 Game Features Implemented

### ✅ Resource Collection System
- **4 Resource Types**: Wood, Stone, Food, Metal
- **Proximity-based Collection**: Auto-collect when player is within 40 units
- **Zone-specific Spawning**: Each zone has unique resource distribution
  - **Forest**: Lots of Wood (15), Good Food (10), Few Stone (3), No Metal
  - **Mountain**: Lots of Stone (20), Some Metal (8), Limited Wood/Food
  - **Desert**: Good Metal (10), Decent Stone (12), Little Wood/Food
  - **River**: Lots of Food (15), Moderate Wood (8), Balanced Stone/Metal
  - **Final City**: Balanced resources with emphasis on Metal (15)

### ✅ Inventory Management
- **Capacity Limits**: Wood (50), Stone (50), Food (30), Metal (20)
- **Add/Remove Functions**: For all resource types
- **Zone Requirements**: Progressive difficulty
  - Forest: 10 Wood, 5 Food
  - Mountain: 15 Stone, 5 Wood
  - Desert: 10 Food, 8 Stone
  - River: 12 Food, 10 Metal
  - Final City: 20 Wood, 20 Stone, 15 Food, 15 Metal

### ✅ Player System
- Movement (Arrow keys: W/A/S/D)
- Health management (100 HP)
- Position tracking
- Auto-collection when near resources
- Save/Load functionality

### ✅ Zone Management
- 5 distinct zones with unique resources
- Progressive unlocking system
- Resource respawning capability
- Zone completion tracking

### ✅ File-Based Save System
- Save player position, health, zone
- Save inventory state
- Load previous game state

---

## 🔧 Setup for Visual Studio 2013

### Method 1: Create New Project

1. Open Visual Studio 2013
2. **File → New → Project**
3. Select **Visual C++ → Win32 Console Application**
4. Name: `EternalQuest`
5. Click **OK** → **Finish**

6. **Add all source files to project:**
   - Right-click **Source Files** folder
   - **Add → Existing Item**
   - Select all `.cpp` files
   - Right-click **Header Files** folder
   - **Add → Existing Item**
   - Select all `.h` files

7. **Build the project:**
   - Press **F7** or **Build → Build Solution**

8. **Run the game:**
   - Press **Ctrl+F5** (Run without debugging)

---

### Method 2: Command Line (Developer Command Prompt)

1. Open **Developer Command Prompt for VS2013**
2. Navigate to your source folder:
   ```
   cd C:\path\to\your\EternalQuest
   ```

3. Compile:
   ```
   cl /EHsc main.cpp Resource.cpp Inventory.cpp Player.cpp Zone.cpp /Fe:EternalQuest.exe
   ```

4. Run:
   ```
   EternalQuest.exe
   ```

---

## 🎯 How to Use in Your Game

### 1. Creating a Zone with Resources

```cpp
// Create Forest Zone
Zone forestZone("Forest");

// Resources are automatically spawned based on zone type
// Forest will have: 15 Wood, 10 Food, 3 Stone, 0 Metal
```

### 2. Creating a Player

```cpp
// Create player at starting position
Player player(100.0f, 100.0f);

// Move player
player.moveUp();     // W key
player.moveDown();   // S key
player.moveLeft();   // A key
player.moveRight();  // D key
```

### 3. Collecting Resources

```cpp
// Auto-collect all resources within range
int collected = player.collectNearbyResources(forestZone.getResources());

if (collected > 0) {
    cout << "Collected " << collected << " resources!" << endl;
}
```

### 4. Checking Inventory

```cpp
// Display full inventory
player.getInventory().displayInventory();

// Get specific resource counts
int wood = player.getInventory().getWood();
int stone = player.getInventory().getStone();
int food = player.getInventory().getFood();
int metal = player.getInventory().getMetal();
```

### 5. Check Zone Completion

```cpp
// Check if player has enough resources for Forest Zone
if (player.getInventory().canCompleteForestZone()) {
    cout << "Ready to face the Forest Boss!" << endl;
}
```

### 6. Save/Load Game

```cpp
// Save game
player.saveToFile("player_save.txt");
player.getInventory().saveToFile("inventory_save.txt");

// Load game
player.loadFromFile("player_save.txt");
player.getInventory().loadFromFile("inventory_save.txt");
```

---

## 📊 Resource Distribution by Zone

| Zone        | Wood | Stone | Food | Metal |
|-------------|------|-------|------|-------|
| Forest      | 15   | 3     | 10   | 0     |
| Mountain    | 5    | 20    | 4    | 8     |
| Desert      | 2    | 12    | 6    | 10    |
| River       | 8    | 6     | 15   | 5     |
| Final City  | 10   | 10    | 10   | 15    |

---

## 🎮 Game Controls

| Key | Action              |
|-----|---------------------|
| W   | Move Up             |
| A   | Move Left           |
| S   | Move Down           |
| D   | Move Right          |
| I   | Show Inventory      |
| O   | Show Objectives     |
| Q   | Quit to Menu        |

---

## 🔍 Code Structure (OOP)

### Resource Class
- Represents collectible items
- Tracks position, type, amount
- Collection radius check
- Respawn functionality

### Inventory Class
- Manages all 4 resource types
- Add/Remove/Check resources
- Capacity limits
- Zone requirement checks
- Save/Load to file

### Player Class
- Position and movement
- Health system
- Inventory management
- Resource collection
- Save/Load state

### Zone Class
- Resource spawning
- Zone-specific distribution
- Unlock/Complete tracking
- Resource respawning

---

## ✅ Viva Demonstration Checklist

- [ ] Compile and run successfully in VS2013
- [ ] Main menu displays correctly
- [ ] Player can move in all directions (W/A/S/D)
- [ ] Resources spawn in Forest Zone
- [ ] Player auto-collects nearby resources
- [ ] Inventory updates in real-time
- [ ] All 4 resource types work (Wood, Stone, Food, Metal)
- [ ] Zone objectives check works
- [ ] Save/Load functionality works
- [ ] Clean code with proper comments
- [ ] OOP principles demonstrated

---

## 🐛 Troubleshooting

### Issue: "cannot open conio.h"
**Solution**: Use `main.cpp` which is specifically for Visual Studio 2013.
For cross-platform testing, use `demo_main.cpp`.

### Issue: "LNK2019 unresolved external symbol"
**Solution**: Make sure ALL .cpp files are added to the project:
- Resource.cpp
- Inventory.cpp
- Player.cpp
- Zone.cpp
- main.cpp

### Issue: Resources not collecting
**Solution**: Check that:
- Player is within 40 units of resource
- Resource hasn't already been collected
- Inventory isn't full for that resource type

---

## 📝 Next Steps for Full Game

1. ✅ **Resource Collection** - COMPLETE
2. ✅ **Inventory System** - COMPLETE
3. ✅ **Zone Management** - COMPLETE
4. ⏳ **Combat System** (Boss battles)
5. ⏳ **Enemy AI** (Regular enemies)
6. ⏳ **Day/Night System**
7. ⏳ **Graphics Integration** (SFML/SDL)
8. ⏳ **Sound Effects**
9. ⏳ **Level Progression**
10. ⏳ **Achievement System**

---

## 📚 Learning Objectives Met

✅ Clean code with meaningful variable names
✅ Object-Oriented Programming (Classes, Encapsulation)
✅ Resource management system
✅ Inventory logic implementation
✅ File I/O for save system
✅ Game state management
✅ Structured level design
✅ Progressive difficulty balancing

---

## 🎓 For Your Viva

**Be prepared to explain:**
1. Why you chose this class structure
2. How the resource collection proximity check works
3. How inventory capacity limits are enforced
4. How zone-specific resource spawning is implemented
5. The save/load file system
6. How zone progression requirements scale

**Good luck with your presentation! 🚀**

---

## License
Educational project for game development learning.
