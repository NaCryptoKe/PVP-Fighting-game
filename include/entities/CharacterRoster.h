#ifndef CHARACTER_ROSTER_H
#define CHARACTER_ROSTER_H

#include <string>
#include <unordered_map>

struct CharacterStats {
    std::string name;
    float maxHealth;
    float walkSpeed;
    float jumpForce;
    float attackDamage;
    float blockDamageReduction;
};

class CharacterRoster {
private:
    std::unordered_map<std::string, CharacterStats> characters;
    
    void initializeDefaultCharacters();

public:
    CharacterRoster();
    
    // Register a new character
    void registerCharacter(const std::string& id, const CharacterStats& stats);
    
    // Get character stats
    CharacterStats getCharacter(const std::string& id) const;
    
    // Check if character exists
    bool hasCharacter(const std::string& id) const;
    
    // Get all character IDs
    std::unordered_map<std::string, CharacterStats> getAllCharacters() const;
};

#endif // CHARACTER_ROSTER_H
