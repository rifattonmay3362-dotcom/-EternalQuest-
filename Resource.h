#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

// Enum for the 4 resource types in Eternal Quest
enum ResourceType {
    WOOD,     // Collected from trees in Forest Zone
    STONE,    // Collected from rocks in Mountain Zone
    FOOD,     // Collected from bushes/plants in various zones
    METAL     // Collected from ore deposits in Mountain/Desert Zones
};

// Resource class - represents collectible items in each zone
class Resource {
private:
    ResourceType type;
    float x, y;                  // Position in game world
    int amount;                  // Amount of resource (1-3)
    bool isCollected;            // Collection status
    float collectionRadius;      // Distance player needs to be within
    std::string zoneName;        // Which zone this resource belongs to
    
public:
    // Constructor
    Resource();
    Resource(ResourceType resType, float posX, float posY, int qty, std::string zone);
    
    // Getters
    ResourceType getType() const { return type; }
    float getX() const { return x; }
    float getY() const { return y; }
    int getAmount() const { return amount; }
    bool getCollected() const { return isCollected; }
    float getCollectionRadius() const { return collectionRadius; }
    std::string getZone() const { return zoneName; }
    
    // Get resource name as string (for display)
    std::string getTypeName() const;
    
    // Get resource icon/symbol (for console display)
    char getIcon() const;
    
    // Check if player is within collection range
    bool canCollect(float playerX, float playerY) const;
    
    // Collect the resource
    void collect();
    
    // Respawn the resource (for game design)
    void respawn();
    
    // Set position
    void setPosition(float posX, float posY);
};

#endif // RESOURCE_H
