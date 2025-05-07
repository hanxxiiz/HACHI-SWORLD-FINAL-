#include "raylib.h"
#include "Menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CharacterNode* CreateCharacterNode(PlayerCharacter type, const char* name, const char* description, const char* texturePath) {
    CharacterNode* node = (CharacterNode*)malloc(sizeof(CharacterNode));
    if (node == NULL) {
        printf("Memory allocation failed for character node\n");
        return NULL;
    }

    node->type = type;
    strcpy_s(node->name, 50, name);
    strcpy_s(node->description, 200, description);
    node->texture = LoadTexture(texturePath);
    node->next = NULL;

    return node;
}

void MainMenu_Init(Menu* menu)
{
    menu->background_texture = LoadTexture("Resources/Menu/MainBackgroundBG.png");
    menu->startButton_texture = LoadTexture("Resources/Menu/StartGameButton.png");
    menu->exitButton_texture = LoadTexture("Resources/Menu/ExitGameButton.png");
    menu->keyBindings_texture = LoadTexture("Resources/Menu/KeyBindingsButton.png");
    menu->keyBindingsGuide_texture = LoadTexture("Resources/Menu/keyBindingsBG.png");
    menu->enterName_texture = LoadTexture("Resources/Menu/EnterNameBG.png");
    menu->selectCharacter_texture = LoadTexture("Resources/Menu/CharacterSelectionBG.png");

    menu->startButton_rectangle = (Rectangle){ 0, 0, 427, 66 };
    menu->keyBindings_rectangle = (Rectangle){ 0, 0, 427, 66 };
    menu->exitButton_rectangle = (Rectangle){ 0, 0, 427, 66 };

    menu->levelSelectionBackground_texture = LoadTexture("LevelSelectionBG.png");
    menu->level1Panel_texture = LoadTexture("Panel_LEVEL1.png");
    menu->level2Panel_texture = LoadTexture("Panel_LEVEL2.png");
    menu->level3Panel_texture = LoadTexture("Panel_LEVEL3.png");

    menu->level1Panel_rectangle = (Rectangle){ 0, 0, 1130, 201 };
    menu->level2Panel_rectangle = (Rectangle){ 0, 0, 1130, 201 };
    menu->level3Panel_rectangle = (Rectangle){ 0, 0, 1130, 201 };

    menu->startButton_position = (Rectangle){ GetScreenWidth() / 2 - 213, GetScreenHeight() / 2 - 30, 427, 66 };
    menu->keyBindings_position = (Rectangle){ GetScreenWidth() / 2 - 213, 590, 427, 66 };
    menu->exitButton_position = (Rectangle){ GetScreenWidth() / 2 - 213, 670, 427, 66 };

    menu->nameInputBox = (Rectangle){ GetScreenWidth() / 2 - 200, 520, 400, 50 };

    strcpy_s(menu->playerName, 500, "");
    menu->nameLength = 0;
    menu->nameInputActive = false;

    menu->start_Game = false;
    menu->exit_Game = false;
    menu->showKeyBindings = false;
    menu->showNameInput = false;
    menu->showCharacterSelection = false;
    menu->pause_Game = false;
    menu->showLevelSelection = false;
    menu->selectedCharacter = PLAYER_HACHI;

    menu->characterList = NULL;
    menu->currentCharacter = NULL;

    CharacterNode* hachi = CreateCharacterNode(PLAYER_HACHI, "Hachi",
        "A brave dog warrior with exceptional speed and loyalty.", "Resources/Characters/Hachi.png");

    CharacterNode* sophie = CreateCharacterNode(PLAYER_SOPHIE, "Sophie",
        "A wise cat mage who uses powerful spells.", "Resources/Characters/Sophie.png");

    CharacterNode* bubbles = CreateCharacterNode(PLAYER_BUBBLES, "Bubbles",
        "A playful fish with water-based abilities.", "Resources/Characters/Bubbles.png");

    if (hachi && sophie && bubbles) {
        hachi->next = sophie;
        sophie->next = bubbles;
        bubbles->next = hachi;

        menu->characterList = hachi;
        menu->currentCharacter = hachi;
    }
}

