#include "stdafx.h"
#include "GUI/PauseMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow &window, sf::Font &font)
        : font(font) {
    //INIT BACKGROUND
    this->background.setSize(
            sf::Vector2f(
                    static_cast<float>(window.getSize().x),
                    static_cast<float>(window.getSize().y)
            )
    );
    this->background.setFillColor(sf::Color(20, 20, 20, 100));


    //INIT CONTAINER
    this->container.setSize(
            sf::Vector2f(
                    static_cast<float>(window.getSize().x) / 4.f,
                    static_cast<float>(window.getSize().y) - 60.f
            )
    );
    this->container.setFillColor(sf::Color(20, 20, 20, 200));
    this->container.setPosition(
            static_cast<float>(window.getSize().x) / 2.f - this->container.getSize().x / 2.f,
            30.f
    );

    //init text
    this->menuText.setFont(font);
    this->menuText.setFillColor(sf::Color(255, 255, 255, 200));
    this->menuText.setCharacterSize(60);
    this->menuText.setString("PAUSED");
    this->menuText.setPosition(this->container.getPosition().x + this->container.getSize().x / 2.f -
                               this->menuText.getGlobalBounds().width / 2.f,
                               this->container.getPosition().y + 40.f);

}

PauseMenu::~PauseMenu() {
    auto it = this->buttons.begin();
    for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
        delete it->second;
    }
}

std::map<std::string, GUI::Button *> &PauseMenu::getButtons() {
    return this->buttons;
}

//Functions

const bool PauseMenu::isButtonPressed(const std::string key) {
    return this->buttons[key]->isPressed();
}

void PauseMenu::addButton(const std::string key, float y, const std::string text) {
    float width = 120.f;
    float height = 40.f;
    float x = this->container.getPosition().x + this->container.getSize().x / 2.f - width / 2.f;

    this->buttons[key] = new GUI::Button(
        x, y,50.f, width, height,
            &this->font, text, 80,
            sf::Color(70, 70, 70, 200),
            sf::Color(250, 250, 250, 250),
            sf::Color(20, 20, 20, 50),
            sf::Color(70, 70, 70, 0),
            sf::Color(150, 150, 150, 0),
            sf::Color(20, 20, 20, 0));

}

void PauseMenu::update(const sf::Vector2f &mousePos) {
    for (auto &i: this->buttons) {
        i.second->update(mousePos);
    }
}

void PauseMenu::render(sf::RenderTarget &target) {
    target.draw(this->background);
    target.draw(this->container);
    for (auto &i: this->buttons) {
        i.second->render(target);
    }
    target.draw(this->menuText);
}
