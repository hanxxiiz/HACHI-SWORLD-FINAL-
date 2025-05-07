#include "raylib.h"

#include "Menu.h"

#include <stdio.h>

#include "Player.h"
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
            // Update the level
            if (currentLevel != NULL) {
                Player_CheckInput(&player);
                Player_ManageInput(&player);

                // Start level music if not already playing
                if (!currentLevel->startPlay) {
                    switch (menu.currentLevel) {
                    case 0: // Level 1
                        
                        SetMusicVolume(currentLevel->levelOne_music, currentLevel->levelOne_volume);
                        PlayMusicStream(currentLevel->levelOne_music);
                        break;
                    case 1: // Level 2
                        SetMusicVolume(currentLevel->levelTwo_music, currentLevel->levelTwo_volume);
                        PlayMusicStream(currentLevel->levelTwo_music);
                        break;
                    case 2: // Level 3
                        SetMusicVolume(currentLevel->levelThree_music, currentLevel->levelThree_volume);
                        PlayMusicStream(currentLevel->levelThree_music);
                        break;
                    }
                    currentLevel->startPlay = true;
                }

                Player_SetPos1(&player);

                // Update music stream
                switch (menu.currentLevel) {
                case 0: UpdateMusicStream(currentLevel->levelOne_music); break;
                case 1: UpdateMusicStream(currentLevel->levelTwo_music); break;
                case 2: UpdateMusicStream(currentLevel->levelThree_music); break;
                }

                BeginMode2D(player.camera);
                
                // Render level
                Level_Init(currentLevel);
                Level_DrawTiles(currentLevel);
                Level_Draw(currentLevel, &player);

                DrawTexturePro(
                    player.hasTurned_right ?
                    player.animation->player_texture_idle :
                    player.animation->player_texture_idle,
                    player.hasTurned_right ?
                    player.animation->player_src_right :
                    player.animation->player_src_left,
                    player.animation->player_disp,
                    player.origin,
                    0.0f,
                    WHITE
                );

                EndMode2D();

                Player_SetAudioVolume(&player);
                Player_DrawUI(&player);

                // Debug info - helps track what's happening
                DrawText(TextFormat("Level: %d", menu.currentLevel + 1), 10, 10, 20, WHITE);
                DrawText(TextFormat("Level Status: %s", currentLevel->startPlay ? "Playing" : "Starting"), 10, 30, 20, WHITE);


                // Check if level is completed
                if (currentLevel->levelCompleted) {
                    // Handle level completion
                    PlaySound(currentLevel->levelcompleted_sound);
                    // Save progress if needed
                    currentState = STATE_LEVEL_SELECT;
                    currentLevel->levelCompleted = false;
                }

                // Check if game over
                if (currentLevel->game_over) {
                    PlaySound(currentLevel->gameover_sound);
                    currentState = STATE_LEVEL_SELECT;
                    currentLevel->game_over = false;
                }
            }
            else {
                // Debug message if level is NULL
                DrawText("ERROR: Level is NULL", screenWidth / 2 - 100, screenHeight / 2, 20, RED);
            }

            // Check for return to menu
            if (IsKeyPressed(KEY_ESCAPE)) {
                if (currentLevel != NULL) {
                    // Stop music based on level
                    switch (menu.currentLevel) {
                    case 0: StopMusicStream(currentLevel->levelOne_music); break;
                    case 1: StopMusicStream(currentLevel->levelTwo_music); break;
                    case 2: StopMusicStream(currentLevel->levelThree_music); break;
                    }

                    // Free the level resources
                    //Level_Unload(currentLevel);
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

