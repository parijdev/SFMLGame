#include "stdafx.h"
#include "States/SettingsState.h"

void SettingsState::initVariables()
{
    this->modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::initBackground()
{
    this->background.setSize(
            sf::Vector2f
                    (
                            static_cast<float> (this->window->getSize().x),
                            static_cast<float> (this->window->getSize().y)
                    )
    );

    if (!this->backgroundTexture.loadFromFile("assets/images/backgrounds/bg1.png"))
    {
        throw "ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
    }
    this->background.setTexture(&this->backgroundTexture);

}

void SettingsState::initFonts()
{
    if (!this->font.loadFromFile("assets/fonts/ByteBounce.ttf"))
    {
        throw("ERROR::MAIN_MENU_STATE::COULD_NOT_LOAD_FONT");
    }
}

void SettingsState::initKeybinds()
{
    std::ifstream ifs("assets/config/mainmenustate_keybinds.ini");

    if (ifs.is_open())
    {
        std::string key = "";
        std::string key2 = "";

        while (ifs >> key >> key2)
        {
            this->keybinds[key] = this->supportedKeys->at(key2);
        }
    }

    ifs.close();

}

void SettingsState::initGui()
{
    float x = this->background.getPosition().x + this->background.getSize().x / 2.f - 250 / 2.f;

    this->buttons["BACK"] = new GUI::Button(
            643, 900.f,60.f, 250.f, 50.f,
            &this->font, "Back", 80.f,
            sf::Color(110, 110, 110, 225),
            sf::Color(250, 250, 250, 250),
            sf::Color(20, 20, 20, 50),
            sf::Color(70, 70, 70, 0),
            sf::Color(150, 150, 150, 0),
            sf::Color(20, 20, 20, 0));

    this->buttons["APPLY"] = new GUI::Button(
            1010, 900.f,60.f, 250.f, 50.f,
            &this->font, "Apply", 80.f,
            sf::Color(110, 110, 110, 225),
            sf::Color(250, 250, 250, 250),
            sf::Color(20, 20, 20, 50),
            sf::Color(70, 70, 70, 0),
            sf::Color(150, 150, 150, 0),
            sf::Color(20, 20, 20, 0));

    std::vector<std::string> modes_str;
    for(auto &i : this->modes)
    {
        modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
    }

    this->dropDownLists["RESOLUTION"] = new GUI::DropDownList(1020, 514, 200, 50, font,
                                                              modes_str.data(), modes_str.size());
}

void SettingsState::initText()
{
    this->optionsText.setFont(this->font);
    this->optionsText.setPosition(sf::Vector2f (700.f, 500.f));
    this->optionsText.setCharacterSize(60.f);
    this->optionsText.setFillColor(sf::Color(255,255,255,200));

    this->optionsText.setString(
            "Resolution \n\nFullscreen \n\nVsync \n\nAntialiasing \n\n "
            );
}


SettingsState::SettingsState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
    : State(window, supportedKeys, states)
{
    this->initVariables();
    this->initBackground();
    this->initFonts();
    this->initKeybinds();
    this->initGui();
    this->initText();
}

SettingsState::~SettingsState()
{
    auto it = this->buttons.begin();
    for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
    {
        delete it->second;
    }

    auto it2 = this->dropDownLists.begin();
    for (it2 = this->dropDownLists.begin(); it2 != this->dropDownLists.end(); ++it2)
    {
        delete it2->second;
    }
}

//Accessors

//Functions

void SettingsState::updateInput(const float& dt)
{

}

void SettingsState::updateGui(const float& dt)
{
    //Update all the GUI elements in the state and handles their functionality.
    //Buttons

    for (auto &it : this->buttons)
    {
        it.second->update(this->mousePosView);
    }

    //Button functionality
    //QUIT the game
    if (this->buttons["BACK"]->isPressed())
    {
        this->endState();
    }
    //APPLY selected settings
    if (this->buttons["APPLY"]->isPressed())
    {
        //TEST REMOVE LATER
        this->window->create(this->modes[this->dropDownLists["RESOLUTION"]->getActiveElementId()], "Test", sf::Style::Default);
    }
    //Dropdown lists
    for (auto &it : this->dropDownLists)
    {
        it.second->update(this->mousePosView, dt);
    }

    //Dropdown lists functionality
}

void SettingsState::update(const float& dt)
{
    this->updateMousePositions();
    this->updateInput(dt);

    this->updateGui(dt);

}

void SettingsState::renderGui(sf::RenderTarget& target)
{
    for (auto &it : this->buttons)
    {
        it.second->render(target);
    }

    for (auto &it : this->dropDownLists)
    {
        it.second->render(target);
    }

}

void SettingsState::render(sf::RenderTarget* target)
{
    if (!target)
        target = this->window;

    target->draw(this->background);

    this->renderGui(*target);

    target->draw(this->optionsText);

    //REMOVE LATER!!!!!! DEBUG FEATURE
    sf::Text mouseText;
    mouseText.setPosition(this->mousePosView.x, this->mousePosView.y + 15);
    mouseText.setFont(this->font);
    mouseText.setCharacterSize(18);
    std::stringstream ss;
    ss << this->mousePosView.x << " " << this->mousePosView.y;
    mouseText.setString(ss.str());
    target->draw(mouseText);
}


