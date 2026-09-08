# PVP Fighting Game

A 2D versus-fighting game built from scratch in **C++17** on **OpenGL / freeglut** — no game engine, no frameworks. Rendering, animation, combat, UI and even the matrix math (`include/computer_graphics/core`) are hand-written.

*Great-Wall stage · dynamic zoom camera · three-layer parallax sky*

## Features

- **Full moveset** — standing / crouching / jumping light & hard punches and kicks, a command throw, and a fireball projectile — all with frame data (startup / active / recovery), hurtboxes and damage boxes
- **Blocking that works** — standing and crouching guard (with its own transition animation), chip damage, blockstun and a block-hit flash; throws are unblockable
- **Circular input buffer + combos** — attack presses are recorded in a fixed-size ring buffer and fire as soon as the fighter can act, enabling chains; hits during hitstun build a combo with per-hit damage scaling and an on-screen **N HIT COMBO** counter
- **Hit reactions & KO** — light / heavy / crouching hitstun clips, backward-throw reaction, KO fall into a lie-down loop, round and match victory poses
- **Interactive tutorial** before the first fight, clickable **Rematch / Quit** buttons at match end
- **Three-layer parallax background** — sunset sky, mountain range and playfield wall, each scrolling at its own rate
- **Dynamic camera** — zooms with fighter separation, tracks the action, clamps to stage bounds
- **HUD** — health bars, round timer and winner messages (stb_truetype font rendering)
- **Data-driven character roster** — characters are defined in one header: stats, frame data and animation folders

## Controls

| Action | Player 1 | Player 2 |
|---|---|---|
| Move | `A` / `D` | `←` / `→` |
| Jump | `W` | `↑` |
| Crouch (hold) | `S` | `↓` |
| Block (hold) | `G` | `7` |
| Light punch | `U` | `1` |
| Light kick | `I` | `2` |
| Hard punch | `J` | `3` |
| Hard kick | `K` | `4` |
| Fireball | `L` | `5` |
| Throw | `H` | `6` |
| Show hurtboxes (hold) | `0` | `0` |

Mouse: tutorial **Next / Skip** buttons and **Rematch / Quit** at match end (Space / Enter also advance the tutorial).

## Rules

Best of 3 rounds, 90 seconds each. Higher health wins a timed-out round — the winner strikes a pose, the loser eats the dirt.

## Building

```bash
make            # builds ./game
make run        # build + run
make test-hud   # standalone HUD preview binary
make clean
```

Requirements: Linux with X11, a C++17 compiler and OpenGL. freeglut (headers + runtime library) and the stb_image / stb_truetype headers are bundled in the repo, so there is nothing else to install.
Run from the repository root — asset paths are relative to it.

## Engine notes

- **Animation** — data-driven: `CharacterRoster` maps state/attack names to sprite folders (`assets/characters/ryu/<clip>/NN.png`); one-shot clips reset automatically on state change.
- **Combat** — frame-data attacks resolved per frame against hurtboxes; `HitImpact` carries damage, knockback, hitstun and blockability.
- **Input** — `InputBuffer` (`include/core/InputBuffer.h`) is a fixed-capacity circular buffer of timestamped inputs.
- **Graphics** — every quad/sprite is a unit quad placed by a `cg::Transform`; the camera view and both projections are built from the same math.

## computer_graphics/core — hand-rolled math

The assignment showcase: all matrix/vector math written from scratch, no GLM, no fixed-function matrix helpers.

| File | Concept |
|---|---|
| `Vec2.h` / `Vec3.h` | vectors |
| `Mat4.h` | column-major 4×4 matrix, multiply, point/direction transform |
| `Scale.h` | scale matrix (a negative factor mirrors — used for the sprite flip) |
| `Rotate.h` | rotations about X / Y / Z, in degrees |
| `Orthographic.h` | orthographic projection matrix (`glOrtho` equivalent, derivation included) |
| `Transform.h` | TRS model transform: `T · R · S` |

Conventions: column-major storage, column vectors (`v' = M·v`), right-handed rotations, degrees in the public API.
Every draw call follows `Projection · View · T·R·S · v` — the projections, the camera view and every model matrix come from this core; OpenGL only uploads them (`glLoadMatrixf` / `glMultMatrixf`).
See `include/computer_graphics/README.md` for the derivations and exact call sites.

## Project layout

```
├── assets/
│   ├── characters/ryu/     # 50+ animation clips (NN.png per folder)
│   ├── background/         # 3 parallax layers
│   ├── projectiles/        # fireball animation
│   ├── fonts/              # HUD font
│   └── audio/              # music + sfx (not wired into the engine yet)
├── include/
│   ├── computer_graphics/core/   # hand-rolled math (see above)
│   ├── core/               # game loop, input manager, input buffer
│   ├── entities/           # Character, Camera, Projectile, Background, roster
│   ├── graphics/           # Animation, Sprite, Texture (stb_image)
│   └── render/             # Renderer, HUD, Font (stb_truetype)
├── src/                    # implementations (mirrors include/)
├── tests/hud_test.cpp      # standalone HUD preview
└── Makefile
```
