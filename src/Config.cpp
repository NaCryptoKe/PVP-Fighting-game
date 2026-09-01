#include "src/Config.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>

// ============================================================
// Construction / Defaults
// ============================================================

Config::Config()
{
    applyDefaults();
}

void Config::applyDefaults()
{
    masterVolume = 0.8f;

    player1Bindings.clear();
    player2Bindings.clear();

    player1Bindings[InputAction::JUMP]        = KeyCode::W;
    player1Bindings[InputAction::CROUCH]      = KeyCode::S;
    player1Bindings[InputAction::RIGHT]       = KeyCode::D;
    player1Bindings[InputAction::LEFT]        = KeyCode::A;
    player1Bindings[InputAction::LIGHT_PUNCH] = KeyCode::K;
    player1Bindings[InputAction::LIGHT_KICK]  = KeyCode::L;
    player1Bindings[InputAction::HARD_PUNCH]  = KeyCode::J;
    player1Bindings[InputAction::HARD_KICK]   = KeyCode::I;
    player1Bindings[InputAction::BLOCK]       = KeyCode::Space;
    player1Bindings[InputAction::ENHANCE]     = KeyCode::U;

    player2Bindings[InputAction::JUMP]        = KeyCode::PadDPADUp;
    player2Bindings[InputAction::CROUCH]      = KeyCode::PadDPADDown;
    player2Bindings[InputAction::RIGHT]       = KeyCode::PadDPADRight;
    player2Bindings[InputAction::LEFT]        = KeyCode::PadDPADLeft;
    player2Bindings[InputAction::LIGHT_PUNCH] = KeyCode::PadSquare;
    player2Bindings[InputAction::LIGHT_KICK]  = KeyCode::PadCross;
    player2Bindings[InputAction::HARD_PUNCH]  = KeyCode::PadTriangle;
    player2Bindings[InputAction::HARD_KICK]   = KeyCode::PadCircle;
    player2Bindings[InputAction::BLOCK]       = KeyCode::PadL1;
    player2Bindings[InputAction::ENHANCE]     = KeyCode::PadR1;
}

// ============================================================
// Volume
// ============================================================

float Config::getMasterVolume() const
{
    return masterVolume;
}

void Config::setMasterVolume(float volume)
{
    masterVolume = std::clamp(volume, 0.0f, 1.0f);
}

// ============================================================
// Keybindings
// ============================================================

std::unordered_map<InputAction, KeyCode>& Config::bindingsFor(bool isPlayer1)
{
    return isPlayer1 ? player1Bindings : player2Bindings;
}

const std::unordered_map<InputAction, KeyCode>& Config::bindingsFor(bool isPlayer1) const
{
    return isPlayer1 ? player1Bindings : player2Bindings;
}

KeyCode Config::getBinding(bool isPlayer1, InputAction action) const
{
    const auto& bindings = bindingsFor(isPlayer1);
    auto it = bindings.find(action);

    // Falls back to player 1's keyboard JUMP key if this action was
    // never bound for this player - keeps callers from reading an
    // uninitialized/garbage KeyCode if a new InputAction is added
    // to Input.hpp without a matching entry here.
    return (it != bindings.end()) ? it->second : KeyCode::W;
}

void Config::setBinding(bool isPlayer1, InputAction action, KeyCode key)
{
    bindingsFor(isPlayer1)[action] = key;
}

void Config::applyBindings(InputManager& input, bool isPlayer1) const
{
    const auto& bindings = bindingsFor(isPlayer1);

    for (const auto& [action, key] : bindings)
    {
        input.setBinding(action, key);
    }
}

// ============================================================
// Enum <-> String
// ============================================================
// Only used by load()/save(), so the on-disk file is readable and
// hand-editable (e.g. "P1.JUMP=W") instead of raw enum integers
// that would break silently if enum ordering ever changes.

std::string Config::actionToString(InputAction action)
{
    switch (action)
    {
        case InputAction::JUMP:         return "JUMP";
        case InputAction::CROUCH:       return "CROUCH";
        case InputAction::LEFT:         return "LEFT";
        case InputAction::RIGHT:        return "RIGHT";
        case InputAction::LIGHT_PUNCH:  return "LIGHT_PUNCH";
        case InputAction::LIGHT_KICK:   return "LIGHT_KICK";
        case InputAction::HARD_PUNCH:   return "HARD_PUNCH";
        case InputAction::HARD_KICK:    return "HARD_KICK";
        case InputAction::BLOCK:        return "BLOCK";
        case InputAction::ENHANCE:      return "ENHANCE";
    }

    return "";
}

bool Config::stringToAction(const std::string& str, InputAction& out)
{
    static const std::unordered_map<std::string, InputAction> table = {
        { "JUMP",         InputAction::JUMP },
        { "CROUCH",       InputAction::CROUCH },
        { "LEFT",         InputAction::LEFT },
        { "RIGHT",        InputAction::RIGHT },
        { "LIGHT_PUNCH",  InputAction::LIGHT_PUNCH },
        { "LIGHT_KICK",   InputAction::LIGHT_KICK },
        { "HARD_PUNCH",   InputAction::HARD_PUNCH },
        { "HARD_KICK",    InputAction::HARD_KICK },
        { "BLOCK",        InputAction::BLOCK },
        { "ENHANCE",      InputAction::ENHANCE },
    };

    auto it = table.find(str);

    if (it == table.end())
    {
        return false;
    }

    out = it->second;
    return true;
}