void MainMenu_Unload(Menu* menu)
{
    if (menu->characterList != NULL) {
        CharacterNode* current = menu->characterList;
        CharacterNode* head = menu->characterList;
        bool firstNode = true;

        do {
            CharacterNode* next = current->next;
            UnloadTexture(current->texture);
            free(current);
            current = next;

            if (!firstNode && current == head) break;
            firstNode = false;
        } while (current != NULL);
    }

    UnloadTexture(menu->background_texture);
    UnloadTexture(menu->startButton_texture);
    UnloadTexture(menu->exitButton_texture);
    UnloadTexture(menu->keyBindings_texture);
    UnloadTexture(menu->keyBindingsGuide_texture);
    UnloadTexture(menu->enterName_texture);
    UnloadTexture(menu->selectCharacter_texture);
    UnloadTexture(menu->levelSelectionBackground_texture);
    UnloadTexture(menu->level1Panel_texture);
    UnloadTexture(menu->level2Panel_texture);
    UnloadTexture(menu->level3Panel_texture);
}

void MainMenu_UpdateStartButton(Menu* menu)
{
    if (CheckCollisionPointRec(GetMousePosition(), menu->startButton_position))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            menu->showNameInput = true;
        }

        DrawTexturePro(
            menu->startButton_texture,
            menu->startButton_rectangle,
            menu->startButton_position,
            (Vector2) {
            0, 0
        },
            0.0f,
            YELLOW
        );
    }
    else
    {
        DrawTexturePro(
            menu->startButton_texture,
            menu->startButton_rectangle,
            menu->startButton_position,
            (Vector2) {
            0, 0
        },
            0.0f,
            WHITE
        );
    }
}

void MainMenu_UpdateKeyBindingsButton(Menu* menu)
{
    if (CheckCollisionPointRec(GetMousePosition(), menu->keyBindings_position))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            menu->showKeyBindings = true;
        }

        DrawTexturePro(
            menu->keyBindings_texture,
            menu->keyBindings_rectangle,
            menu->keyBindings_position,
            (Vector2) {
            0, 0
        },
            0.0f,
            YELLOW
        );
    }
    else
    {
        DrawTexturePro(
            menu->keyBindings_texture,
            menu->keyBindings_rectangle,
            menu->keyBindings_position,
            (Vector2) {
            0, 0
        },
            0.0f,
            WHITE
        );
    }
}

