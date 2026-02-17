#ifndef INVENTORY_H
#define INVENTORY_H

#include "Resource.h"
#include <string>
#include <fstream>

// Inventory class - manages player's collected resources
class Inventory {
private:
    int wood;
    int stone;
    int food;
    int metal;
    
    // Max capacity for each resource (balancing)
    int maxWood;
    int maxStone;
    int maxFood;
    int maxMetal;
    
    // Requirements for each zone completion
    struct ZoneRequirements {
        int woodNeeded;
        int stoneNeeded;
        int foodNeeded;
        int metalNeeded;
    };
    
public:
    // Constructor
    Inventory();
    
    // Add resources to inventory
    bool addWood(int amount);
    bool addStone(int amount);
    bool addFood(int amount);
    bool addMetal(int amount);
    bool addResource(ResourceType type, int amount);
    
    // Remove/Use resources
    bool useWood(int amount);
    bool useStone(int amount);
    bool useFood(int amount);
    bool useMetal(int amount);
    bool useResource(ResourceType type, int amount);
    
    // Getters for resource counts
    int getWood() const { return wood; }
    int getStone() const { return stone; }
    int getFood() const { return food; }
    int getMetal() const { return metal; }
    int getResourceCount(ResourceType type) const;
    
    // Check if player has enough resources
    bool hasEnoughWood(int amount) const { return wood >= amount; }
    bool hasEnoughStone(int amount) const { return stone >= amount; }
    bool hasEnoughFood(int amount) const { return food >= amount; }
    bool hasEnoughMetal(int amount) const { return metal >= amount; }
    bool hasEnoughResource(ResourceType type, int amount) const;
    
    // Check if inventory is full for a resource type
    bool isWoodFull() const { return wood >= maxWood; }
    bool isStoneFull() const { return stone >= maxStone; }
    bool isFoodFull() const { return food >= maxFood; }
    bool isMetalFull() const { return metal >= maxMetal; }
    
    // Zone requirement checks
    bool canCompleteForestZone() const;      // Requires: 10 Wood, 5 Food
    bool canCompleteMountainZone() const;    // Requires: 15 Stone, 5 Wood
    bool canCompleteDesertZone() const;      // Requires: 10 Food, 8 Stone
    bool canCompleteRiverZone() const;       // Requires: 12 Food, 10 Metal
    bool canCompleteFinalCity() const;       // Requires: All resources
    
    // Display inventory
    void displayInventory() const;
    std::string getInventoryString() const;
    
    // Save/Load inventory to file
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    
    // Reset inventory
    void clear();
    
    // Get total resources
    int getTotalResources() const;
};

#endif // INVENTORY_H
