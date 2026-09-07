# Stickman Kombat — Follow-up Development Roadmap

## Phase 1 — HUD System Implementation

* [ ] Create `HUD.h` and `HUD.cpp` to manage UI overlay rendering


* [ ] Implement bitmap text rendering helper functions using standard GLUT font rendering


* [ ] Set up orthographic projection mode (`gluOrtho2D`) for 2D UI overlay rendering


* [ ] Draw Player 1 (top-left) and Player 2 (top-right) health bars with current HP values


* [ ] Display real-time round scores at top-center


* [ ] Display round winner banner when a player's health drops to 0


* [ ] Display match champion banner when a player reaches 2 round wins


* [ ] Render end-game interactive prompts: `[R] Rematch` and `[Q] Exit`

* [ ] Wire input handling in `Game` to handle `r` (reset scores/health) and `q` (exit program) during match over state



---

## Phase 2 — Sprite, Texture & Animation Integration

* [ ] Implement `TextureLoader` class using `stb_image.h` (or custom loader) to generate OpenGL texture IDs


* [ ] Enable OpenGL alpha blending (`glEnable(GL_BLEND)` / `glBlendFunc`) for sprite transparency


* [ ] Create `SpriteSheet` class to map frame indices to standard UV coordinates `(u1, v1, u2, v2)`

* [ ] Create `Animation` class to track frame sequences, playback speed, and frame updates


* [ ] Configure animation sets corresponding to player states (`IDLE`, `WALK`, `JUMP`, `CROUCH`, `BLOCK`, `LIGHT_PUNCH`, `HARD_PUNCH`, `HIT_STUN`)


* [ ] Replace colored quad drawing in `Player::render()` with textured quad UV mapping


* [ ] Implement horizontal sprite flipping by swapping UV coordinates based on facing direction


* [ ] Update frame advancement logic inside `Player::update(float deltaTime)`


---

## Phase 3 — Verification & Playtest

* [ ] Verify health bars update dynamically when hitboxes connect


* [ ] Confirm round scores increment and reset properly upon match completion


* [ ] Confirm `r` restarts a clean match and `q` exits cleanly


* [ ] Confirm character sprites display clean textures without visual artifacts


* [ ] Confirm animations switch state cleanly and flip correctly when passing the opponent