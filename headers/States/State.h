#ifndef STATE_H
#define STATE_H

#include "Entities/Player.h"
#include "GUI/GraphicsSettings.h"

class Player;
class GraphicsSettings;
class State;

class StateData
{
public:
    StateData() {};

    //Variables
    float gridSize;
    sf::RenderWindow* window;
    GraphicsSettings* gfxSettings;
    std::map<std::string, int>* supportedKeys;
    std::stack<State*>* states;

};

class State
{
private:

protected:
    StateData* stateData;
	std::stack<State*>* states;
	sf::RenderWindow* window;
	std::map<std::string, int>* supportedKeys;
	std::map<std::string, int> keybinds;
	bool quit;
	bool paused;
    float keytime;
    float keytimeMax;
    float pauseKeytime;
    float pauseKeytimeMax;
    float gridSize;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
    sf::Vector2u mousePosGrid;

	//Resources
	std::map<std::string, sf::Texture> textures;

	//Functions
	virtual void initKeybinds() = 0;


public:
	State(StateData* state_data);
	virtual ~State();

    //Accessors
    const bool& getQuit() const;
    const bool getKeytime();
    const bool getPauseKeytime();


    //Functions
	void endState();
	void pauseState();
	void unpauseState();

	virtual void updateMousePositions();
    virtual void updateKeytime(const float& dt);
    virtual void updatePauseKeytime(const float& dt); // update keytime for pause button
	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = NULL) = 0;
};

#endif
