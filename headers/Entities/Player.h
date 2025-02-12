#ifndef PLAYER_H
#define PLAYER_H


#include "Entity.h"

class Player :
    public Entity
{
private:
    //Variables
    float idleTimer;
    bool attacking;
    bool attackDirection;

    bool idleSpecialAnimation;

    //Initializer functions
    void initVariables();
    void initComponents();

public:
    Player(float x, float y, sf::Texture& texture_sheet);
    virtual ~Player();

    //Functions
    void updateAttack();
    void updateAnimations(const float& dt);
    virtual void update(const float& dt);
};


#endif

