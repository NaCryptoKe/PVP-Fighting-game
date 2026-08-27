| System	        | Files	            | Handles                                           | Progress |
|-------------------|-------------------|---------------------------------------------------|----------|
|Texture loading	|Texture.hpp/.cpp	|PNG → GPU texture                                  |   ✅     |
|Geometry/collision	|HitBox.hpp/.cpp	|AABB, overlap test, local→world conversion         |   ✅     |
|Low-level drawing	|Renderer.hpp/.cpp	|quads, sprites, text-adjacent primitives           |   ✅     |
|Text rendering	    |Font.hpp/.cpp	    |TTF baking, string drawing                         |   ✅     |
|Sprite	            |Sprite.hpp/.cpp	|position/scale/flip + draw delegation              |   ✅     |
|Animation	        |Animation.hpp/.cpp	|frame sequencing/timing                            |   ✅     |
|Input	            |Input.hpp/.cpp	    |keyboard, arrow keys, pad, action-binding layer    |   ✅     |
|Character logic	|Character.hpp/.cpp	|state machine, physics, attacks, hurtboxes         |   ⏳     |
|Info to Player	    |HUD.hpp/.cpp       |Draw health bars, round timer, winner message      |   ✅     |
|Timer Logic	    |RoundTimer.hpp/.cpp|Track countdown, expose isExpired(), getSecond()   |   ✅     |
|Possible Game	    |GameState.hpp/.cpp |enum class for menu, fighting, round and others    |   ✅     |
|Audio      	    |AudioManager.hpp/cpp|Load/play SFX, music, and hit sounds              |   ❌     |
|Characters 	    |CharacterRoaster.hpp/.cpp|The character definition for the characters  |   ❌     |
|Camera     	    |Camera.hpp/.cpp    |Screen shake, hit-pause and others                 |   ✅     |
|Configuration	    |Config.hpp/.cpp    |Persistent keybindings, volume and others          |   ❌     |
|Main Menu  	    |MainMenu.hpp/.cpp  |Title screen ,character select UI et.c             |   ❌     |
|Orchestration	    |Game.hpp/.cpp	    |init, update loop, render loop, combat resolution  |   ❌     |
|Entry point	    |main.cpp	        |GLUT wiring                                        |   ⏳     |

## Order

Character -> GameState -> CharacterRoster -> AudioManager -> Config -> Game -> MainMenu -> main.cpp