void MainMenu_UpdateExitButton(Menu* menu)
{
    if (CheckCollisionPointRec(GetMousePosition(), menu->exitButton_position))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            menu->exit_Game = true;
        }

        DrawTexturePro(
            menu->exitButton_texture,
            menu->exitButton_rectangle,
            menu->exitButton_position,
            (Vector2) {
            0, 0
        },
            0.0f,
            YELLOW
        );
    }
    else
    {
        DrawTexturePro(
            menu->exitButton_texture,
            menu->exitButton_rectangle,
            menu->exitButton_position,
            (Vector2) {
            0, 0
        },
            0.0f,
            WHITE
        );
    }
}
void MainMenu_ShowNameInput(Menu* menu)
{
    DrawTexturePro(
        menu->enterName_texture,
        (Rectangle) {
        0, 0, menu->enterName_texture.width, menu->enterName_texture.height
    },
        (Rectangle) {
        0, 0, GetScreenWidth(), GetScreenHeight()
    },
        (Vector2) {
        0, 0
    },
        0.0f,
        WHITE
    );
    DrawText("Enter Your Display Name",
        GetScreenWidth() / 2 - MeasureText("Enter Your Display Name", 40) / 2,
        450,
        40,
        WHITE);

    Color boxColor = menu->nameInputActive ? SKYBLUE : LIGHTGRAY;
    DrawRectangleRec(menu->nameInputBox, boxColor);
    DrawRectangleLinesEx(menu->nameInputBox, 2, menu->nameInputActive ? BLUE : GRAY);

    DrawText("Play as:",
        menu->nameInputBox.x - MeasureText("Play as:", 30) - 20,
        530,
        30,
        WHITE);

    DrawText(menu->playerName,
        menu->nameInputBox.x + 10,
        535,
        20,
        BLACK);

    if (menu->nameInputActive) {
        DrawText("_",
            menu->nameInputBox.x + 10 + MeasureText(menu->playerName, 20),
            540,
            20,
            BLACK);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        menu->nameInputActive = CheckCollisionPointRec(GetMousePosition(), menu->nameInputBox);
    }

    if (menu->nameInputActive) {
        int key = GetCharPressed();

        if (IsKeyPressed(KEY_BACKSPACE) && menu->nameLength > 0) {
            menu->nameLength--;
            menu->playerName[menu->nameLength] = '\0';
        }

        if (key > 0 && menu->nameLength < 300) {
            if ((key >= 32 && key <= 125) || key == 95) {
                menu->playerName[menu->nameLength] = (char)key;
                menu->nameLength++;
                menu->playerName[menu->nameLength] = '\0';
            }
        }
    }

    Rectangle continueButton = { GetScreenWidth() / 2 - 100, menu->nameInputBox.y + 70, 200, 30 };
    bool overContinue = CheckCollisionPointRec(GetMousePosition(), continueButton);

    DrawRectangleRec(continueButton, overContinue ? SKYBLUE : BLUE);
    DrawText("Continue",
        continueButton.x + continueButton.width / 2 - MeasureText("Continue", 20) / 2,
        continueButton.y + continueButton.height / 2 - 10,
        20,
        WHITE);

    if (overContinue && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (menu->nameLength > 0) {
            int level = GetOrCreatePlayerLevel(menu->playerName);
            menu->currentLevel = level;
            menu->showCharacterSelection = true;
        }
    }

    DrawText("Press B to go back",
        GetScreenWidth() - 300,
        GetScreenHeight() - 60,
        20,
        WHITE);

    if (IsKeyPressed(KEY_B)) {
        menu->showNameInput = false;
    }
}

void MainMenu_LandingPage(Menu* menu)
{
    DrawTexturePro(
        menu->background_texture,
        (Rectangle) {
        0, 0, menu->background_texture.width, menu->background_texture.height
    },
        (Rectangle) {
        0, 0, GetScreenWidth(), GetScreenHeight()
    },
        (Vector2) {
        0, 0
    },
        0.0f,
        WHITE
    );

    MainMenu_UpdateStartButton(menu);
    MainMenu_UpdateKeyBindingsButton(menu);
    MainMenu_UpdateExitButton(menu);
}

void MainMenu_ShowKeyBindings(Menu* menu)
{
    DrawTexturePro(
        menu->keyBindingsGuide_texture,
        (Rectangle) {
        0, 0, menu->keyBindingsGuide_texture.width, menu->keyBindingsGuide_texture.height
    },
        (Rectangle) {
        0, 0, GetScreenWidth(), GetScreenHeight()
    },
        (Vector2) {
        0, 0
    },
        0.0f,
        WHITE
    );

    DrawText("Press B to go back", 830, GetScreenHeight() - 100, 25, WHITE);

    if (IsKeyPressed(KEY_B))
    {
        menu->showKeyBindings = false;
    }
}

void MainMenu_Update(Menu* menu)
{
    if (menu->showKeyBindings)
    {
        MainMenu_ShowKeyBindings(menu);
    }
    else if (menu->showNameInput)
    {
        MainMenu_ShowNameInput(menu);
    }
    else if (menu->showCharacterSelection)
    {
        MainMenu_SelectingACharacter(menu);
    }
    else if (menu->showLevelSelection)
    {
        MainMenu_ShowLevelSelection(menu);
    }
    else
    {
        MainMenu_LandingPage(menu);
    }

    if (IsKeyPressed(KEY_ESCAPE) && menu->start_Game)
    {
        menu->pause_Game = !menu->pause_Game;
    }
}

