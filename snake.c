#include "raylib.h"
#include "stdio.h"
#include <stdlib.h>

// Pages
#include "./pages/ScoreWindow.h"
// #include "./pages/OptionsWindow.h"

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define SNAKE_LENGTH 256

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

// typedef struct Snake Snake;
// typedef struct Food Food;
// typedef struct ScreenSettings ScreenSettings;

typedef struct Snake
{
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color color;
    int fruitsCounter;
    int highestScore;
    int counterTail;
    bool allowMove;
} Snake;

typedef struct Food
{
    Vector2 position;
    Vector2 size;
    bool active;
    Color color;
} Food;

typedef struct ScreenSettings
{
    Vector2 offset;
    int screenWidth;
    int screenHeight;
    int framesCounter;
    int optionSelect;
    int squareSize;
    char currentScreen;
    bool gameOver;
    bool pause;
    bool exitGame;
} ScreenSettings;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static Snake snake[SNAKE_LENGTH] = {0};
static Vector2 snakePosition[SNAKE_LENGTH] = {0};

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static char menu(void);                                                                   // Initialize menu and select options
static void InitScoreGame(ScreenSettings *configs, Food *fruit);                          //, Snake snake[SNAKE_LENGTH]);     // Initialize game
static void UpdateGame(ScreenSettings *configs, Food *fruit, Texture2D snakeBackTexture); //, Snake snake[SNAKE_LENGTH]);        // Update game (one frame)
static void UnloadGame(Music backgroundSound);                                            // Unload game
void getHighestScore();
void saveScore();
void dificulty(ScreenSettings *configs);
void TitleWindow(ScreenSettings *configs, Texture2D snakeBackTexture);
void DificultyWindow(ScreenSettings *configs);
void GameWindow(ScreenSettings *configs, Food *fruit, Texture2D backTexture, Texture2D iconTexture);
void InitSound(Music backgroundSound);
void menuControlls(ScreenSettings *configs);
void singlePlayerControlls(ScreenSettings *configs); //, Snake **snake);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    ScreenSettings *configs = malloc(sizeof(ScreenSettings));
    configs->screenWidth = 800;  // GetScreenWidth();
    configs->screenHeight = 600; // GetScreenHeight();
    configs->framesCounter = 0;
    configs->pause = false;
    configs->gameOver = false;
    configs->currentScreen = 'T'; // L - Logo; T - Title; M - Mode selector; G - Gameplay; O - Options; S - Score;
    configs->optionSelect = 0;
    configs->squareSize = 31;
    configs->offset;

    Food *fruit = malloc(sizeof(Food));

    Music backgroundSound;

    InitWindow(configs->screenWidth, configs->screenHeight, "Snake Game");
    InitScoreGame(configs, fruit);
    SetTargetFPS(60); // 44 - easy; 60 - normal; 144 - hard;

    // LOAD TUDO :) ----------------------

    // BackGround
    Image back = LoadImage("resources/images/back.gif");
    Texture2D backTexture = LoadTextureFromImage(back);

    // Fruit
    Image icon = LoadImage("resources/images/iconFruit.gif");
    Texture2D iconTexture = LoadTextureFromImage(icon);

    // Snake Title BackGround
    Image snakeBack = LoadImage("resources/images/SnakeBackground.gif");
    Texture2D snakeBackimgTexture = LoadTextureFromImage(snakeBack);

    // Sound
    InitSound(backgroundSound);

    // Score
    getHighestScore();

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose() && !configs->exitGame) // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        // InitWindow(configs->screenWidth, configs->screenHeight, "Snake Game");

        UpdateMusicStream(backgroundSound);

        switch ((configs)->currentScreen)
        {
        case 'T':
            menuControlls(configs);
            TitleWindow(configs, snakeBackimgTexture);
            break;
        case 'D':
            dificulty(configs);
            DificultyWindow(configs);
            break;
        case 'G':
            UpdateGame(configs, fruit, snakeBackimgTexture);
            GameWindow(configs, fruit, backTexture, iconTexture);
            break;
        case 'S':
            // ScoreWindow(0);
            break;
        case 'O':
            // OptionsWindow();
            break;
        default:
            TitleWindow(configs, snakeBackimgTexture);
            break;
        }

        if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER))
        {
            configs->screenWidth = GetScreenWidth();
            configs->screenHeight = GetScreenHeight();
            ToggleFullscreen();
        }

        if (IsCursorOnScreen())
        {
            HideCursor();
        }
        else
        {
            ShowCursor();
        }
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    saveScore();
    UnloadGame(backgroundSound); // Unload loaded data (textures, sounds, models...)
    CloseWindow();               // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitScoreGame(ScreenSettings *configs, Food *fruit) //, Snake snake[SNAKE_LENGTH])
{
    configs->framesCounter = 0;
    configs->gameOver = false;
    configs->pause = false;

    snake->counterTail = 1;
    snake->allowMove = false;
    snake->fruitsCounter = 0;

    configs->offset.x = configs->screenWidth % configs->squareSize;
    configs->offset.y = configs->screenHeight % configs->squareSize;

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snake[i].position = (Vector2){configs->offset.x / 2, configs->offset.y / 2};
        snake[i].size = (Vector2){configs->squareSize, configs->squareSize};
        snake[i].speed = (Vector2){configs->squareSize, 0};

        if (i == 0)
        {
            snake[i].color = DARKGREEN;
        }
        else
        {
            snake[i].color = GREEN;
        }
    }

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snakePosition[i] = (Vector2){0.0f, 0.0f};
    }

    fruit->size = (Vector2){configs->squareSize, configs->squareSize};
    fruit->color = SKYBLUE;
    fruit->active = false;

    SetExitKey(KEY_F10);
}