std::string Config::keyCodeToString(KeyCode key)
{
    switch (key)
    {
        case KeyCode::W: return "W";
        case KeyCode::S: return "S";
        case KeyCode::D: return "D";
        case KeyCode::A: return "A";
        case KeyCode::K: return "K";
        case KeyCode::L: return "L";
        case KeyCode::J: return "J";
        case KeyCode::I: return "I";
        case KeyCode::Space: return "Space";
        case KeyCode::U: return "U";

        case KeyCode::PadCross:    return "PadCross";
        case KeyCode::PadCircle:   return "PadCircle";
        case KeyCode::PadSquare:   return "PadSquare";
        case KeyCode::PadTriangle: return "PadTriangle";

        case KeyCode::PadL1: return "PadL1";
        case KeyCode::PadR1: return "PadR1";
        case KeyCode::PadL2: return "PadL2";
        case KeyCode::PadR2: return "PadR2";
        case KeyCode::PadL3: return "PadL3";
        case KeyCode::PadR3: return "PadR3";

        case KeyCode::PadStickUp:    return "PadStickUp";
        case KeyCode::PadStickDown:  return "PadStickDown";
        case KeyCode::PadStickLeft:  return "PadStickLeft";
        case KeyCode::PadStickRight: return "PadStickRight";

        case KeyCode::PadDPADUp:    return "PadDPADUp";
        case KeyCode::PadDPADDown:  return "PadDPADDown";
        case KeyCode::PadDPADLeft:  return "PadDPADLeft";
        case KeyCode::PadDPADRight: return "PadDPADRight";

        case KeyCode::COUNT: return "";
    }

    return "";
}

bool Config::stringToKeyCode(const std::string& str, KeyCode& out)
{
    static const std::unordered_map<std::string, KeyCode> table = {
        { "W", KeyCode::W }, { "S", KeyCode::S },
        { "D", KeyCode::D }, { "A", KeyCode::A },
        { "K", KeyCode::K }, { "L", KeyCode::L },
        { "J", KeyCode::J }, { "I", KeyCode::I },
        { "Space", KeyCode::Space }, { "U", KeyCode::U },

        { "PadCross", KeyCode::PadCross },
        { "PadCircle", KeyCode::PadCircle },
        { "PadSquare", KeyCode::PadSquare },
        { "PadTriangle", KeyCode::PadTriangle },

        { "PadL1", KeyCode::PadL1 }, { "PadR1", KeyCode::PadR1 },
        { "PadL2", KeyCode::PadL2 }, { "PadR2", KeyCode::PadR2 },
        { "PadL3", KeyCode::PadL3 }, { "PadR3", KeyCode::PadR3 },

        { "PadStickUp", KeyCode::PadStickUp },
        { "PadStickDown", KeyCode::PadStickDown },
        { "PadStickLeft", KeyCode::PadStickLeft },
        { "PadStickRight", KeyCode::PadStickRight },

        { "PadDPADUp", KeyCode::PadDPADUp },
        { "PadDPADDown", KeyCode::PadDPADDown },
        { "PadDPADLeft", KeyCode::PadDPADLeft },
        { "PadDPADRight", KeyCode::PadDPADRight },
    };

    auto it = table.find(str);

    if (it == table.end())
    {
        return false;
    }

    out = it->second;
    return true;
}

// ============================================================
// File I/O
// ============================================================
//
// File format is deliberately simple - one "key=value" pair per
// line, no sections, no quoting:
//
//   master_volume=0.80
//   P1.JUMP=W
//   P2.JUMP=PadStickUp
//   ...
//
// Blank lines and lines starting with '#' are ignored.

namespace
{
    std::string trim(const std::string& s)
    {
        const char* whitespace = " \t\r\n";

        size_t start = s.find_first_not_of(whitespace);

        if (start == std::string::npos)
        {
            return "";
        }

        size_t end = s.find_last_not_of(whitespace);

        return s.substr(start, end - start + 1);
    }
}

bool Config::load(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        applyDefaults();
        return false;
    }

    // Start from defaults so any key missing/unparseable in the
    // file still ends up with a sane value rather than whatever
    // was left over from the Config's previous state.
    applyDefaults();

    int entriesLoaded = 0;
    std::string line;

    while (std::getline(file, line))
    {
        line = trim(line);

        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        size_t eq = line.find('=');

        if (eq == std::string::npos)
        {
            continue; // malformed line - skip rather than fail the whole load
        }

        std::string key   = trim(line.substr(0, eq));
        std::string value = trim(line.substr(eq + 1));

        if (key == "master_volume")
        {
            try
            {
                setMasterVolume(std::stof(value));
                ++entriesLoaded;
            }
            catch (const std::exception&)
            {
                // Leave the default volume in place.
            }

            continue;
        }

        bool isPlayer1;

        if (key.rfind("P1.", 0) == 0)
        {
            isPlayer1 = true;
        }
        else if (key.rfind("P2.", 0) == 0)
        {
            isPlayer1 = false;
        }
        else
        {
            continue; // unrecognized key - skip
        }

        InputAction action;
        KeyCode keyCode;

        if (stringToAction(key.substr(3), action) &&
            stringToKeyCode(value, keyCode))
        {
            setBinding(isPlayer1, action, keyCode);
            ++entriesLoaded;
        }
    }

    return entriesLoaded > 0;
}

bool Config::save(const std::string& path) const
{
    std::ofstream file(path, std::ios::trunc);

    if (!file.is_open())
    {
        return false;
    }

    file << "# Auto-generated config file. Safe to hand-edit.\n";
    file << "master_volume=" << masterVolume << "\n";

    for (const auto& [action, key] : player1Bindings)
    {
        file << "P1." << actionToString(action) << "=" << keyCodeToString(key) << "\n";
    }

    for (const auto& [action, key] : player2Bindings)
    {
        file << "P2." << actionToString(action) << "=" << keyCodeToString(key) << "\n";
    }

    return true;
}