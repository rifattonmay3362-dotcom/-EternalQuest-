#include "Resource.h"
#include <cmath>

// Default constructor
Resource::Resource() 
    : type(WOOD), x(0), y(0), amount(1), isCollected(false), 
      collectionRadius(40.0f), zoneName("Unknown") {
}

// Parameterized constructor
Resource::Resource(ResourceType resType, float posX, float posY, int qty, std::string zone)
    : type(resType), x(posX), y(posY), amount(qty), isCollected(false),
      collectionRadius(40.0f), zoneName(zone) {
}

// Get resource name as string
std::string Resource::getTypeName() const {
    switch(type) {
        case WOOD:  return "Wood";
        case STONE: return "Stone";
        case FOOD:  return "Food";
        case METAL: return "Metal";
        default:    return "Unknown";
    }
}

// Get resource icon for display
char Resource::getIcon() const {
    switch(type) {
        case WOOD:  return 'W';  // W for Wood (tree)
        case STONE: return 'S';  // S for Stone (rock)
        case FOOD:  return 'F';  // F for Food (bush)
        case METAL: return 'M';  // M for Metal (ore)
        default:    return '?';
    }
}

// Check if player can collect this resource
bool Resource::canCollect(float playerX, float playerY) const {
    if (isCollected) {
        return false;
    }
    
    // Calculate distance between player and resource
    float dx = playerX - x;
    float dy = playerY - y;
    float distance = std::sqrt(dx * dx + dy * dy);
    
    return distance <= collectionRadius;
}

// Collect the resource
void Resource::collect() {
    isCollected = true;
}

// Respawn the resource (reset collection status)
void Resource::respawn() {
    isCollected = false;
}

// Set new position
void Resource::setPosition(float posX, float posY) {
    x = posX;
    y = posY;
}
