# computer_graphics/core — hand-rolled math core

Custom implementation of the classic computer-graphics primitives for this
assignment. No GLM, no fixed-function matrix helpers — every matrix is built
and multiplied explicitly so the math is fully visible.

## Files

| File            | Concept                                            |
|-----------------|----------------------------------------------------|
| `core/Vec2.h`   | 2D vector                                          |
| `core/Vec3.h`   | 3D vector                                          |
| `core/Mat4.h`   | column-major 4x4 matrix, multiply, point/direction transform |
| `core/Scale.h`  | scale matrix (about the local origin; negative = mirror) |
| `core/Rotate.h` | rotation about Z (2D) and X/Y/Z (3D), in degrees   |
| `core/Orthographic.h` | orthographic projection matrix (`glOrtho` equivalent) |
| `core/Transform.h` | TRS model transform: `T * R * S`                |

## Conventions

- Column-major storage (OpenGL layout), translation in `m[12..14]`
- Column vectors: `v' = M * v`; `(A * B)` applies `B` first
- Degrees in the public API, radians inside the trig
- Right-handed rotations (counter-clockwise about the positive axis)

## Where the game uses it (the showcase)

- **Orthographic** — `Game::reshape` and the HUD pass in `Game::render`
  replace `glOrtho` with `cg::Orthographic(...).toMatrix()` via `glLoadMatrixf`.
- **Transform + Scale** — every sprite and quad goes through
  `Renderer::drawTexturedQuad` / `drawColoredQuad`, which build a `cg::Transform`
  and upload it with `glMultMatrixf`. The sprite mirror-flip is a negative
  X scale; `Sprite` stores a `cg::Transform` directly.
- **Mat4 composition** — `Camera::apply` builds its view matrix explicitly:
  `T(screenCenter) * S(zoom) * T(-focus)`.
- **Rotate** — part of every `Transform::toMatrix()`, and visibly spinning
  the diamond accents on the tutorial screen.

Full vertex path per draw call:
`Projection * View * T * R * S * v` — the projection/view come from this core
too, only the matrix *upload* uses OpenGL (`glLoadMatrixf` / `glMultMatrixf`).
