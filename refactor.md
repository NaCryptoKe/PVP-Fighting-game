> [!NOTE]
> Future fixes intended to be done on the codebase after finishing version 1.0

# Refactor Notes

## Character.cpp

### Animation Loading

Currently there is separate function for each CharacterState:

- loadIdleAnimation()
- loadWalkAnimation()
- loadJumpAnimation()
- loadCrouchAnimation()
- loadBlockAnimation()
- loadHitStunAnimation()
- loadKOAnimation()   

This is unnecesarilly repetitive.

Refactor it to something like:

```c
bool loadAnimation(
    CharacterState state,
    const char* folder,
    int frames,
    float duration,
    bool looping
);
```

The function should:
1. Create animation
2. Load it
3. Store it in the animation map using CharacterState
4. Reture success/failure

Then the individual animation loading methods can be removed.


