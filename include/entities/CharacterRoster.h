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
    float hitstunTime = 0.3f;
    bool blockable = true;
};

// Which sprite animation folder belongs to which state/attack
struct AnimationConfig
{
    std::string stateName;    // e.g. "IDLE", "WALK_FORWARD", "LIGHT_PUNCH", ...
    std::string folderPath;   // e.g. "assets/characters/ryu/idle/"  (loads NN.png..NN.png)
    int frameCount;           // how many frames to load from that folder
    int startFrame = 0;       // first frame number (ryu clips mostly start at 01.png)
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
    float spriteScale = 1.0f; // sprite pixels -> world units multiplier
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
    // Returns the data for the requested character, defaulting to Ryu.
    static CharacterData getCharacter(const std::string& name)
    {
        if (name == "Ryu")
            return createRyu();

        if (name == "Stickman")
            return createStickman();

        // Fallback to the default character if the requested name is unknown
        std::printf("CharacterRoster: unknown character \"%s\", using Ryu\n", name.c_str());
        return createRyu();
    }

    // Names of every character in the roster
    static const std::vector<std::string>& getAvailableCharacters()
    {
        static std::vector<std::string> available = { "Ryu", "Stickman" };
        return available;
    }

private:
    // Adds one animation entry; the folder must hold frames named "NN.png"
    static void addAnim(
        CharacterData& data,
        const std::string& stateName,
        const std::string& folderPath,
        int frameCount, int startFrame,
        float frameDuration, bool looping
    )
    {
        AnimationConfig anim;
        anim.stateName     = stateName;
        anim.folderPath    = folderPath;
        anim.frameCount    = frameCount;
        anim.startFrame    = startFrame;
        anim.frameDuration = frameDuration;
        anim.looping       = looping;
        data.animations.push_back(anim);
    }

    // Adds one attack entry
    static void addAttack(
        CharacterData& data,
        AttackType type, const std::string& name,
        int startupFrame, int activeFrame, int recoveryFrame,
        float damageAmount,
        float hboffsetX, float hboffsetY, float width, float height,
        float knockBackForce, float hitstunTime, bool blockable
    )
    {
        AttackConfig attack;
        attack.type           = type;
        attack.name           = name;
        attack.startupFrame   = startupFrame;
        attack.activeFrame    = activeFrame;
        attack.recoveryFrame  = recoveryFrame;
        attack.damageAmount   = damageAmount;
        attack.hboffsetX      = hboffsetX;
        attack.hboffsetY      = hboffsetY;
        attack.width          = width;
        attack.height         = height;
        attack.knockBackForce = knockBackForce;
        attack.hitstunTime    = hitstunTime;
        attack.blockable      = blockable;
        data.attacks.push_back(attack);
    }

