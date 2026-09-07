#include "entities/CharacterRoster.h"

CharacterRoster::CharacterRoster()
{
    initializeDefaultCharacters();
}

void CharacterRoster::initializeDefaultCharacters()
{
    // Default character: Ryu
    CharacterStats ryu = {
        .name = "Ryu",
        .maxHealth = 1000.0f,
        .walkSpeed = 450.0f,
        .jumpForce = 2571.43f,
        .attackDamage = 50.0f,
        .blockDamageReduction = 0.25f
    };
    registerCharacter("ryu", ryu);

    // Default character: Ken
    CharacterStats ken = {
        .name = "Ken",
        .maxHealth = 950.0f,
        .walkSpeed = 500.0f,
        .jumpForce = 2600.0f,
        .attackDamage = 60.0f,
        .blockDamageReduction = 0.20f
    };
    registerCharacter("ken", ken);

    // Default character: Chun-Li
    CharacterStats chunli = {
        .name = "Chun-Li",
        .maxHealth = 1050.0f,
        .walkSpeed = 420.0f,
        .jumpForce = 2500.0f,
        .attackDamage = 45.0f,
        .blockDamageReduction = 0.30f
    };
    registerCharacter("chunli", chunli);

    // Default character: Blanka
    CharacterStats blanka = {
        .name = "Blanka",
        .maxHealth = 1100.0f,
        .walkSpeed = 480.0f,
        .jumpForce = 2400.0f,
        .attackDamage = 55.0f,
        .blockDamageReduction = 0.35f
    };
    registerCharacter("blanka", blanka);
}

void CharacterRoster::registerCharacter(const std::string& id, const CharacterStats& stats)
{
    characters[id] = stats;
}

CharacterStats CharacterRoster::getCharacter(const std::string& id) const
{
    auto it = characters.find(id);
    if (it != characters.end())
    {
        return it->second;
    }
    
    // Return default Ryu if character not found
    return {
        .name = "Unknown",
        .maxHealth = 1000.0f,
        .walkSpeed = 450.0f,
        .jumpForce = 2571.43f,
        .attackDamage = 50.0f,
        .blockDamageReduction = 0.25f
    };
}

bool CharacterRoster::hasCharacter(const std::string& id) const
{
    return characters.find(id) != characters.end();
}

std::unordered_map<std::string, CharacterStats> CharacterRoster::getAllCharacters() const
{
    return characters;
}
