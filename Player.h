#ifndef PLAYER_H
#define PLAYER_H

#include "Inventory.h"
#include "Resource.h"
#include <vector>

// Player class - represents the player character
class Player {
private:
    float x, y;                  // Player position
    float speed;                 // Movement speed
    int health;                  // Player health
    int maxHealth;               // Maximum health
    std::string currentZone;     // Current zone player is in
    Inventory inventory;         // Player's inventory
    
public:
    // Constructor
    Player();
    Player(float startX, float startY);
    
    // Movement methods (Arrow key controls)
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void move(float dx, float dy);
    
    // Position getters and setters
    float getX() const { return x; }
    float getY() const { return y; }
    void setPosition(float newX, float newY);
    
    // Health management
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    void takeDamage(int damage);
    void heal(int amount);
    bool isAlive() const { return health > 0; }
    
    // Zone management
    std::string getCurrentZone() const { return currentZone; }
    void setCurrentZone(const std::string& zone) { currentZone = zone; }
    
    // Inventory access
    Inventory& getInventory() { return inventory; }
    const Inventory& getInventory() const { return inventory; }
    
    // Resource collection
    bool collectResource(Resource& resource);
    int collectNearbyResources(std::vector<Resource>& resources);
    
    // Display player status
    void displayStatus() const;
    
    // Save/Load player data
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
};

#endif // PLAYER_H
