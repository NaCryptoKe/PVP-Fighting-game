#include "src/Config.hpp"
#include <cassert>
#include <cstdio>

int main()
{
    // Defaults are sane
    Config c;
    assert(c.getMasterVolume() == 0.8f);
    assert(c.getBinding(true, InputAction::JUMP) == KeyCode::W);
    assert(c.getBinding(false, InputAction::JUMP) == KeyCode::PadStickUp);

    // Mutate + persist
    c.setMasterVolume(0.35f);
    c.setBinding(true, InputAction::JUMP, KeyCode::I);
    c.setBinding(false, InputAction::BLOCK, KeyCode::PadR2);
    bool saved = c.save("/tmp/config_test.ini");
    assert(saved);

    // Fresh Config loading from that file should see the changes
    Config c2;
    bool loaded = c2.load("/tmp/config_test.ini");
    assert(loaded);
    assert(c2.getMasterVolume() > 0.34f && c2.getMasterVolume() < 0.36f);
    assert(c2.getBinding(true, InputAction::JUMP) == KeyCode::I);
    assert(c2.getBinding(false, InputAction::BLOCK) == KeyCode::PadR2);
    // Untouched binding should still be the default
    assert(c2.getBinding(true, InputAction::CROUCH) == KeyCode::S);

    // Loading a nonexistent file falls back to defaults and reports false
    Config c3;
    bool loadedMissing = c3.load("/tmp/definitely_does_not_exist.ini");
    assert(!loadedMissing);
    assert(c3.getMasterVolume() == 0.8f);

    // applyBindings actually pushes into a real InputManager
    InputManager input;
    c2.applyBindings(input, true);
    input.applyPadDefaults(); // shouldn't matter, different actions untouched by keyboard side effects
    // No direct getter on InputManager for bindings, so just confirm
    // this doesn't crash and setBinding was reachable.

    printf("Expected: all Config assertions pass / Actual: reached end without assertion failure\n");
    return 0;
}