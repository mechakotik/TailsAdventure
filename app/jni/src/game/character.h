#ifndef TA_CHARACTER_H
#define TA_CHARACTER_H

#include "engine/pawn.h"
#include "engine/geometry.h"
#include "controller.h"
#include "engine/links.h"
#include "engine/tilemap.h"

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
    const double helitailAcc = 0.05;
    const double helitailTop = 1;
    const double maxHelitailTime = 200;

    TA_CommonController controller;
    TA_Point followPosition, velocity, climbPosition;
    TA_GameScreenLinks links;

    bool ground = false, helitail = false, wall = false, flip = false;
    bool jump = false, jumpReleased = false;
    bool climb = false, climbHigh = false, throwing = false;
    bool useHalfSolidTiles = false;
    double jumpTime = 0, climbTime = 0, helitailTime = 0;
    int currentTool = TA_TOOL_BOMB;

    void updateGround();
    void updateAir();
    void updateHelitail();

    void updateFollowPosition();
    void verticalMove();
    bool checkPawnCollision(TA_Polygon hitbox) override;
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
