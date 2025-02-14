#include "stdafx.h"
#include "Entities/Player.h"

//initializer functions
void Player::initVariables()
{
	this->attacking = false;
	this->attackDirection = true;
	this->idleTimer = 0.f;
	this->idleSpecialAnimation = false;
}

void Player::initComponents()
{

}

//constructor/destructor
Player::Player(float x, float y, sf::Texture& texture_sheet)
{
	this->initVariables();

	this->setPosition(x, y);

	this->createHitboxComponent(this->sprite, 192.f, 245.f, 192.f, 139.f);
	this->createMovementComponent(300.f, 15.f, 5.f);
	this->createAnimationComponent(texture_sheet);

	this->animationComponent->addAnimation("IDLE", 11.f,
                                           0, 0,
                                           3, 0,
                                           576, 384);
	this->animationComponent->addAnimation("WALK", 3.f,
                                           0, 0,
                                           3, 0,
                                           576, 384);
	this->animationComponent->addAnimation("IDLE_JUMP", 9.f,
                                           1, 1,
                                           5, 1,
                                           576, 384);
	this->animationComponent->addAnimation("ATTACK", 6.f,
                                           0, 2,
                                           12, 2,
                                           576, 384);
}

Player::~Player()
{

}


//Functions

void Player::updateAttack()
{

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !this->attacking)
	{
		this->attacking = true;

		this->attackDirection = (this->sprite.getScale().x > 0.f); 
	}
}


void Player::updateAnimations(const float& dt)
{
	if (this->attacking)
	{
		if (this->attackDirection)
		{
			this->sprite.setOrigin(0.f, 0.f);
			this->sprite.setScale(1.f, 1.f);
		}
		else
		{
			this->sprite.setOrigin(576.f, 0.f);
			this->sprite.setScale(-1.f, 1.f);
		}
		//animate and check for animation ends
		if (this->animationComponent->play("ATTACK", dt, true))
		{
			this->idleTimer = 0.f;
			this->attacking = false;
		}
	}


	if (this->movementComponent->getState(IDLE))
	{

		this->idleTimer += dt;

		if (this->idleTimer >= 10.f && !this->idleSpecialAnimation)
		{
			if (this->animationComponent->play("IDLE_JUMP", dt))
			{
				this->idleSpecialAnimation = false;
				this->idleTimer = 0.f;
			}
		}
		else if (this->idleTimer < 10.f)
		{
			this->animationComponent->play("IDLE", dt);
		}
	}
	else
	{

		if (this->movementComponent->getState(MOVING_LEFT) && !this->attacking)
		{
			if (this->sprite.getScale().x < 0.f)
			{
				this->sprite.setOrigin(0.f, 0.f);
				this->sprite.setScale(1.f, 1.f);
			}
			this->idleTimer = 0.f;
			this->animationComponent->play("WALK", dt,
                                           this->movementComponent->getVelocity().x,
                                           this->movementComponent->getMaxVelocity());
		}
		else if (this->movementComponent->getState(MOVING_RIGHT) && !this->attacking)
		{
			if (this->sprite.getScale().x > 0.f)
			{
				this->sprite.setOrigin(576.f, 0.f);
				this->sprite.setScale(-1.f, 1.f);
			}
			this->idleTimer = 0.f;
			this->animationComponent->play("WALK", dt,
                                           this->movementComponent->getVelocity().x,
                                           this->movementComponent->getMaxVelocity());
		}
		else if (this->movementComponent->getState(MOVING_UP) || this->movementComponent->getState(MOVING_DOWN))
		{
			this->idleTimer = 0.f;
			this->animationComponent->play("WALK", dt,
                                           this->movementComponent->getVelocity().y,
                                           this->movementComponent->getMaxVelocity());
		}
	}
}
 

void Player::update(const float& dt)
{
	
	this->movementComponent->update(dt);

	this->updateAttack();

	this->updateAnimations(dt);

	this->hitboxComponent->update();
}
