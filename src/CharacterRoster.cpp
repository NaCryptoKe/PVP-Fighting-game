#include "src/CharacterRoster.hpp"
#include "src/Character.hpp"

Character CharacterRoster::createCharacter(CharacterType type)
{
    switch (type)
    {
        case CharacterType::RYU:
            return createRyu();
    }

    return Character();
}

Character CharacterRoster::createRyu()
{
    Character character;

    character.loadAnimation(CharacterState::IDLE, "./assets/ryu/idle/", 4, 0.16f, true);

    character.setHurtboxes(
        80.0f,
        180.0f,
        90.0f,
        110.0f
    );

    character.init();

    return character;
}
