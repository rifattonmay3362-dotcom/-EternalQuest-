#ifndef ZONE_H
#define ZONE_H

#include "Resource.h"
#include <vector>
#include <string>

// Zone class - represents each level/zone in the game
class Zone {
private:
    std::string name;
    std::vector<Resource> resources;
    bool isCompleted;
    bool isUnlocked;
    
    // Resource spawn information
    int woodCount;
    int stoneCount;
    int foodCount;
    int metalCount;
    
public:
    // Constructor
    Zone();
    Zone(const std::string& zoneName);
    
    // Getters
    std::string getName() const { return name; }
    bool getCompleted() const { return isCompleted; }
    bool getUnlocked() const { return isUnlocked; }
    std::vector<Resource>& getResources() { return resources; }
    const std::vector<Resource>& getResources() const { return resources; }
    
    // Setters
    void setCompleted(bool completed) { isCompleted = completed; }
    void setUnlocked(bool unlocked) { isUnlocked = unlocked; }
    
    // Spawn resources in this zone
    void spawnResources();
    
    // Respawn all resources
    void respawnAllResources();
    
    // Get zone-specific resource counts
    int getAvailableWood() const;
    int getAvailableStone() const;
    int getAvailableFood() const;
    int getAvailableMetal() const;
};

#endif // ZONE_H
