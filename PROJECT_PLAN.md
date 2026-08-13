# Fighting Game - GLUT/GLEW Project Plan

A scoped, school-project-sized fighting game (MK/Street Fighter inspired) using OpenGL, GLUT, and GLEW. 3D characters on a 2D fighting plane, local 2-player only.

---

## Scope (Locked - Don't Expand)
- [ ] Local 2-player only (no online/netcode)
- [ ] 2 characters max, sharing one rig/animation pipeline if possible
- [ ] 1 stage, flat plane + simple backdrop
- [ ] Button-triggered specials only (no motion-input parsing like quarter-circles)
- [ ] Simple hit reactions (flinch/stagger), no combo-cancel system

---

## Phase 1: Core Setup (Weeks 1, ~8-12 hrs)
- [ ] Create GLUT window + OpenGL context
- [ ] Set up GLEW and confirm extensions load correctly
- [ ] Implement fixed-timestep game loop via `glutTimerFunc`
- [ ] Set up camera for 2D-plane-in-3D view (fixed side-view, characters move on one axis)
- [ ] Load and render a single static 3D test model

## Phase 2: Character Rendering & Animation (Weeks 2, ~22 hrs)
- [ ] Import/load 3D character models (e.g. Mixamo rigged models)
- [ ] Decide animation approach: jointed rigid parts vs. keyframe pose blending vs. full GPU skinning
- [ ] Implement basic animation playback (idle, walk, jump, crouch)
- [ ] Get both characters rendering simultaneously on the fighting plane

## Phase 3: Input & Movement (Weeks 3, ~10–15 hrs)
- [ ] Track key state manually via `glutKeyboardFunc` / `glutKeyboardUpFunc` (bool array, not raw events)
- [ ] Implement walk left/right, jump, crouch
- [ ] Implement facing direction (auto-flip to face opponent)
- [ ] Support two separate control schemes (Player 1 / Player 2 keys)
- [ ] Add input buffering (small window so inputs feel responsive, not dropped)

## Phase 4: Combat Core (Weeks 4, ~17 hrs)
- [ ] Design character state machine (idle, walk, crouch, jump, attack, block, hit-stun, block-stun)
- [ ] Implement hitbox and hurtbox volumes per animation frame
- [ ] Implement hit detection (hitbox vs. hurtbox overlap)
- [ ] Implement damage system and health values
- [ ] Implement blocking (reduced damage when holding block)
- [ ] Implement basic hit reactions (flinch, stagger, knockback)

## Phase 5: Special Moves (Weeks 5, ~8 hrs)
- [ ] Add 1–2 button-triggered special moves per character
- [ ] Add simple cooldown or resource cost if desired (optional)
- [ ] Wire specials into the state machine cleanly (no state leaks/soft-locks)

## Phase 6: UI/HUD (Weeks 6, ~8 hrs)
- [ ] Health bars for both players
- [ ] Round timer
- [ ] Win/lose screen + round reset
- [ ] Optional: main menu / character select screen

## Phase 7: Polish (Weeks 7, ~10–15 hrs)
- [ ] Stage backdrop art
- [ ] Hit sound effects
- [ ] Basic hit VFX (flash, particle burst, or simple sprite effect)
- [ ] Hitstop / screen shake on hit (optional, adds "juice")

## Phase 8: Testing & Bug Fixing (Weeks 8, ~10–15 hrs)
- [ ] Playtest for balance (damage values, move speed, frame timing)
- [ ] Fix collision edge cases (both players attacking simultaneously, corner cases at stage edges)
- [ ] Fix state machine edge cases (input during hit-stun, double inputs, etc.)
- [ ] Buffer time for unexpected bugs before demo/deadline

---

## Stretch Goals (Only If Ahead of Schedule)
- [ ] Combo system (chain 2–3 attacks together)
- [ ] Second stage
- [ ] Gamepad support
- [ ] Simple AI opponent for single-player practice mode

---

## Notes
- GLUT's `glutTimerFunc` recursive call is the standard game-loop pattern here - no need to fight it.
- Prioritize combat *feel* over visual fidelity early - placeholder capsule/box characters are fine while tuning hitboxes.
- Keep a running log of frame data (startup/active/recovery frames per move) - this makes balancing much easier later.
