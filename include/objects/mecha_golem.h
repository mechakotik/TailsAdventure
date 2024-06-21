#ifndef TA_MECHA_GOLEM_H
#define TA_MECHA_GOLEM_H

#include "object_set.h"

class TA_MechaGolem : public TA_Object {
public:
    using TA_Object::TA_Object;
    void load();
    bool update() override;
    void draw() override;

private:
    const double invincibleTime = 30;
    const double damageFlashTime = 5;
    const double waitTime = 30;
    const double goTime = 30;
    const double goBorder = 32;
    const double goDistance = 16;
    const double stepHeight = 8;
    const double armMoveMaxDistance = 64;
    const double armMoveTime = 15;

    enum State {
        STATE_IDLE,
        STATE_WAIT,
        STATE_GO_LEFT,
        STATE_GO_RIGHT,
        STATE_ARM_MOVE,
        STATE_ARM_MOVE_BACK,
        STATE_ARM_CIRCLE,
        STATE_ARM_BITE
    };

    enum Hitbox {
        HITBOX_WALL_LEFT,
        HITBOX_WALL_RIGHT,
        HITBOX_BODY,
        HITBOX_WEAK,
        HITBOX_ARM,
        HITBOX_MAX
    };

    State state = STATE_IDLE;

    void updateIdle();
    void updateWait();
    void initGo();
    void updateGo(int direction);
    void initArmMove();
    void updateArmMove();
    void updateArmMoveBack();
    TA_Point getOptimalArmTarget();

    void updateDamage();
    void updateHitboxes();
    void drawArm();

    TA_Sprite headSprite, bodySprite, leftFootSprite, rightFootSprite, headFlashSprite;
    TA_Sprite armSprite, armPartSprite;
    TA_Sound hitSound;

    double timer = 0;
    double startX = 0;
    double invincibleTimer = invincibleTime;

    TA_Point armPosition, armTarget;
};

#endif