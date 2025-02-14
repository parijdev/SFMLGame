#include "stdafx.h"
#include "Game.h"

//Static functions

//Init functions

void Game::initVariables()
{
    this->window = NULL;
    this->fullscreen = false;
    this->dt = 0.f;
}

void Game::initWindow() {

    /*Creates a SFML window using options from window.ini file*/

    std::ifstream ifs("assets/config/window.ini");
    this->videoModes = sf::VideoMode::getFullscreenModes();

    std::string title = "None";
    sf::VideoMode window_bounds = sf::VideoMode::getDesktopMode();
    bool fullscreen = false;
    unsigned framerate_limit = 120;
    bool vertical_sync_enabled = false;
    unsigned antialiasing_level = 0;

    if (ifs.is_open())
    {
        std::getline(ifs, title);
        ifs >> window_bounds.width >> window_bounds.height;
        ifs >> fullscreen;
        ifs >> framerate_limit;
        ifs >> vertical_sync_enabled;
        ifs >> antialiasing_level;
    }

    ifs.close();

    this->fullscreen = fullscreen;
    this->windowSettings.antialiasingLevel = antialiasing_level;

    if (this->fullscreen)
        this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Fullscreen, windowSettings);
    else
        this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Titlebar | sf::Style::Close, windowSettings);
        
    this->window->setFramerateLimit(framerate_limit);
    this->window->setVerticalSyncEnabled(vertical_sync_enabled);
}

void Game::initKeys()
{
    std::ifstream ifs("assets/config/supported_keys.ini");

    if (ifs.is_open()) 
    {
        std::string key = "";
        int key_value = 0;
        while (ifs >> key >> key_value)
        {
            this->supportedKeys[key] = key_value;
        }
    }

    ifs.close();



    //DEBUG REMOVE LATER
    for (auto i : this->supportedKeys)
    {
        std::cout << i.first << " " << i.second << '\n';
    }

}

void Game::initStates()
{
    //push MainMenuState for menu or GameState to play the game
    this->states.push(new MainMenuState(this->window, &this->supportedKeys, &this->states));
   
}

//Constructor/Destructor

Game::Game()
{
    this->initWindow();
    this->initKeys();
    this->initStates();

}

Game::~Game()
{
	delete this->window;

    while (!this->states.empty()) 
    {
        delete this->states.top();
        this->states.pop();
    }

}

//Functions

void Game::endApplication()
{
    std::cout << "Ending Application!" << "\n";
}

void Game::updateDt() {
    /*Updates the dt variable with the time it takes to update and render one frame.*/

    this->dt = this->dtClock.restart().asSeconds();

    //system("cls");
    //std::cout << this->dt << '\n';
}

void Game::updateSFMLEvents(){

    while (this->window->pollEvent(this->sfEvent))
    {
        if (this->sfEvent.type == sf::Event::Closed)
            this->window->close();
    }

}

void Game::update(){

    this->updateSFMLEvents();

    if (!this->states.empty())
    {
        this->states.top()->update(this->dt);

        if (this->states.top()->getQuit()) 
        {
            this->states.top()->endState();
            delete this->states.top();
            this->states.pop();
        }
    }

    //Application end
    else
    {
        this->endApplication();
        this->window->close();
    }
    
}

void Game::render(){
    this->window->clear();

    //Render items
    if (!this->states.empty())
        this->states.top()->render(this->window);

    this->window->display();
}

void Game::run(){
    while (this->window->isOpen())
    {
        this->updateDt();
        this->update();
        this->render();
    }
}