int GetOrCreatePlayerLevel(const char* playerName) {
    /*char filename[128];
    snprintf(filename, sizeof(filename), "users/%s.txt", playerName);

    FILE* file = fopen(filename, "r");

    if (file) {
        int level;
        if (fscanf(file, "%d", &level) == 1) {
            fclose(file);
            return level;
        }
        else {
            fclose(file);
            return -1; \
        }
    }
    else {
        file = fopen(filename, "w");
        if (file) {
            fprintf(file, "-1\n");
            fclose(file);
        }
        return -1;
    }*/
    return -1;
}

void MainMenu_ShowLevelSelection(Menu* menu)
{
    DrawTexturePro(
        menu->levelSelectionBackground_texture,
        (Rectangle) {
        0, 0,
            menu->levelSelectionBackground_texture.width,
            menu->levelSelectionBackground_texture.height
    },
        (Rectangle) {
        0, 0, GetScreenWidth(), GetScreenHeight()
    },
        (Vector2) {
        0, 0
    },
        0.0f,
        WHITE
    );

    DrawText("SELECT LEVEL",
        GetScreenWidth() / 2 - MeasureText("SELECT LEVEL", 50) / 2,
        100,
        50,
        WHITE);

    const int panelWidth = 800;
    const int panelHeight = 150;
    const int spacing = 40;

    int totalHeight = panelHeight * 3 + spacing * 2;
    int startY = GetScreenHeight() / 2 - totalHeight / 2;
    int x = GetScreenWidth() / 2 - panelWidth / 2;

    for (int i = 0; i < 3; i++)
    {
        Rectangle panel = {
            x,
            startY + i * (panelHeight + spacing),
            panelWidth,
            panelHeight
        };

        bool levelUnlocked = true;
        if (i > 0) {
            levelUnlocked = (menu->currentLevel >= i);
        }

        Color panelColor = levelUnlocked ? RAYWHITE : DARKGRAY;
        Color textColor = levelUnlocked ? BLACK : GRAY;

        DrawRectangleRec(panel, panelColor);
        DrawRectangleLinesEx(panel, 4, levelUnlocked ? GOLD : GRAY);

        char label[32];
        snprintf(label, sizeof(label), "Level %d", i + 1);

        DrawText(label,
            panel.x + 30,
            panel.y + panel.height / 2 - 15,
            40,
            textColor);

        const char* descriptions[] = {
            "Forest Adventure - Start your journey!",
            "Desert Challenge - Face new obstacles!",
            "Mountain Peak - The final test!"
        };

        DrawText(descriptions[i],
            panel.x + 200,
            panel.y + panel.height / 2 - 15,
            20,
            textColor);

        if (!levelUnlocked) {
            DrawText("LOCKED",
                panel.x + panel.width - 150,
                panel.y + panel.height / 2 - 15,
                30,
                RED);
        }

        if (levelUnlocked &&
            CheckCollisionPointRec(GetMousePosition(), panel) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            menu->currentLevel = i;
            menu->showNameInput = true;
            menu->showLevelSelection = false;
        }
    }

    DrawText("Press B to go back",
        GetScreenWidth() - 300,
        GetScreenHeight() - 60,
        20,
        WHITE);

    if (IsKeyPressed(KEY_B)) {
        menu->showLevelSelection = false;
    }
}