// Update game (one frame)
void UpdateGame(ScreenSettings *configs, Food *fruit, Texture2D snakeBackTexture) //, Snake snake[SNAKE_LENGTH])
{
    if (configs->currentScreen == 'T')
    {
        menuControlls(configs);
    }
    else if (configs->currentScreen == 'D')
    {
        dificulty(configs);
    }
    else if (configs->currentScreen == 'G')
    {
        if (!configs->gameOver)
        {
            if (IsKeyPressed(KEY_ESCAPE))
                configs->pause = !configs->pause;

            if (!configs->pause)
            {
                // Player control
                singlePlayerControlls(configs);

                // Snake movement
                for (int i = 0; i < snake->counterTail; i++)
                    snakePosition[i] = snake[i].position;

                if ((configs->framesCounter % 5) == 0)
                {
                    for (int i = 0; i < snake->counterTail; i++)
                    {
                        if (i == 0)
                        {
                            snake[0].position.x += snake[0].speed.x;
                            snake[0].position.y += snake[0].speed.y;
                            snake->allowMove = true;
                        }
                        else
                            snake[i].position = snakePosition[i - 1];
                    }
                }

                // Wall behaviour
                if (((snake[0].position.x) > (configs->screenWidth - configs->offset.x)) ||
                    ((snake[0].position.y) > (configs->screenHeight - configs->offset.y)) ||
                    (snake[0].position.x < 0) || (snake[0].position.y < 0))
                {
                    configs->gameOver = true;
                }

                // Collision with yourself
                for (int i = 1; i < snake->counterTail; i++)
                {
                    if ((snake[0].position.x == snake[i].position.x) && (snake[0].position.y == snake[i].position.y))
                        configs->gameOver = true;
                }

                // Fruit position calculation
                if (!fruit->active)
                {
                    fruit->active = true;
                    fruit->position = (Vector2){GetRandomValue(0, (configs->screenWidth / configs->squareSize) - 1) * configs->squareSize + configs->offset.x / 2, GetRandomValue(0, (configs->screenHeight / configs->squareSize) - 1) * configs->squareSize + configs->offset.y / 2};

                    for (int i = 0; i < snake->counterTail; i++)
                    {
                        while ((fruit->position.x == snake[i].position.x) && (fruit->position.y == snake[i].position.y))
                        {
                            fruit->position = (Vector2){GetRandomValue(0, (configs->screenWidth / configs->squareSize) - 1) * configs->squareSize + configs->offset.x / 2, GetRandomValue(0, (configs->screenHeight / configs->squareSize) - 1) * configs->squareSize + configs->offset.y / 2};
                            i = 0;
                        }
                    }
                }

                // Collision
                if ((snake[0].position.x < (fruit->position.x + fruit->size.x) && (snake[0].position.x + snake[0].size.x) > fruit->position.x) &&
                    (snake[0].position.y < (fruit->position.y + fruit->size.y) && (snake[0].position.y + snake[0].size.y) > fruit->position.y))
                {
                    snake->fruitsCounter += 1;
                    snake[snake->counterTail].position = snakePosition[snake->counterTail - 1];
                    snake->counterTail += 1;
                    fruit->active = false;
                }

                configs->framesCounter++;
            }
        }
        else
        {
            if (snake->highestScore < snake->fruitsCounter * 100)
            {
                snake->highestScore = snake->fruitsCounter * 100;
            }
            if (IsKeyPressed(KEY_S))
            {
                saveScore();
            }
            if (IsKeyPressed(KEY_ENTER))
            {
                InitScoreGame(configs, fruit);
                configs->gameOver = false;
            }
            if (IsKeyPressed(KEY_G))
            {
                OpenURL("https://github.com/astuno/UPE---LP2Game");
            }
            if (IsKeyPressed(KEY_X))
            {
                configs->exitGame = true;
            }
        }
    }
}

