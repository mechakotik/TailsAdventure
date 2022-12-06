#ifndef TA_CHARACTER_H
#define TA_CHARACTER_H

#include "pawn.h"
#include "geometry.h"
#include "character_controller.h"
#include "links.h"
#include "tilemap.h"

enum TA_CharacterTool {
    TA_TOOL_BOMB
};

class TA_Character : public TA_Pawn {
private:
    const double jmp = -2.5;
    const double grv = 0.125;
    const double acc = 0.25;
    const double topX = 1;
    const double topY = 5;
    const double maxJumpTime = 10;

    TA_CharacterController controller;
    TA_Point followPosition, velocity, climbPosition;
    TA_GameScreenLinks links;

    bool ground = false, wall = false, jump = false, jumpReleased = false, climb = false, climbHigh = false;
    bool useHalfSolidTiles = false;
    double jumpTime = 0, climbTime = 0;
    int currentTool = TA_TOOL_BOMB;

    void updateFollowPosition();
    void verticalMove();
    bool checkCollision(TA_Polygon hitbox) override;
    void updateCollisions();
    void updateAnimation();
    void updateClimb();
    void updateTool();

public:
    void load(TA_GameScreenLinks newLinks);
    void update();
    void drawControls() {controller.draw();}
    bool isOnGround() {return ground;}

    int getRingsCount() {return 12;}
    int getCurrentItem() {return 0;}
};

#endif // TA_CHARACTER_H
