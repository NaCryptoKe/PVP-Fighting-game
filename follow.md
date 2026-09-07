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

- [ ] Decide crouch keybind: `s` for P1, `GLUT_KEY_DOWN` for P2
- [ ] In `Game::update()`, inside P1's `canMove()` block, add: if `s` held → call a new `player1.setCrouching(true)`; else call `player1.setCrouching(false)`
- [ ] In `Player.h`, declare `void setCrouching(bool wantCrouch);`
- [ ] In `Player.cpp`, implement `setCrouching()`: if `wantCrouch` and `canAct()` and `isGrounded()`, set `currentState = PlayerState::CROUCH`; else if currently `CROUCH`, set back to `PlayerState::IDLE`
- [ ] In `Player::render()`'s switch statement, add a `case PlayerState::CROUCH:` with its own `glColor3f(...)` (pick any distinct color — e.g. brown) so it's not left undefined
- [ ] Repeat the same wiring for P2 using `GLUT_KEY_DOWN` via `isSpecialKeyDown`
- [ ] Test: hold `s` as P1, confirm the quad changes to the new crouch color
- [ ] Test: release `s`, confirm it returns to IDLE color
- [ ] Test: confirm P1 can't jump while crouched (should already be true since `jump()` doesn't check crouch — verify, don't assume)
- [ ] Repeat both tests for P2 with `GLUT_KEY_DOWN`

## Phase 4 — Crouch Block

- [ ] Decide: crouch-block = crouch key **and** block key held at the same time
- [ ] In `Game::update()`, change P1's block line to: `player1.setBlocking(input.isKeyDown('k') || (input.isKeyDown('s') && input.isKeyDown('k')));` — actually simpler: keep `setBlocking` call as-is, since holding both `s` and `k` will just call both `setCrouching(true)` and `setBlocking(true)` — decide which one should "win" the visual state (recommend: block visually wins, so call `setBlocking()` *after* `setCrouching()` in your update order, since whichever runs last sets `currentState` last)
- [ ] Test: hold `s` and `k` together on P1, confirm state ends up as `BLOCK` (not `CROUCH`) after both calls
- [ ] Get punched while holding both, confirm damage is still reduced (this uses the same `BLOCK` check in `takeDamage()` — no new code needed if the state correctly resolves to `BLOCK`)
- [ ] Repeat for P2 with `GLUT_KEY_DOWN` + `3`

## Phase 5 — Final check

- [ ] Run through Phase 2's full checklist one more time end-to-end, now with crouch/crouch-block added, confirm nothing regressed
- [ ] Commit and push to GitHub