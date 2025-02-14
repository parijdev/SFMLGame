#include "stdafx.h"
#include "States/MainMenuState.h"

//Inititalizer functions
void MainMenuState::initVariables()
{

}

void MainMenuState::initBackground()
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

void MainMenuState::initFonts()
{
	if (!this->font.loadFromFile("assets/fonts/ByteBounce.ttf"))
	{
		throw("ERROR::MAIN_MENU_STATE::COULD_NOT_LOAD_FONT");
	}
}

void MainMenuState::initKeybinds()
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

void MainMenuState::initButtons()
{
    float x = this->background.getPosition().x + this->background.getSize().x / 2.f;

	this->buttons["GAME_STATE"] = new GUI::Button(
		x - 235.f, 600.f,85.f, 470.f, 50.f,
		&this->font, "New Game", 125.f,
		sf::Color(130, 130, 130, 225),
        sf::Color(250, 250, 250, 250),
        sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0),
        sf::Color(150, 150, 150, 0),
        sf::Color(20, 20, 20, 0));

	this->buttons["SETTINGS_STATE"] = new GUI::Button(
		x - 125.f, 740.f,60.f, 250.f, 50.f,
		&this->font, "Settings", 80.f,
		sf::Color(110, 110, 110, 225),
        sf::Color(250, 250, 250, 250),
        sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0),
        sf::Color(150, 150, 150, 0),
        sf::Color(20, 20, 20, 0));

	this->buttons["EDITOR_STATE"] = new GUI::Button(
		x - 100.f, 820.f,50.f, 200.f, 50.f,
		&this->font, "Editor", 80.f,
		sf::Color(110, 110, 110, 225),
        sf::Color(250, 250, 250, 250),
        sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0),
        sf::Color(150, 150, 150, 0),
        sf::Color(20, 20, 20, 0));

	this->buttons["EXIT_STATE"] = new GUI::Button(
		x - 70.f, 920.f,60.f, 140.f, 50.f,
		&this->font, "Quit", 80.f,
		sf::Color(110, 110, 110, 225),
        sf::Color(250, 250, 250, 250),
        sf::Color(20, 20, 20, 50),
		sf::Color(70, 70, 70, 0),
        sf::Color(150, 150, 150, 0),
        sf::Color(20, 20, 20, 0));
}


MainMenuState::MainMenuState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	: State(window, supportedKeys, states)
{
	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initButtons();
}

MainMenuState::~MainMenuState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}


void MainMenuState::updateInput(const float& dt)
{

}

void MainMenuState::updateButtons()
{
	//Update all the buttons in the state and handles their funtionlaity 

	for (auto &it : this->buttons)
	{
		it.second->update(this->mousePosView);
	}

	//New game
	if (this->buttons["GAME_STATE"]->isPressed())
	{
		this->states->push(new GameState(this->window, this->supportedKeys, this->states));
	}

	//Settings
    if (this->buttons["SETTINGS_STATE"]->isPressed())
    {
        this->states->push(new SettingsState(this->window, this->supportedKeys, this->states));
    }

	//Editor
	if (this->buttons["EDITOR_STATE"]->isPressed())
	{
		this->states->push(new EditorState(this->window, this->supportedKeys, this->states));
	}


	//QUIT the game
	if (this->buttons["EXIT_STATE"]->isPressed())
	{
		this->endState();
	}

}

void MainMenuState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);

	this->updateButtons();

}

void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	for (auto &it : this->buttons)
	{
		it.second->render(target);
	}

}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->draw(this->background);

	this->renderButtons(*target);


	//REMOVE LATER!!!!!! DEBUG FEATURE
	//sf::Text mouseText;
	//mouseText.setPosition(this->mousePosView.x, this->mousePosView.y + 15);
	//mouseText.setFont(this->font);
	//mouseText.setCharacterSize(18);
	//std::stringstream ss;
	//ss << this->mousePosView.x << " " << this->mousePosView.y;
	//mouseText.setString(ss.str());
	//target->draw(mouseText);
}
