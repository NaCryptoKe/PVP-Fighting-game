## Phase 1 — Cleanup

- [x] Delete `AABB.cpp` (empty, unused — `AABB.h` is header-only with inline functions)
- [x] Delete `StickFigureRenderer.h`
- [x] Delete `StickFigureRenderer.cpp`
- [x] Delete `Renderer.h`
- [x] Delete `Renderer.cpp`
- [x] Open your Makefile, confirm `find src -name '*.cpp'` is still being used (auto-discovers files) — if so, deletion needs no Makefile edit
- [x] Rebuild once after deletion, confirm it still compiles clean

## Phase 2 — Playtest current build (do this before touching crouch)

- [x] Run the game, confirm window opens titled "Stickman Kombat"
- [x] P1: press `a`, confirm player moves left
- [x] P1: press `d`, confirm player moves right
- [x] P1: press `w`, confirm player jumps and lands back on the ground line
- [x] P1: press `u`, confirm cyan hitbox briefly appears during LIGHT_PUNCH
- [x] P1: press `j`, confirm cyan hitbox briefly appears during HARD_PUNCH
- [x] P2: press Left arrow, confirm player moves left
- [x] P2: press Right arrow, confirm player moves right
- [x] P2: press Up arrow, confirm player jumps
- [x] P2: press `1`, confirm LIGHT_PUNCH plays
- [x] P2: press `2`, confirm HARD_PUNCH plays
- [x] Land a P1 punch on P2, confirm P2's health decreases (add a temporary `std::cout` of `getHealth()` if you can't see it yet — no UI exists)
- [x] Land a P1 punch on P2, confirm P2 gets knocked back
- [x] Confirm that knockback stops on its own within under a second (does not slide forever)
- [x] Hold `3` on P2, get punched by P1, confirm P2's health drop is smaller than an unblocked hit
- [x] Reduce P2's health to 0 (repeated hits), confirm console prints "Round over!" with score
- [x] Confirm both players reset to starting position/full health ~2 seconds after a KO
- [x] Repeat KOs until one player reaches 2 round wins, confirm console prints the correct match winner
- [x] After match-over message, press `r`, confirm score resets to 0-0 and a fresh round starts
- [x] Trigger match-over again, press `q`, confirm the program closes

If any single item above fails, stop and fix that one thing before moving to Phase 3 — don't stack crouch on top of a broken base.

## Phase 3 — Crouch

- [x] Decide crouch keybind: `s` for P1, `GLUT_KEY_DOWN` for P2
- [x] In `Game::update()`, inside P1's `canMove()` block, add: if `s` held → call a new `player1.setCrouching(true)`; else call `player1.setCrouching(false)`
- [x] In `Player.h`, declare `void setCrouching(bool wantCrouch);`
- [x] In `Player.cpp`, implement `setCrouching()`: if `wantCrouch` and `canAct()` and `isGrounded()`, set `currentState = PlayerState::CROUCH`; else if currently `CROUCH`, set back to `PlayerState::IDLE`
- [x] In `Player::render()`'s switch statement, add a `case PlayerState::CROUCH:` with its own `glColor3f(...)` (pick any distinct color — e.g. brown) so it's not left undefined
- [x] Repeat the same wiring for P2 using `GLUT_KEY_DOWN` via `isSpecialKeyDown`
- [x] Test: hold `s` as P1, confirm the quad changes to the new crouch color
- [x] Test: release `s`, confirm it returns to IDLE color
- [x] Test: confirm P1 can't jump while crouched (verified by guarding `jump()` against `PlayerState::CROUCH`)
- [x] Repeat both tests for P2 with `GLUT_KEY_DOWN`

## Phase 4 — Crouch Block

- [x] Decide: crouch-block = crouch key **and** block key held at the same time
- [x] In `Game::update()`, keep block calls as-is while ensuring crouch is set first and block second so the visual state resolves to `BLOCK` when both are held
- [x] Test: hold `s` and `k` together on P1, confirm state ends up as `BLOCK` (not `CROUCH`) after both calls
- [x] Get punched while holding both, confirm damage is still reduced (enabled by the existing `BLOCK` damage reduction path)
- [x] Repeat for P2 with `GLUT_KEY_DOWN` + `3`

## Phase 5 — Final check

- [x] Run through Phase 2's full checklist one more time end-to-end, now with crouch/crouch-block added, confirm nothing regressed
- [x] Commit and push to GitHub
