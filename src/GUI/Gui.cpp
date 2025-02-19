#include "stdafx.h"
#include "GUI/Gui.h"

/*============================== BUTTONS =============================*/

GUI::Button::Button(float x, float y, float y_h, float width, float height,
              sf::Font* font, std::string text, unsigned character_size,
              sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
              sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
              sf::Color outline_idle_color,
              sf::Color outline_hover_color,
              sf::Color outline_active_color,
              short unsigned id)
{
	this->buttonState = BTN_IDLE;
    this->id = id;

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setFillColor(idle_color);
    this->shape.setOutlineThickness(0.f); //OUTLINE THICKNESS change if you want to have it
    this->shape.setOutlineColor(outline_idle_color);


	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(text_idle_color);
	this->text.setCharacterSize(character_size);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width /2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - y_h
	);

	this->textIdleColor = text_idle_color;
	this->textHoverColor = text_hover_color;
	this->textActiveColor = text_active_color;

	this->idleColor = idle_color;
	this->hoverColor = hover_color;
	this->activeColor = active_color;

    this->outlineIdleColor = outline_idle_color;
    this->outlineHoverColor = outline_hover_color;
    this->outlineActiveColor = outline_active_color;

}

GUI::Button::~Button()
{

}

//Accessors

const bool GUI::Button::isPressed() const
{
    if (this->buttonState == BTN_ACTIVE)
        return true;

    return false;
}

const std::string GUI::Button::getText() const
{
    return this->text.getString();
}

const short unsigned &GUI::Button::getId() const
{
    return this->id;
}


//Modifiers

void GUI::Button::setText(const std::string text)
{
    this->text.setString(text);
}

void GUI::Button::setId(const unsigned short id)
{
    this->id = id;
}

//Functions

void GUI::Button::update(const sf::Vector2f& mousePos)
{
	//UPDATE the booleanse for hover and pressed
	
	//IDLE
	this->buttonState = BTN_IDLE;

	//HOVER

	if (this->shape.getGlobalBounds().contains(mousePos))
	{
		this->buttonState = BTN_HOVER; 

		//PRESSED
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->buttonState = BTN_ACTIVE;
		}
	}

	switch (this->buttonState)
	{
	case BTN_IDLE:
		this->shape.setFillColor(this->idleColor);
		this->text.setFillColor(this->textIdleColor);
        this->shape.setOutlineColor(this->outlineIdleColor);
		break;

	case BTN_HOVER:
		this->shape.setFillColor(this->hoverColor);
		this->text.setFillColor(this->textHoverColor);
        this->shape.setOutlineColor(this->outlineHoverColor);
		break;

	case BTN_ACTIVE:
		this->shape.setFillColor(this->activeColor);
		this->text.setFillColor(this->textActiveColor);
            this->shape.setOutlineColor(this->outlineActiveColor);
		break;

	default:
		this->shape.setFillColor(sf::Color::Red);
		this->text.setFillColor(sf::Color::Blue);
        this->shape.setOutlineColor(sf::Color::Green);
		break;
	}
}

void GUI::Button::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	target.draw(this->text);
}




/*========================== DROP DOWN LIST =========================*/

GUI::DropDownList::DropDownList(float x, float y, float width, float height,
                                sf::Font& font, std::string list[],
                                unsigned nrOfElements, unsigned default_index)
    : font(font), showList(false), keytimeMax(1.f), keytime(0.f)
{
    //unsigned nrOfElements = sizeof(list) / sizeof (std::string);

    this->activeElement = new GUI::Button(
            x, y ,15.f, width, height,
            &this->font, list[default_index], 24.f,
            sf::Color(255, 255, 255, 150),
            sf::Color(255, 255, 255, 200),
            sf::Color(20, 20, 20, 50),
            sf::Color(70, 70, 70, 200),
            sf::Color(150, 150, 150, 200),
            sf::Color(20, 20, 20, 200),
            sf::Color(255, 255, 255, 200),
            sf::Color(255, 255, 255, 255),
            sf::Color(20, 20, 20, 50)
    );

    for (size_t i = 0; i < nrOfElements; ++i)
    {
        this->list.push_back(
                new GUI::Button(
                        x, y + ((i+1) * height),15.f, width, height,
                        &this->font, list[i], 24.f,
                        sf::Color(255, 255, 255, 150),
                        sf::Color(255, 255, 255, 255),
                        sf::Color(20, 20, 20, 50),
                        sf::Color(70, 70, 70, 200),
                        sf::Color(150, 150, 150, 200),
                        sf::Color(20, 20, 20, 200),
                        sf::Color(255, 255, 255, 0),
                        sf::Color(255, 255, 255, 0),
                        sf::Color(20, 20, 20, 0),
                        i
                ));
    }

}

GUI::DropDownList::~DropDownList() {
    delete this->activeElement;
    for (size_t i = 0; i < this->list.size(); i++)
    {
        delete this->list[i];
    }
}

//Accessors

const bool GUI::DropDownList::getKeytime()
{
    if(this->keytime >= this->keytimeMax)
    {
        this->keytime = 0.f;
        return true;
    }
    return false;
}

const unsigned short &GUI::DropDownList::getActiveElementId() const
{
    return this->activeElement->getId();
}

//Functions

void GUI::DropDownList::updateKeytime(const float& dt)
{
    if(this->keytime < this->keytimeMax)
        this->keytime += 10.f * dt;
}


