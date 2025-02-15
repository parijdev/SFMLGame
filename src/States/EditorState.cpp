#include "stdafx.h"
#include "States/EditorState.h"

//Inititalizer functions
void EditorState::initVariables()
{

}

void EditorState::initBackground()
{

}

void EditorState::initFonts()
{
	if (!this->font.loadFromFile("assets/fonts/ByteBounce.ttf"))
	{
		throw("ERROR::EDITOR_STATE::COULD_NOT_LOAD_FONT");
	}
}

void EditorState::initKeybinds()
{
	std::ifstream ifs("assets/config/editorstate_keybinds.ini");

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

void EditorState::initPauseMenu()
{
    this->pmenu = new PauseMenu(*this->window, this->font);
    this->pmenu->addButton("QUIT", 870.f, "Quit");
}

void EditorState::initButtons()
{

}

void EditorState::initGui()
{
    this->selectorRect.setSize(sf::Vector2f(this->stateData->gridSize, this->stateData->gridSize));

    this->selectorRect.setFillColor(sf::Color::Transparent);
    this->selectorRect.setOutlineThickness(1.f);
    this->selectorRect.setOutlineColor(sf::Color::Magenta);
}

void EditorState::initTileMap()
{
    this->tileMap = new TileMap(this->stateData->gridSize, 20 , 20);
}





EditorState::EditorState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
    this->initPauseMenu();
	this->initButtons();
    this->initGui();
    this->initTileMap();
}

EditorState::~EditorState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}

    delete this->pmenu;

    delete this->tileMap;
}
//Functions


void EditorState::updateInput(const float& dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeytime())
    {
        if (!this->paused)
            this->pauseState();
        else
            this->unpauseState();
    }
}

void EditorState::updateEditorInput(const float& dt)
{
    //Add a tile to the tilemap
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeytime())
    {
        this->tileMap->addTile(this->mousePosGrid.x, this->mousePosGrid.y, 0);
    }
    //Remove a tile to the tilemap
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getKeytime())
    {
        this->tileMap->removeTile(this->mousePosGrid.x, this->mousePosGrid.y, 0);
    }
}



void EditorState::updateButtons()
{
	//Update all the buttons in the state and handles their funtionlaity 

	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosView);
	}

}

void EditorState::updateGui()
{
    this->selectorRect.setPosition(this->mousePosGrid.x * this->stateData->gridSize, this->mousePosGrid.y * this->stateData->gridSize);
}

void EditorState::updatePauseMenuButtons() {
    if (this->pmenu->isButtonPressed("QUIT"))
        this->endState();
}


void EditorState::update(const float& dt)
{
	this->updateMousePositions();
    this->updateKeytime(dt);
	this->updateInput(dt);

    if(!this->paused)   //UNPAUSED
    {
        this->updateGui();
        this->updateButtons();
        this->updateEditorInput(dt);
    }
    else    //PAUSED
    {
        this->pmenu->update(this->mousePosView);
        this->updatePauseMenuButtons();
    }

	this->updateButtons();

}

void EditorState::renderButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}

}

void EditorState::renderGui(sf::RenderTarget &target)
{
    target.draw(this->selectorRect);
}


void EditorState::render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	this->renderButtons(*target);
    this->renderGui(*target);

    this->tileMap->render(*target);

    if (this->paused) // pause menu render
    {
        this->pmenu->render(*target);
    }


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





