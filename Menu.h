#pragma once
#include "raylib.h"
#include "Player.h"

// Character node for linked list
typedef struct CharacterNode {
    PlayerCharacter type;
    Texture2D texture;            // Character preview texture
    char name[50];                // Character name
    char description[200];        // Character description
    struct CharacterNode* next;   // Pointer to next character
} CharacterNode;

typedef struct Menu {
    Texture2D background_texture;
    Texture2D startButton_texture;
    Texture2D exitButton_texture;
    Texture2D keyBindings_texture;
    Texture2D keyBindingsGuide_texture;
    Texture2D enterName_texture;
    Texture2D selectCharacter_texture;

    Rectangle startButton_rectangle;
    Rectangle keyBindings_rectangle;
    Rectangle exitButton_rectangle;

    Texture2D levelSelectionBackground_texture;
    Texture2D level1Panel_texture;
    Texture2D level2Panel_texture;
    Texture2D level3Panel_texture;

    Rectangle level1Panel_rectangle;
    Rectangle level2Panel_rectangle;
    Rectangle level3Panel_rectangle;

    Rectangle startButton_position;
    Rectangle keyBindings_position;
    Rectangle exitButton_position;

    Rectangle nameInputBox;

    char playerName[500];
    int nameLength;
    bool nameInputActive;

    bool start_Game;
    bool exit_Game;
    bool showKeyBindings;
    bool showNameInput;
    bool pause_Game;
    bool showLevelSelection;
    bool showCharacterSelection;

    int currentLevel;

    // Character selection
    CharacterNode* characterList;     // Head of character linked list
    CharacterNode* currentCharacter;  // Currently selected character
    PlayerCharacter selectedCharacter;  // Final selected character
} Menu;

void MainMenu_Init(Menu* menu);
void MainMenu_Unload(Menu* menu);
void MainMenu_UpdateStartButton(Menu* menu);
void MainMenu_UpdateKeyBindingsButton(Menu* menu);
void MainMenu_UpdateExitButton(Menu* menu);
void MainMenu_ShowNameInput(Menu* menu);
void MainMenu_LandingPage(Menu* menu);
void MainMenu_ShowKeyBindings(Menu* menu);
void MainMenu_Update(Menu* menu);
void MainMenu_ShowLevelSelection(Menu* menu);
void MainMenu_SelectingACharacter(Menu* menu);
int GetOrCreatePlayerLevel(const char* playerName);