// Unload game variables
void UnloadGame(Music backgroundSound)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
    UnloadMusicStream(backgroundSound);
    CloseAudioDevice();
    // UnloadFont(font);
}

void menuControlls(ScreenSettings *configs)
{
    if (IsKeyPressed(KEY_W))
    {
        configs->optionSelect--;
    }
    else if (IsKeyPressed(KEY_S))
    {
        configs->optionSelect++;
    }

    if (configs->optionSelect < 0)
        configs->optionSelect = 0;
    else if (configs->optionSelect > 1)
        configs->optionSelect = 1;

    if (IsKeyPressed(KEY_ENTER))
    {
        switch (configs->optionSelect)
        {
        case 0:
            configs->currentScreen = 'D';
            break;
        // case 1:
        //     configs->currentScreen = 'O';
        //     break;
        case 1:
            configs->exitGame = true;
            break;
        default:
            break;
        }
    }
}

void singlePlayerControlls(ScreenSettings *configs) //, Snake **snake)
{
    if (IsKeyPressed(KEY_D) && (snake[0].speed.x == 0) && snake->allowMove)
    {
        snake[0].speed = (Vector2){configs->squareSize, 0};
        snake->allowMove = false;
    }
    if (IsKeyPressed(KEY_A) && (snake[0].speed.x == 0) && snake->allowMove)
    {
        snake[0].speed = (Vector2){-configs->squareSize, 0};
        snake->allowMove = false;
    }
    if (IsKeyPressed(KEY_W) && (snake[0].speed.y == 0) && snake->allowMove)
    {
        snake[0].speed = (Vector2){0, -configs->squareSize};
        snake->allowMove = false;
    }
    if (IsKeyPressed(KEY_S) && (snake[0].speed.y == 0) && snake->allowMove)
    {
        snake[0].speed = (Vector2){0, configs->squareSize};
        snake->allowMove = false;
    }
}

void TitleWindow(ScreenSettings *configs, Texture2D snakeBackTexture)
{
    BeginDrawing();
    ClearBackground(DARKGREEN);
    // IMAGE
    DrawTexture(snakeBackTexture, GetScreenWidth() / 2 + 100, 100, WHITE);
    DrawText("SNAKE GAME", GetScreenWidth() / 2 - MeasureText("SNAKE GAME", 40) / 2, 150, 40, BLACK);
    DrawText("START", GetScreenWidth() / 2 - MeasureText("START", 20) / 2, GetScreenHeight() / 2 + 100, 20, (configs->optionSelect == 0) ? WHITE : BLACK);
    // DrawText("OPTIONS", GetScreenWidth() / 2 - MeasureText("OPTIONS", 20) / 2, GetScreenHeight() / 2 + 150, 20, (configs->optionSelect == 1) ? WHITE : BLACK);
    DrawText("EXIT", GetScreenWidth() / 2 - MeasureText("EXIT", 20) / 2, GetScreenHeight() / 2 + 150, 20, (configs->optionSelect == 1) ? WHITE : BLACK);
    EndDrawing();
}

void DificultyWindow(ScreenSettings *configs)
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("SELECT YOUR DIFICULTY LEVEL", GetScreenWidth() / 2 - MeasureText("SELECT YOUR DIFICULTY LEVEL", 40) / 2, 150, 40, BLACK);
    DrawText("EASY", GetScreenWidth() / 2 - MeasureText("EASY", 20) / 2, GetScreenHeight() / 2 + 100, 20, (configs->optionSelect == 0) ? WHITE : GRAY);
    DrawText("MEDIUM", GetScreenWidth() / 2 - MeasureText("MEDIUM", 20) / 2, GetScreenHeight() / 2 + 150, 20, (configs->optionSelect == 1) ? WHITE : GRAY);
    DrawText("HARD", GetScreenWidth() / 2 - MeasureText("HARD", 20) / 2, GetScreenHeight() / 2 + 200, 20, (configs->optionSelect == 2) ? WHITE : GRAY);
    EndDrawing();
}

