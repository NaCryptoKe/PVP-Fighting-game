#include "src/CharacterRoster.hpp"

Character CharacterRoster::createCharacter(CharacterType type)
{
    switch (type)
    {
        case CharacterType::CHUN_LI:
            return createChunLi();
    }

    return Character();
}

Character CharacterRoster::createChunLi()
{
    Character character;

    character.loadIdleAnimation("./assets/characters/chun-li/idle/", 3, 0.16f);
    character.loadJumpAnimation("./assets/characters/chun-li/jump/", 6, 0.16f);
    character.loadWalkAnimation("./assets/characters/chun-li/walk/", 14, 0.16f);

    character.setHurtboxes(
        80.0f,
        180.0f,
        90.0f,
        110.0f
    );

    character.init();

    return character;
}