    static CharacterData createRyu()
    {
        CharacterData data;
        data.name          = "Ryu";
        data.maxHealth     = 1000.0f;
        data.walkSpeed     = 450.0f;
        data.jumpForce     = 2571.43f;
        // Ryu's frames are roughly 66x93 px; scale them up to match the
        // 200x300 hitbox and the stage scale (fallback quad is 300 tall).
        data.spriteScale   = 3.2f;
        data.hitBoxOffsetX = -100.0f;
        data.hitBoxOffsetY = 0.0f;
        data.hitBoxWidth   = 200.0f;
        data.hitBoxHeight  = 300.0f;

        // --- Standing attacks ---
        addAttack(data, AttackType::LIGHT_PUNCH, "LIGHT_PUNCH",  4, 3,  8,  50.0f, 100.0f, 150.0f, 120.0f,  80.0f, 150.0f, 0.30f, true);
        addAttack(data, AttackType::LIGHT_KICK,  "LIGHT_KICK",   5, 3,  9,  60.0f, 130.0f, 100.0f, 140.0f,  70.0f, 180.0f, 0.30f, true);
        addAttack(data, AttackType::HARD_PUNCH,  "HARD_PUNCH",   8, 4, 14, 120.0f, 120.0f, 150.0f, 160.0f, 100.0f, 350.0f, 0.45f, true);
        addAttack(data, AttackType::HARD_KICK,   "HARD_KICK",   10, 4, 16, 130.0f, 150.0f, 110.0f, 180.0f,  90.0f, 400.0f, 0.45f, true);

        // --- Crouching attacks ---
        addAttack(data, AttackType::LIGHT_PUNCH, "CROUCH_LIGHT_PUNCH", 4, 3,  8,  40.0f, 110.0f,  70.0f, 120.0f,  70.0f, 120.0f, 0.30f, true);
        addAttack(data, AttackType::LIGHT_KICK,  "CROUCH_LIGHT_KICK",  5, 3,  9,  50.0f, 130.0f,  60.0f, 140.0f,  70.0f, 150.0f, 0.30f, true);
        addAttack(data, AttackType::HARD_PUNCH,  "CROUCH_HARD_PUNCH",  7, 4, 15, 110.0f,  90.0f, 140.0f, 130.0f, 170.0f, 350.0f, 0.45f, true);
        addAttack(data, AttackType::HARD_KICK,   "CROUCH_HARD_KICK",   8, 4, 16, 100.0f, 140.0f,  40.0f, 170.0f,  70.0f, 300.0f, 0.45f, true);

        // --- Jumping attacks ---
        addAttack(data, AttackType::LIGHT_PUNCH, "JUMP_LIGHT_PUNCH", 4, 4,  8,  45.0f, 110.0f, 150.0f, 120.0f,  80.0f, 150.0f, 0.30f, true);
        addAttack(data, AttackType::LIGHT_KICK,  "JUMP_LIGHT_KICK",  5, 4,  9,  55.0f, 130.0f, 120.0f, 140.0f,  80.0f, 180.0f, 0.30f, true);
        addAttack(data, AttackType::HARD_PUNCH,  "JUMP_HARD_PUNCH",  7, 5, 12, 100.0f, 120.0f, 150.0f, 150.0f, 100.0f, 300.0f, 0.40f, true);
        addAttack(data, AttackType::HARD_KICK,   "JUMP_HARD_KICK",   8, 5, 14, 120.0f, 140.0f, 100.0f, 170.0f, 110.0f, 350.0f, 0.40f, true);

        // --- Specials ---
        // Throw: unblockable, the victim is thrown backwards (BACKWARD_THROW anim)
        addAttack(data, AttackType::HARD_PUNCH, "THROW", 3, 2, 12, 100.0f,  80.0f, 120.0f,  90.0f, 120.0f, 450.0f, 0.55f, false);
        // Fireball: zero melee box — the spawned projectile carries the damage
        addAttack(data, AttackType::HARD_PUNCH, "FIREBALL", 6, 2, 14, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.35f, true);


        // --- Animations ---
        // Every folder holds frames named 00.png/01.png..NN.png; the idle clip
        // starts at 00.png while every other ryu clip starts at 01.png.
        // State keys must match the names used in Character::updateAnimation().
        // Durations are tuned so one-shot clips roughly cover their state time
        // (attack totals = startup+active+recovery frames at 60fps).
        const std::string root = "assets/characters/ryu/";
        addAnim(data, "IDLE",                  root + "idle/",                 4, 0, 0.15f,  true);
        addAnim(data, "WALK_FORWARD",          root + "dash_front/",          6, 1, 0.08f,  true);
        addAnim(data, "WALK_BACKWARD",         root + "dash/",                5, 1, 0.08f,  true);
        addAnim(data, "JUMP",                  root + "jump/",                6, 1, 0.08f,  false);
        addAnim(data, "JUMP_FORWARD",          root + "jump_front/",          6, 1, 0.08f,  false);
        addAnim(data, "BLOCK",                 root + "blocking/",            2, 1, 0.20f,  true);
        addAnim(data, "BLOCK_HIT",             root + "block_hit/",           1, 1, 0.25f,  false);
        addAnim(data, "BLOCK_TO_CROUCH_BLOCK", root + "block_to_crouch_block/", 2, 1, 0.08f, false);
        addAnim(data, "CROUCH_BLOCK",          root + "crouch_block/",        2, 1, 0.20f,  true);
        addAnim(data, "CROUCH",                root + "crouch/",              2, 2, 0.20f,  true);

        // Attack clips (looked up by attack name or CROUCH_/JUMP_ variant)
        addAnim(data, "LIGHT_PUNCH",           root + "light_front_punch/",   2, 1, 0.12f,  false);
        addAnim(data, "LIGHT_KICK",            root + "light_low_back_kick/", 4, 1, 0.07f,  false);
        addAnim(data, "HARD_PUNCH",            root + "hard_overhead_punch/", 9, 1, 0.028f, false);
        addAnim(data, "HARD_KICK",             root + "hard_turn_kick/",      5, 1, 0.10f,  false);
        addAnim(data, "CROUCH_LIGHT_PUNCH",    root + "crouch_front_punch/",  2, 1, 0.12f,  false);
        addAnim(data, "CROUCH_LIGHT_KICK",     root + "low_crouch_front_kick/", 2, 1, 0.14f, false);
        addAnim(data, "CROUCH_HARD_PUNCH",     root + "crouch_uppercut/",     3, 1, 0.14f,  false);
        addAnim(data, "CROUCH_HARD_KICK",      root + "sweeping_low_kick/",   4, 1, 0.115f, false);
        addAnim(data, "JUMP_LIGHT_PUNCH",      root + "jumping_front_punch/", 2, 1, 0.13f,  false);
        addAnim(data, "JUMP_LIGHT_KICK",       root + "jump_kick/",           2, 1, 0.15f,  false);
        addAnim(data, "JUMP_HARD_PUNCH",       root + "jumping_back_punch/",  2, 1, 0.20f,  false);
        addAnim(data, "JUMP_HARD_KICK",        root + "jump_spin_kick/",      8, 1, 0.055f, false);
        addAnim(data, "FIREBALL",              root + "fireball/",            5, 1, 0.07f,  false);
        addAnim(data, "THROW",                 root + "throw/",               5, 1, 0.055f, false);

        // Hit reactions
        addAnim(data, "HITSTUN",               root + "hit_stun/",            4, 1, 0.075f, false);
        addAnim(data, "HIT_BLOW",              root + "hit_blow/",            4, 1, 0.11f,  false);
        addAnim(data, "CROUCH_STUN",           root + "hit_crouch_stun/",     3, 1, 0.115f, false);
        addAnim(data, "BACKWARD_THROW",        root + "backward_throw/",      5, 1, 0.11f,  false);

        // Defeat & victory
        addAnim(data, "DEATH",                 root + "KO/",                  5, 1, 0.09f,  false);
        addAnim(data, "DEATH_IDLE",            root + "KO_idle/",             2, 1, 0.25f,  true);
        addAnim(data, "VICTORY",               root + "victory_pose/",        6, 1, 0.12f,  true);
        addAnim(data, "CELEBRATION",           root + "celebration/",         3, 1, 0.15f,  true);

        return data;
    }

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

        // Minimal moveset — no sprite folders yet, so it renders as the
        // fallback colored quad (used by the HUD test).
        addAttack(data, AttackType::LIGHT_PUNCH, "LIGHT_PUNCH", 4, 3,  8,  50.0f, 100.0f, 150.0f, 120.0f,  80.0f, 150.0f, 0.30f, true);
        addAttack(data, AttackType::HARD_PUNCH,  "HARD_PUNCH",  8, 4, 14, 120.0f, 120.0f, 150.0f, 160.0f, 100.0f, 350.0f, 0.45f, true);

        return data;
    }
};

#endif // CHARACTER_ROSTER_H
