# QUICK START GUIDE - Visual Studio 2013

## 🚀 Get Started in 5 Minutes!

### Step 1: Download All Files
Make sure you have these files in one folder:
- Resource.h & Resource.cpp
- Inventory.h & Inventory.cpp
- Player.h & Player.cpp
- Zone.h & Zone.cpp
- main.cpp (for VS2013) OR demo_main.cpp (for testing)

### Step 2: Open Visual Studio 2013
1. Launch Visual Studio 2013
2. File → New → Project
3. Visual C++ → Win32 Console Application
4. Name it: "EternalQuest"
5. Click OK → Finish

### Step 3: Add Your Files
1. Right-click "Source Files" folder
   - Add → Existing Item
   - Select all .cpp files

2. Right-click "Header Files" folder
   - Add → Existing Item
   - Select all .h files

### Step 4: Build & Run
1. Press **F7** to build
2. Press **Ctrl+F5** to run
3. Enjoy your game!

---

## 📋 What You'll See

```
========================================
   ETERNAL QUEST: REBIRTH OF CIVILIZATION
========================================

  1. Start New Game
  2. Options
  3. Exit

========================================
Enter your choice: 
```

Select **1** to start the Forest Zone!

---

## 🎮 Game Controls

**W** = Move Up
**S** = Move Down  
**A** = Move Left
**D** = Move Right
**I** = Show Inventory
**O** = Show Objectives
**Q** = Quit to Menu

---

## ✅ Success Indicators

You'll know it's working when:
- ✓ Game compiles without errors
- ✓ Main menu appears
- ✓ You can move with WASD
- ✓ Resources auto-collect when near
- ✓ Inventory updates show Wood/Stone/Food/Metal counts
- ✓ Objectives complete when you collect enough resources

---

## 🎯 Forest Zone Objectives

Collect these resources to complete the zone:
- **10 Wood** (from trees)
- **5 Food** (from bushes)

When complete, you can face the Forest Boss!

---

## 🆘 Quick Fixes

**Problem**: "Cannot find conio.h"
**Fix**: Use `main.cpp` (designed for VS2013)

**Problem**: "Unresolved external symbol"
**Fix**: Make sure ALL .cpp files are in the project

**Problem**: "0 resources collected"
**Fix**: Move around more! Resources spawn at random positions

---

## 📊 Expected Output Example

```
==========================================
           PLAYER STATUS
==========================================
  Health:   100/100
  Position: (150, 140)
  Zone:     Forest
==========================================

==========================================
  CURRENT ZONE: Forest
==========================================
  Resources Available:
    Wood:  40 [W]
    Stone: 8 [S]
    Food:  16 [F]
    Metal: 0 [M]
==========================================

  [+] Collected 1 Wood!
  [+] Collected 2 Food!

==========================================
           RESOURCE INVENTORY
==========================================
  Wood:  3/50 [W]
  Stone: 0/50 [S]
  Food:  2/30 [F]
  Metal: 0/20 [M]
==========================================
  Total: 5 resources
==========================================
```

---

## 🎓 For Your Viva

Be ready to show:
1. Code compiling successfully
2. Game running smoothly
3. Resource collection working
4. Inventory updating correctly
5. Clean code structure (OOP)
6. Save/load functionality

---

**That's it! You're ready to go! 🎉**

For detailed documentation, see: README_VS2013.md
