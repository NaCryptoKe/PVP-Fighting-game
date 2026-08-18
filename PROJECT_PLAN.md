# Fighting Game - GLUT/GLEW Project Plan

A scoped, school-project-sized **2D fighting game** inspired by Mortal Kombat / Street Fighter, using OpenGL, GLUT, and GLEW. Local 2-player only.

---

## Scope (Locked - Don't Expand)

* [ ] Local 2-player only (no online/netcode)
* [ ] 2 characters maximum
* [ ] Sprite-based 2D characters
* [ ] 1 stage with a simple 2D background
* [ ] Button-triggered specials only (no motion-input parsing like quarter-circles)
* [ ] Simple hit reactions (flinch/stagger), no combo-cancel system
* [ ] Single horizontal fighting plane
* [ ] No 3D models, skeletal animation, or 3D physics

---

## Phase 1: Core Setup (Week 1, ~8–12 hrs)

## Phase 1: Core Setup (Week 1, ~8–12 hrs)

- [x] Create GLUT window + OpenGL context
- [x] Implement fixed-timestep game loop via `glutTimerFunc`
- [x] Set up 2D orthographic projection
- [x] Define screen/world coordinate system
- [x] Implement basic 2D rendering
- [x] Render a textured quad
- [x] Load and render a single test sprite
- [x] Implement basic sprite positioning and scaling

## Phase 2: Sprite System & Animation (Week 2, ~18–22 hrs)

* [x] Design `Sprite` representation
* [x] Load PNG/JPG textures
* [x] Implement texture management
* [x] Implement sprite rendering using textured quads
* [x] Implement sprite sheets / texture atlases
* [x] Implement frame-based animation
* [x] Implement animation playback and timing
* [x] Create basic animations:

  * [ ] Idle
  * [ ] Walk
  * [x] Jump
  * [ ] Crouch
  * [ ] Light attack
  * [ ] Heavy attack
  * [ ] Block
  * [ ] Hit reaction
* [x] Render both characters simultaneously

## Phase 3: Input & Movement (Week 3, ~10–15 hrs)

* [x] Track key state manually via `glutKeyboardFunc` / `glutKeyboardUpFunc`
* [x] Use a bool array or equivalent input-state structure
* [x] Implement walk left/right
* [ ] Implement jump
* [ ] Implement crouch
* [ ] Implement facing direction
* [ ] Automatically face the opponent
* [ ] Support two separate control schemes
* [ ] Add input buffering
* [ ] Prevent players from leaving the stage boundaries

## Phase 4: Combat Core (Week 4, ~17 hrs)

* [x] Design character state machine:

  * [ ] Idle
  * [ ] Walk
  * [ ] Crouch
  * [ ] Jump
  * [ ] Attack
  * [ ] Block
  * [ ] Hit-stun
  * [ ] Block-stun
* [ ] Define character hurtboxes
* [ ] Define attack hitboxes
* [ ] Associate hitboxes with animation frames
* [ ] Implement hitbox vs. hurtbox collision
* [ ] Implement damage system
* [ ] Implement health values
* [ ] Implement blocking
* [ ] Implement reduced block damage
* [ ] Implement hit reactions
* [ ] Implement knockback
* [ ] Prevent invalid state transitions

## Phase 5: Special Moves (Week 5, ~8 hrs)

* [ ] Add 1–2 button-triggered special moves per character
* [ ] Create special-move animations
* [ ] Add special-move hitboxes
* [ ] Add simple cooldown or resource cost if desired
* [ ] Wire specials into the state machine
* [ ] Prevent state leaks / soft-locks

## Phase 6: UI/HUD (Week 6, ~8 hrs)

* [ ] Health bar for Player 1
* [ ] Health bar for Player 2
* [ ] Round timer
* [ ] Player names
* [ ] Round counter
* [ ] Win/lose screen
* [ ] Round reset
* [ ] Match reset
* [ ] Optional: main menu
* [ ] Optional: character selection screen

## Phase 7: Stage & Visual Polish (Week 7, ~10–15 hrs)

* [ ] Create/import simple 2D stage background
* [ ] Add floor/stage boundaries
* [ ] Add character shadows
* [ ] Add hit sound effects
* [ ] Add attack sound effects
* [ ] Add basic hit VFX
* [ ] Add sprite flash on hit
* [ ] Add hitstop
* [ ] Add screen shake
* [ ] Add simple round-start / round-end effects

## Phase 8: Testing & Bug Fixing (Week 8, ~10–15 hrs)

* [ ] Playtest movement
* [ ] Playtest attack timing
* [ ] Balance damage values
* [ ] Balance movement speed
* [ ] Balance attack startup/active/recovery frames
* [ ] Test simultaneous attacks
* [ ] Test attacks at stage boundaries
* [ ] Test jumping/crouching interactions
* [ ] Test blocking edge cases
* [ ] Test state-machine transitions
* [ ] Test animation synchronization
* [ ] Fix input buffering edge cases
* [ ] Fix collision edge cases
* [ ] Buffer time for unexpected bugs before demo/deadline

---

## Stretch Goals (Only If Ahead of Schedule)

* [ ] Combo system (chain 2–3 attacks together)
* [ ] Second stage
* [ ] Gamepad support
* [ ] Simple AI opponent for single-player practice mode
* [ ] Character selection screen
* [ ] Additional character
* [ ] Projectile-based special attack
* [ ] Simple particle system
* [ ] Replay last round locally

---

## Notes

* GLUT's `glutTimerFunc` recursive call is the standard game-loop pattern here — no need to fight it.
* Use an **orthographic projection** rather than a perspective camera.
* Keep gameplay coordinates separate from screen/pixel coordinates where practical.
* Characters should be represented as sprites rather than 3D models.
* Use placeholder rectangles/sprites while implementing gameplay; visual assets come later.
* Prioritize combat *feel* over visual fidelity early.
* Keep a running log of frame data for every move:

  * Startup frames
  * Active frames
  * Recovery frames
  * Hit-stun
  * Block-stun
  * Damage
  * Knockback
* Keep hitboxes independent from the visible sprite. The sprite is presentation; the hitbox is gameplay.
* Keep rendering, input, combat, animation, and game-state logic separated so the project does not become one enormous `main.cpp`.