void MainMenu_SelectingACharacter(Menu* menu)
{
    DrawTexturePro(
        menu->selectCharacter_texture,
        (Rectangle) {
        0, 0,
            menu->selectCharacter_texture.width,
            menu->selectCharacter_texture.height
    },
        (Rectangle) {
        0, 0, GetScreenWidth(), GetScreenHeight()
    },
        (Vector2) {
        0, 0
    },
        0.0f,
        WHITE
    );

    DrawText("SELECT YOUR CHARACTER",
        GetScreenWidth() / 2 - MeasureText("SELECT YOUR CHARACTER", 50) / 2,
        100,
        50,
        WHITE);

    if (menu->currentCharacter != NULL) {
        int previewWidth = 300;
        int previewHeight = 300;
        int previewX = GetScreenWidth() / 2 - previewWidth / 2;
        int previewY = GetScreenHeight() / 2 - previewHeight / 2 - 50;

        if (menu->currentCharacter->texture.id > 0) {
            DrawTexturePro(
                menu->currentCharacter->texture,
                (Rectangle) {
                0, 0, menu->currentCharacter->texture.width, menu->currentCharacter->texture.height
            },
                (Rectangle) {
                previewX, previewY, previewWidth, previewHeight
            },
                (Vector2) {
                0, 0
            },
                0.0f,
                WHITE
            );
        }
        else {
            DrawRectangle(previewX, previewY, previewWidth, previewHeight, LIGHTGRAY);
            DrawText("Character Preview",
                previewX + previewWidth / 2 - MeasureText("Character Preview", 20) / 2,
                previewY + previewHeight / 2 - 10,
                20,
                DARKGRAY);
        }

        DrawText(menu->currentCharacter->name,
            GetScreenWidth() / 2 - MeasureText(menu->currentCharacter->name, 40) / 2,
            previewY + previewHeight + 20,
            40,
            WHITE);

        DrawText(menu->currentCharacter->description,
            GetScreenWidth() / 2 - MeasureText(menu->currentCharacter->description, 20) / 2,
            previewY + previewHeight + 70,
            20,
            LIGHTGRAY);
    }

    DrawText("Press LEFT/RIGHT to browse characters",
        GetScreenWidth() / 2 - MeasureText("Press LEFT/RIGHT to browse characters", 20) / 2,
        GetScreenHeight() - 130,
        20,
        WHITE);

    DrawText("Press ENTER to select character",
        GetScreenWidth() / 2 - MeasureText("Press ENTER to select character", 20) / 2,
        GetScreenHeight() - 100,
        20,
        WHITE);

    DrawText("Or use shortcuts: H for Hachi, S for Sophie, Alt+B for Bubbles",
        GetScreenWidth() / 2 - MeasureText("Or use shortcuts: H for Hachi, S for Sophie, Alt+B for Bubbles", 20) / 2,
        GetScreenHeight() - 70,
        20,
        WHITE);

    DrawText("Press B to go back",
        GetScreenWidth() / 2 - MeasureText("Press B to go back", 20) / 2,
        GetScreenHeight() - 40,
        20,
        WHITE);

    if (IsKeyPressed(KEY_RIGHT) && menu->currentCharacter != NULL) {
        menu->currentCharacter = menu->currentCharacter->next;
    }

    if (IsKeyPressed(KEY_LEFT) && menu->currentCharacter != NULL) {
        CharacterNode* prev = menu->currentCharacter;
        while (prev->next != menu->currentCharacter) {
            prev = prev->next;
        }
        menu->currentCharacter = prev;
    }

    if (IsKeyPressed(KEY_H)) {
        menu->selectedCharacter = PLAYER_HACHI;
        menu->start_Game = true;
        menu->showCharacterSelection = false;
    }
    else if (IsKeyPressed(KEY_S)) {
        menu->selectedCharacter = PLAYER_SOPHIE;
        menu->start_Game = true;
        menu->showCharacterSelection = false;
    }
    else if (IsKeyPressed(KEY_B) && !IsKeyDown(KEY_LEFT_ALT) && !IsKeyDown(KEY_RIGHT_ALT)) {
        menu->showCharacterSelection = false;
    }
    else if (IsKeyPressed(KEY_B) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {
        menu->selectedCharacter = PLAYER_BUBBLES;
        menu->start_Game = true;
        menu->showCharacterSelection = false;
    }

    if (IsKeyPressed(KEY_ENTER) && menu->currentCharacter != NULL) {
        menu->selectedCharacter = menu->currentCharacter->type;
        menu->start_Game = true;
        menu->showCharacterSelection = false;
    }
}