void GUI::DropDownList::update(const sf::Vector2f &mousePos, const float& dt)
{
    this->updateKeytime(dt);

    this->activeElement->update(mousePos);

    //Show and hide the list

    if(this->activeElement->isPressed() && this->getKeytime())
    {
        if(this->showList)
            this->showList = false;
        else
            this->showList = true;
    }
    if(this->showList)
    {
        for(auto &i : this->list)
        {
            i->update(mousePos);

            if(i->isPressed() && this->getKeytime())
            {
                this->showList = false;
                this->activeElement->setText(i->getText());
                this->activeElement->setId(i->getId());
            }
        }
    }
}

void GUI::DropDownList::render(sf::RenderTarget &target)
{
    this->activeElement->render(target);

    if(this->showList)
    {
        for(auto &i : this->list)
        {
            i->render(target);
        }
    }
}




/*========================== TEXTURE SELECTOR =========================*/




GUI::TextureSelector::TextureSelector(float x, float y, float width, float height,float gridSize,
                                      const sf::Texture* texture_sheet, sf::Font& font, std::string text)
                                      : keytimeMax(3.f), keytime(0.f)
{
    this->gridSize = gridSize;
    this->active = false;
    this->hidden = false;

    float offset = 100.f;

    this->bounds.setSize(sf::Vector2f (width, height));
    this->bounds.setPosition(x + offset, y);
    this->bounds.setFillColor(sf::Color(50,50,50,100));
    this->bounds.setOutlineThickness(1.f);
    this->bounds.setOutlineColor(sf::Color(255,255,255,200));

    this->sheet.setTexture(*texture_sheet);
    this->sheet.setPosition(x + offset, y);

    if(this->sheet.getGlobalBounds().width > this->bounds.getGlobalBounds().width)
    {
        this->sheet.setTextureRect(sf::IntRect (0,0,
                                                this->bounds.getGlobalBounds().width,
                                                this->sheet.getGlobalBounds().height ));
    }
    if(this->sheet.getGlobalBounds().height > this->bounds.getGlobalBounds().height)
    {
        this->sheet.setTextureRect(sf::IntRect (0,0,
                                                this->sheet.getGlobalBounds().width,
                                                this->bounds.getGlobalBounds().height));
    }
    this->selector.setPosition(x + offset,y);
    this->selector.setSize(sf::Vector2f(gridSize, gridSize));
    this->selector.setFillColor(sf::Color::Transparent);
    this->selector.setOutlineThickness(2.f);
    this->selector.setOutlineColor(sf::Color::Red);

    this->textureRect.width = static_cast<int>(gridSize);
    this->textureRect.height = static_cast<int>(gridSize);

    this->hide_btn = new GUI::Button(
            x, 50.f,30.f, 80.f, 100.f,
            &font, text, 45.f,
            sf::Color(130, 130, 130, 225),
            sf::Color(250, 250, 250, 250),
            sf::Color(20, 20, 20, 50),
            sf::Color(70, 70, 70, 200),
            sf::Color(150, 150, 150, 250),
            sf::Color(20, 20, 20, 50));;
}

GUI::TextureSelector::~TextureSelector()
{
    delete this->hide_btn;
}

//Accessors

const bool &GUI::TextureSelector::getActive() const
{
    return this->active;
}

const sf::IntRect &GUI::TextureSelector::getTextureRect() const
{
    return this->textureRect;
}

const bool GUI::TextureSelector::getKeytime()
{
    if(this->keytime >= this->keytimeMax)
    {
        this->keytime = 0.f;
        return true;
    }
    return false;
}

//Functions

void GUI::TextureSelector::updateKeytime(const float& dt)
{
    if(this->keytime < this->keytimeMax)
        this->keytime += 10.f * dt;
}

void GUI::TextureSelector::update(const sf::Vector2i& mousePosWindow, const float& dt ) {
    this->updateKeytime(dt);
    this->hide_btn->update(static_cast<sf::Vector2f>(mousePosWindow));

    if(this->hide_btn->isPressed() && this->getKeytime())
    {
        if(this->hidden)
            this->hidden = false;
        else
            this->hidden = true;
    }

    if(!this->hidden)
    {
        if(this->bounds.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
        {
            this->active = true;
        }
        else
        {
            this->active = false;
        }

        if(this->active)
        {
            this->mousePosGrid.x = (mousePosWindow.x - static_cast<int>(this->bounds.getPosition().x)) / static_cast<unsigned>(this->gridSize);
            this->mousePosGrid.y = (mousePosWindow.y - static_cast<int>(this->bounds.getPosition().y)) / static_cast<unsigned>(this->gridSize);

            this->selector.setPosition(this->bounds.getPosition().x + this->mousePosGrid.x * this->gridSize,
                                       this->bounds.getPosition().y + this->mousePosGrid.y * this->gridSize
            );
        }

        //Update texture rectangle
        this->textureRect.left = static_cast<int>(this->selector.getPosition().x - this->bounds.getPosition().x);
        this->textureRect.top = static_cast<int>(this->selector.getPosition().y - this->bounds.getPosition().y);
    }

}

void GUI::TextureSelector::render(sf::RenderTarget &target)
{
    if(!this->hidden)
    {

        target.draw(this->bounds);
        target.draw(this->sheet);

        if(this->active)
            target.draw(this->selector);
    }
    this->hide_btn->render(target);

}



