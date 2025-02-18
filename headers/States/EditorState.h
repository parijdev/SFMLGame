#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "State.h"
#include "GUI/Gui.h"
#include "GUI/PauseMenu.h"
#include "Map/TileMap.h"

class State;
class Gui;
class PauseMenu;
class TileMap;

class EditorState :
    public State
{
private:
    //Variables
    float keytime;
    const float keytimeMax;

    sf::Font font;
    sf::Text cursorText;
    PauseMenu* pmenu;

    std::map<std::string, GUI::Button*>buttons;

    TileMap* tileMap;

    sf::RectangleShape sidebar;

    sf::IntRect textureRect;
    sf::RectangleShape selectorRect;

    GUI::TextureSelector* textureSelector;



    //Functions
    void initVariables();
    void initBackground();
    void initFonts();
    void initText();
    void initKeybinds();
    void initPauseMenu();
    void initButtons();
    void initGui();
    void initTileMap();


public:
    EditorState(StateData* state_data);
    virtual ~EditorState();

    //Accessors
    const bool getKeytime();

    //Functions
    void updateKeytime(const float& dt);
    void updateInput(const float& dt);
    void updateEditorInput(const float& dt);
    void updateButtons();
    void updateGui(const float& dt);
    void updatePauseMenuButtons();
    void update(const float& dt);
    void renderButtons(sf::RenderTarget& target);
    void renderGui(sf::RenderTarget& target);
    void render(sf::RenderTarget* target = NULL);
};


#endif
