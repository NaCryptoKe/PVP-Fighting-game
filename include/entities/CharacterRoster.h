#ifndef CHARACTER_ROSTER_H
#define CHARACTER_ROSTER_H

#include "entities/CharacterState.h"
#include <string>
#include <vector>

// Pre-defined configuration for a single attack
struct AttackConfig
{
    AttackType type;
    int startupFrame;
    int activeFrame;
    int recoveryFrame;
    float damageAmount;
    float hboffsetX;
    float hboffsetY;
    float width;
    float height;
    std::string name;
    float knockBackForce = 0.0f;
    bool blockable = true;
};

// Which sprite animation folder belongs to which state/attack
struct AnimationConfig
{
    std::string stateName;    // e.g. "IDLE", "WALK", "JUMP", "LIGHT_PUNCH", ...
    std::string folderPath;   // e.g. "assets/characters/stickman/idle/"  (loads 00.png..NN.png)
    int frameCount;           // how many frames are in that folder
    float frameDuration;      // seconds per frame (~0.08f = 12.5fps, 1/60 = 16.6fps)
    bool looping = true;
};

// All data needed to define a character in one place
struct CharacterData
{
    std::string name;
    float maxHealth;
    float walkSpeed;
    float jumpForce;
    float hitBoxOffsetX;
    float hitBoxOffsetY;
    float hitBoxWidth;
    float hitBoxHeight;
    std::vector<AttackConfig> attacks;
    std::vector<AnimationConfig> animations;
};

// Central roster that stores all character definitions.
// This single file is both the header AND the implementation:
// the game simply calls CharacterRoster::getCharacter("name") and applies the data.
class CharacterRoster
{
public:
    // Returns the data for the requested character, or the default Stickman.
    static CharacterData getCharacter(const std::string& name)
    {
        if (name == "Stickman")
            return createStickman();

        // Fallback to the default character if the requested name is unknown
        std::printf("CharacterRoster: unknown character \"%s\", using Stickman\n", name.c_str());
        return createStickman();
    }

    // Names of every character in the roster
    static const std::vector<std::string>& getAvailableCharacters()
    {
        static std::vector<std::string> available = { "Stickman" };
        return available;
    }

private:
    static CharacterData createStickman()
    {
        CharacterData data;
        data.name          = "Stickman";
        data.maxHealth     = 1000.0f;
        data.walkSpeed     = 450.0f;
        data.jumpForce     = 2571.43f;
        data.hitBoxOffsetX = -100.0f;
        data.hitBoxOffsetY = 0.0f;
        data.hitBoxWidth   = 200.0f;
        data.hitBoxHeight  = 300.0f;

        // --- Attacks ---
        AttackConfig lightPunch;
        lightPunch.type           = AttackType::LIGHT_PUNCH;
        lightPunch.startupFrame   = 4;
        lightPunch.activeFrame    = 3;
        lightPunch.recoveryFrame  = 8;
        lightPunch.damageAmount   = 50.0f;
        lightPunch.hboffsetX      = 100.0f;
        lightPunch.hboffsetY      = 150.0f;
        lightPunch.width          = 120.0f;
        lightPunch.height         = 80.0f;
        lightPunch.name           = "LIGHT_PUNCH";
        lightPunch.knockBackForce = 150.0f;
        lightPunch.blockable      = true;
        data.attacks.push_back(lightPunch);

        AttackConfig hardPunch;
        hardPunch.type           = AttackType::HARD_PUNCH;
        hardPunch.startupFrame   = 8;
        hardPunch.activeFrame    = 4;
        hardPunch.recoveryFrame  = 14;
        hardPunch.damageAmount   = 120.0f;
        hardPunch.hboffsetX      = 120.0f;
        hardPunch.hboffsetY      = 150.0f;
        hardPunch.width          = 160.0f;
        hardPunch.height         = 100.0f;
        hardPunch.name           = "HARD_PUNCH";
        hardPunch.knockBackForce = 350.0f;
        hardPunch.blockable      = true;
        data.attacks.push_back(hardPunch);

        // --- Animations (fill these in once you drop in sprite frames) ---
        // Every folder must contain frames named 00.png, 01.png, 02.png ...
        // Example:
        //   assets/characters/stickman/idle/00.png  01.png  02.png ...
        //
        // AnimationConfig idle;
        // idle.stateName    = "IDLE";
        // idle.folderPath   = "assets/characters/stickman/idle/";
        // idle.frameCount   = 3;
        // idle.frameDuration = 0.2f;
        // idle.looping      = true;
        // data.animations.push_back(idle);
        //
        // Repeat for "WALK", "JUMP", "BLOCK", "HITSTUN", "DEATH", "CROUCH"
        // and one animation per attack name ("LIGHT_PUNCH", "HARD_PUNCH").

        return data;
    }
};

#endif // CHARACTER_ROSTER_H