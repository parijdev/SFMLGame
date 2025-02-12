#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <iostream>
#include <map>


#include "SFML\System.hpp"
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
#include "SFML\Network.hpp"

class AnimationComponent
{
private:
	class Animation
	{
	public:
		//variables
		sf::Sprite& sprite;
		sf::Texture& textureSheet;
		float animationTimer;
		float timer;
		bool done;
		int width;
		int height;
		sf::IntRect startRect;
		sf::IntRect currentRect;
		sf::IntRect endRect;

		Animation(sf::Sprite& sprite,sf::Texture& texture_sheet
			,float animation_timer
			,int start_frame_x, int start_frame_y, int frames_x, int frames_y, int width, int height)
			:sprite (sprite)
			,textureSheet(texture_sheet)
			,animationTimer(animation_timer)
			,timer(0.f), done(false)
			,width(width), height(height)
		{
			this->startRect = sf::IntRect(start_frame_x * width, start_frame_y * height, width, height);
			this->currentRect = this->startRect;
			this->endRect = sf::IntRect(frames_x * width, frames_y * height, width, height);

			this->sprite.setTexture(this->textureSheet, true);
			this->sprite.setTextureRect(this->startRect);
		}

		//accessor
		const bool& isDone() const
		{
			return this->done;
		}
		//functions
		const bool& play(const float& dt)
		{
			//update timer
			this->done = false;
			this->timer += 85.f * dt;
			if (this->timer >= this->animationTimer)
			{
				//reset timer
				this->timer = 0.f;

				//animate
				if (this->currentRect != this->endRect)
				{
					this->currentRect.left += this->width;
				}
				else // reset
				{
					this->currentRect.left = this->startRect.left;
					this->done = true;
				}

				this->sprite.setTextureRect(this->currentRect);
			}
			return this->done;
		}

		const bool& play(const float& dt, float mod_percent)
		{
			//update timer

			if (mod_percent < 0.5f)
				mod_percent = 0.5f;

			this->done = false;
			this->timer += mod_percent * 85.f * dt;
			if (this->timer >= this->animationTimer)
			{
				//reset timer
				this->timer = 0.f;

				//animate
				if (this->currentRect != this->endRect)
				{
					this->currentRect.left += this->width;
				}
				else // reset
				{
					this->currentRect.left = this->startRect.left;
					this->done = true;
				}

				this->sprite.setTextureRect(this->currentRect);
			}
			return this->done;
		}

		void reset() 
		{
			this->timer = this->animationTimer;
			this->currentRect = this->startRect;
		}
	};

	sf::Sprite& sprite;
	sf::Texture& texture_sheet;
	std::map<std::string, Animation*> animations;
	Animation* lastAnimation;
	Animation* priorityAnimation;

public: 
	AnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet);
	virtual ~AnimationComponent();

	//accessors
	const bool& isDone(const std::string key);

	//functions
	void addAnimation(const std::string key
		, float animation_timer
		, int start_frame_x, int start_frame_y, int frames_x, int frames_y, int width, int height);


	const bool& play(const std::string key, const float& dt, const bool priority = false);
	const bool& play(const std::string key, const float& dt, const float& modifier, const float& modifier_max, const bool priority = false);
};


#endif
