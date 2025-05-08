#include "raylib.h"

#include "Menu.h"

#include <stdio.h>

#include "Player.h"
#include "Enemies.h"
#include "Animation.h"
#include "Level.h"

typedef enum GameState {

    STATE_NAME_INPUT,

    STATE_CHARACTER_SELECT,

    STATE_MAIN_MENU,

    STATE_LEVEL_SELECT,

    STATE_GAME,

    STATE_EXIT

} GameState;

int main(void)

{

    const int screenWidth = 1280;
    const int screenHeight = 720;

    /*SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(screenWidth, screenHeight, "My Game");

    SetTargetFPS(60);
    ToggleFullscreen();*/

    InitWindow(0, 0, "Hachi's World");
    ToggleFullscreen();
    InitAudioDevice();
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    // Start with name input

    GameState currentState = STATE_NAME_INPUT;

    Menu menu;

    MainMenu_Init(&menu);

    menu.showNameInput = true;

    Player player;
    Level* currentLevel = NULL;
    Enemies enemies;
    Animation animation;


    while (!WindowShouldClose() && currentState != STATE_EXIT)

    {

        BeginDrawing();

        ClearBackground(BLACK);

        switch (currentState)

        {

        case STATE_NAME_INPUT:

            MainMenu_ShowNameInput(&menu);

            // Check if user completed name input and pressed continue

            if (menu.showCharacterSelection) {

                // User has entered name and pressed continue

                currentState = STATE_CHARACTER_SELECT;

                menu.showNameInput = false;

            }

            else if (IsKeyPressed(KEY_B)) {

                // Go back to main menu if B is pressed

                menu.showNameInput = false;

                currentState = STATE_MAIN_MENU;

            }

            break;

        case STATE_CHARACTER_SELECT:

            // Show character selection UI

            MainMenu_SelectingACharacter(&menu);

            if (!menu.showCharacterSelection) {

                if (menu.start_Game) {

                    // If character was selected, go to main menu

                    menu.start_Game = false;

                    currentState = STATE_MAIN_MENU;

                }

                else {

                    // If user pressed B to go back, return to name input

                    menu.showNameInput = true;

                    currentState = STATE_NAME_INPUT;

                }

            }

            break;

        case STATE_MAIN_MENU:

            MainMenu_LandingPage(&menu);

            // Check if start button was clicked

            if (CheckCollisionPointRec(GetMousePosition(), menu.startButton_position) &&

                IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

                currentState = STATE_LEVEL_SELECT;

                menu.showLevelSelection = true;

            }

            // Check for key bindings display

            if (menu.showKeyBindings) {

                MainMenu_ShowKeyBindings(&menu);

            }

            // Check if exit button was clicked

            if (menu.exit_Game) {

                currentState = STATE_EXIT;

            }

            break;

        case STATE_LEVEL_SELECT:

            MainMenu_ShowLevelSelection(&menu);

            // Navigation options

            DrawText("Press B to go back to menu",

                screenWidth / 2 - MeasureText("Press B to go back to menu", 20) / 2,

                screenHeight - 50, 20, WHITE);

            if (IsKeyPressed(KEY_B)) {

                menu.showLevelSelection = false;

                currentState = STATE_MAIN_MENU;

            }

            // Check if a level was selected

            if (menu.currentLevel >= 0 && !menu.showLevelSelection) {

                Player_Init(&player, PLAYER_HACHI);
                Enemies_Init(&enemies);
                currentLevel = Level_Create(menu.currentLevel + 1); // +1 because levels are 1-indexed in Level_Create

                if (currentLevel == NULL) {
                    // Handle level creation failure
                    TraceLog(LOG_ERROR, "Failed to create level %d", menu.currentLevel + 1);
                    currentState = STATE_MAIN_MENU;
                }
                else {
                    // Level created successfully, proceed to game
                    currentState = STATE_GAME;
                }


            }

            break;

        case STATE_GAME:
            if (currentLevel != NULL) {
                // Initialize level if not started
                if (!currentLevel->startPlay) {
                    Level_Init(currentLevel);
                    Player_SetPos1(&player);
                    Enemies_Reset(&enemies);
                    currentLevel->levelCompleted = false;

                    switch (menu.currentLevel) {
                    case 0:
                        SetMusicVolume(currentLevel->levelOne_music, currentLevel->levelOne_volume);
                        PlayMusicStream(currentLevel->levelOne_music);
                        break;
                    case 1:
                        SetMusicVolume(currentLevel->levelTwo_music, currentLevel->levelTwo_volume);
                        PlayMusicStream(currentLevel->levelTwo_music);
                        break;
                    case 2:
                        SetMusicVolume(currentLevel->levelThree_music, currentLevel->levelThree_volume);
                        PlayMusicStream(currentLevel->levelThree_music);
                        break;
                    }

                    currentLevel->startPlay = true;
                }

                // Update audio stream
                switch (menu.currentLevel) {
                case 0: UpdateMusicStream(currentLevel->levelOne_music); break;
                case 1: UpdateMusicStream(currentLevel->levelTwo_music); break;
                case 2: UpdateMusicStream(currentLevel->levelThree_music); break;
                }

                player.camera.target = (Vector2){ player.animation.player_disp.x, player.animation.player_disp.y };

                // Begin game rendering
                BeginMode2D(player.camera);
                ClearBackground(SKYBLUE);

                Level_Draw(currentLevel, &player);
                Player_DrawUI(&player);

                if (!menu.pause_Game) {
                    Player_CheckInput(&player);
                    Player_ManageInput(&player);
                    Player_SetAudioVolume(&player);

                    // Manage enemies and level-specific mechanics
                    if (menu.currentLevel == 0) {
                        Enemies_ManageRadish(&enemies);
                        Enemies_ManageCassava(&enemies);
                        Level_CheckCollisions(currentLevel, &player, &enemies);

                        if (player.death) {
                            Level_ManageDeath(currentLevel, &player, &enemies);
                            if (player.hp <= 0) {
                                Player_SetPos1(&player);
                                Enemies_Reset(&enemies);
                                currentLevel->startPlay = false;
                                // Optionally return to level select or restart
                                currentState = STATE_LEVEL_SELECT;
                            }
                        }
                    }
                    else if (menu.currentLevel == 1) {
                        Enemies_ManageRadish(&enemies);
                        Enemies_ManageCassava(&enemies);
                        Level_CheckCollisions(currentLevel, &player, &enemies);

                        if (player.death) {
                            Level_ManageDeath(currentLevel, &player, &enemies);
                            if (player.hp <= 0) {
                                Player_SetPos1(&player);
                                Enemies_Reset(&enemies);
                                currentLevel->startPlay = false;
                                currentState = STATE_LEVEL_SELECT;
                            }
                        }
                    }
                    // Add Level 3 behavior similarly
                }


                EndMode2D();

                // Debug info
                DrawText(TextFormat("Level: %d", menu.currentLevel + 1), 10, 10, 20, WHITE);
                DrawText(TextFormat("Level Status: %s", currentLevel->startPlay ? "Playing" : "Starting"), 10, 30, 20, WHITE);

                if (currentLevel->levelCompleted) {
                    PlaySound(currentLevel->levelcompleted_sound);
                    currentLevel->levelCompleted = false;
                    currentState = STATE_LEVEL_SELECT;
                }

                if (currentLevel->game_over) {
                    PlaySound(currentLevel->gameover_sound);
                    currentLevel->game_over = false;
                    currentState = STATE_LEVEL_SELECT;
                }
            }
            else {
                DrawText("ERROR: Level is NULL", screenWidth / 2 - 100, screenHeight / 2, 20, RED);
            }

            // Exit to main menu
            if (IsKeyPressed(KEY_ESCAPE)) {
                if (currentLevel != NULL) {
                    switch (menu.currentLevel) {
                    case 0: StopMusicStream(currentLevel->levelOne_music); break;
                    case 1: StopMusicStream(currentLevel->levelTwo_music); break;
                    case 2: StopMusicStream(currentLevel->levelThree_music); break;
                    }
                    free(currentLevel);
                    currentLevel = NULL;
                }
                currentState = STATE_MAIN_MENU;
            }
            break;


        default:

            break;

        }

        EndDrawing();

    }

    MainMenu_Unload(&menu);

    CloseWindow();

    return 0;

}