void GameWindow(ScreenSettings *configs, Food *fruit, Texture2D backTexture, Texture2D iconTexture)
{

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTextureV(backTexture, (Vector2){0, 0}, WHITE);
    if (!configs->gameOver)
    {
        // Draw BackGround
        // TODO

        // Draw grid lines
        for (int i = 0; i < configs->screenWidth / configs->squareSize + 1; i++)
        {
            DrawLineV((Vector2){configs->squareSize * i + configs->offset.x / 2, configs->offset.y / 2}, (Vector2){configs->squareSize * i + configs->offset.x / 2, configs->screenHeight - configs->offset.y / 2}, LIGHTGRAY);
        }

        for (int i = 0; i < configs->screenHeight / configs->squareSize + 1; i++)
        {
            DrawLineV((Vector2){configs->offset.x / 2, configs->squareSize * i + configs->offset.y / 2}, (Vector2){configs->screenWidth - configs->offset.x / 2, configs->squareSize * i + configs->offset.y / 2}, LIGHTGRAY);
        }

        // Draw snake
        for (int i = 0; i < snake->counterTail; i++)
            DrawRectangleV(snake[i].position, snake[i].size, snake[i].color);

        // Draw fruit to pick
        DrawTextureV(iconTexture, fruit->position, WHITE);

        if (configs->pause)
        {
            DrawRectangle(70, 95, 180, 27, DARKGRAY);
            DrawText(TextFormat("Fruits eated: %i", snake->fruitsCounter), 76, 100, 20, GREEN);
            DrawText("GAME PAUSED", configs->screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, configs->screenHeight / 2 - 40, 40, GRAY);
        }
    }
    else
    {
        DrawText(TextFormat("You atual score was: %i", snake->fruitsCounter * 100), 76, 100, 20, GREEN);
        DrawText(TextFormat("You highest score was: %i", snake->highestScore), 76, 140, 20, GREEN);
        DrawText("Press [S] to save your Score", GetScreenWidth() / 2 - MeasureText("Press [S] to save your Score", 20) / 2, GetScreenHeight() / 2 - 100, 20, GRAY);
        DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
        DrawText("Press [G] to check us in GitHub!", GetScreenWidth() / 2 - MeasureText("Press [G] to check us in GitHub!", 20) / 2, GetScreenHeight() / 2, 20, GRAY);
        DrawText("Press [X] to exit", GetScreenWidth() / 2 - MeasureText("Press [X] to exit", 20) / 2, GetScreenHeight() / 2 + 50, 20, GRAY);
    }
    EndDrawing();
}

void InitSound(Music backgroundSound)
{
    InitAudioDevice();
    backgroundSound = LoadMusicStream("./resources/sounds/02_A-Type Music (version 1.1).mp3");
    SetMasterVolume(0);
    PlayMusicStream(backgroundSound);
}

void getHighestScore()
{
    FILE *filePtr = fopen("score.txt", "r");

    if (filePtr != NULL)
    {
        fscanf(filePtr, "%i\n", &snake->highestScore);
    }
    else
    {
        snake->highestScore = 0;
    }
    fclose(filePtr);
}

void saveScore()
{
    FILE *filePtr = fopen("score.txt", "w+");

    fprintf(filePtr, "%d\n", snake->highestScore);

    fclose(filePtr);
}

void dificulty(ScreenSettings *configs)
{
    if (IsKeyPressed(KEY_W))
    {
        configs->optionSelect--;
    }
    else if (IsKeyPressed(KEY_S))
    {
        configs->optionSelect++;
    }

    if (configs->optionSelect < 0)
        configs->optionSelect = 0;
    else if (configs->optionSelect > 2)
        configs->optionSelect = 2;

    if (IsKeyPressed(KEY_ENTER))
    {
        switch (configs->optionSelect)
        {
        case 0:
            SetTargetFPS(44);
            configs->currentScreen = 'G';
            break;
        case 1:
            SetTargetFPS(60);
            configs->currentScreen = 'G';
            break;
        case 2:
            SetTargetFPS(144);
            configs->currentScreen = 'G';
            break;
        default:
            break;
        }
    }
}