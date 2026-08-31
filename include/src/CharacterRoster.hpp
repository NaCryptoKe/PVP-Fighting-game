#ifndef CHARACTER_ROSTER_HPP
#define CHARACTER_ROSTER_HPP

#include "src/Character.hpp"

// who the fighter is.
enum class CharacterType
{
    RYU
    // ...
};

class CharacterRoster
{
public:
   static Character createCharacter(CharacterType type);

private:
   static Character createRyu();
};

#endif  // CHARACTER_ROSTER_HPP
