#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "src/Input.hpp"

#include <string>
#include <unordered_map>

// ============================================================
// Config
// ============================================================
//
// Persists user-facing settings across sessions: master volume
// and per-player keybindings. Backed by a plain "key=value" text
// file - no external parsing library, matching the rest of the
// project (miniaudio/stb are the only third-party deps).
//
// This is a local 2-player game, so bindings are stored as two
// independent sets (player 1 / player 2) rather than one shared
// set - each player can be remapped without touching the other's
// keys, whether they're both on keyboards, one on a pad, etc.
//
// Typical use on startup:
//
//   Config config;
//   if (!config.load("config.ini"))
//   {
//       // No file yet, or it didn't parse - config already holds
//       // built-in defaults (see applyDefaults()). Write them out
//       // so the file exists next time.
//       config.save("config.ini");
//   }
//
//   config.applyBindings(p1Input, true);
//   config.applyBindings(p2Input, false);
//   audio.set_master_volume(config.getMasterVolume());

class Config
{
public:
    Config();

    // Resets every setting to its built-in default. Called by the
    // constructor, and by load() if the file can't be read/parsed,
    // so a Config is always in a valid, usable state.
    void applyDefaults();

    // Reads settings from a "key=value" text file at `path`.
    // Returns false if the file doesn't exist or no valid entries
    // were found - in that case applyDefaults() has already been
    // called, so the Config is still safe to use as-is.
    // Unrecognized lines/keys are skipped rather than treated as
    // a parse failure, so a hand-edited or partially-old file
    // still loads whatever it does understand.
    bool load(const std::string& path);

    // Writes current settings to `path`, overwriting it. Returns
    // false if the file couldn't be opened for writing.
    bool save(const std::string& path) const;

    // ------------------------------------------------------------
    // Volume
    // ------------------------------------------------------------

    float getMasterVolume() const;
    void setMasterVolume(float volume); // clamped to [0.0f, 1.0f]

    // ------------------------------------------------------------
    // Keybindings
    // ------------------------------------------------------------

    KeyCode getBinding(bool isPlayer1, InputAction action) const;
    void setBinding(bool isPlayer1, InputAction action, KeyCode key);

    // Pushes this Config's bindings for one player onto an
    // InputManager, overriding whatever applyKeyboardDefaults() /
    // applyPadDefaults() set up.
    void applyBindings(InputManager& input, bool isPlayer1) const;

private:
    float masterVolume;

    std::unordered_map<InputAction, KeyCode> player1Bindings;
    std::unordered_map<InputAction, KeyCode> player2Bindings;

    std::unordered_map<InputAction, KeyCode>& bindingsFor(bool isPlayer1);
    const std::unordered_map<InputAction, KeyCode>& bindingsFor(bool isPlayer1) const;

    // Enum <-> string tables, used only by load()/save() so the
    // file stays human-readable/hand-editable (e.g. "P1.JUMP=W").
    static std::string actionToString(InputAction action);
    static bool stringToAction(const std::string& str, InputAction& out);

    static std::string keyCodeToString(KeyCode key);
    static bool stringToKeyCode(const std::string& str, KeyCode& out);
};

#endif // CONFIG_HPP