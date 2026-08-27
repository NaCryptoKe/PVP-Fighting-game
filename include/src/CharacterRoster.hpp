#ifndef CHARACTER_ROSTER_HPP
#define CHARACTER_ROSTER_HPP

#include "src/Character.hpp"

// who the fighter is.
enum class CharacterType
{
    CHUN-LI
    // ...
};

class CharacterRoster
{
public:
   static Character createCharacter(CharacterType type);

private:
   static Character createChunLi();
};

#endif  // CHARACTER_ROSTER_HPP
