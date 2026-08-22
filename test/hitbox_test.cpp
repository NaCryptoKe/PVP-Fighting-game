#include <iostream>
#include "src/HitBox.hpp"

void printAABB(const AABB& box)
{
    std::cout << "Left   : " << box.left << '\n';
    std::cout << "Right  : " << box.right << '\n';
    std::cout << "Bottom : " << box.bottom << '\n';
    std::cout << "Top    : " << box.top << '\n';
    std::cout << '\n';
}

int main()
{
    // ------------------------------------------------
    // Test 1: Character facing RIGHT
    // ------------------------------------------------
    HitBox body;
    body.offsetX = -25.0f;
    body.offsetY = 0.0f;
    body.width   = 50.0f;
    body.height  = 100.0f;

    float footX = 500.0f;
    float footY = 100.0f;

    AABB rightBox = body.toWorld(footX, footY, true);

    std::cout << "Facing Right:\n";
    printAABB(rightBox);

    // Expected:
    // left   = 475
    // right  = 525
    // bottom = 100
    // top    = 200

    // ------------------------------------------------
    // Test 2: Character facing LEFT
    // ------------------------------------------------
    AABB leftBox = body.toWorld(footX, footY, false);

    std::cout << "Facing Left:\n";
    printAABB(leftBox);

    // Since:
    // worldOffsetX = -offsetX - width
    //              = -(-25) - 50
    //              = 25 - 50
    //              = -25
    //
    // This particular box is symmetric,
    // so left/right are the same.

    // ------------------------------------------------
    // Test 3: Overlapping boxes
    // ------------------------------------------------
    AABB enemy;
    enemy.left   = 500;
    enemy.right  = 550;
    enemy.bottom = 120;
    enemy.top    = 220;

    bool hit = aabbOverlap(rightBox, enemy);

    std::cout << "Overlap test: ";
    std::cout << (hit ? "YES\n" : "NO\n");

    // ------------------------------------------------
    // Test 4: Non-overlapping boxes
    // ------------------------------------------------
    AABB farEnemy;
    farEnemy.left   = 1000;
    farEnemy.right  = 1050;
    farEnemy.bottom = 100;
    farEnemy.top    = 200;

    bool hit2 = aabbOverlap(rightBox, farEnemy);

    std::cout << "Far overlap test: ";
    std::cout << (hit2 ? "YES\n" : "NO\n");

    return 0;
}