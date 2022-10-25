#include "raylib.h"
#include "stdio.h"
#include <stdlib.h>

// Pages
//#include "./pages/ScoreWindow.h"
#include "./pages/OptionsWindow.h"

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define SNAKE_LENGTH 256
#define SQUARE_SIZE 31

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct Snake
{
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color color;
    int fruitsCounter;
    bool allowMove;
    int counterTail;
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
    int screenWidth;
    int screenHeight;
    int framesCounter;
    int optionSelect;
    char currentScreen;
    bool gameOver;
    bool pause;
    bool exitGame;
} ScreenSettings;

typedef struct ScoreData
{
    char *pos;
    char *name;
    char *score;
    struct ScoreData *next;
} ScoreData;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

// static Food fruit = {0};
static Snake snake[SNAKE_LENGTH] = {0};
static Vector2 snakePosition[SNAKE_LENGTH] = {0};
static Vector2 offset = {0};
static Music sound;
// static Font font = {0};
// static int SQUARE_SIZE = 31;
//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static char menu(void);                                                // Initialize menu and select options
static void InitScoreGame(ScreenSettings **configs, Food **fruit);     // Initialize game
static void InitSound(void);                                           // Init sound settings
static void UpdateGame(ScreenSettings **configs, Food **fruit);        // Update game (one frame)
static void DrawCurrentScreen(ScreenSettings **configs, Food **fruit); // Select and Draw the current screen
static void UnloadGame(void);                                          // Unload game
static void UpdateDrawFrame(ScreenSettings **configs, Food **fruit);   // Update and Draw (one frame)
static void GameWindow(ScreenSettings **configs, Food **fruit);
static void TitleWindow();
void ScoreWindow();
ScoreData *getData();

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

    Food *fruit = malloc(sizeof(Food));

    InitWindow(configs->screenWidth, configs->screenHeight, "Snake Game");
    // font = LoadFont("resources/alpha_beta.png");

    // ToggleFullscreen();
    InitScoreGame(&configs, &fruit);
    SetTargetFPS(60);
    InitSound();

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose() && !configs->exitGame) // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        // InitWindow(configs->screenWidth, configs->screenHeight, "Snake Game");

        UpdateMusicStream(sound);
        UpdateDrawFrame(&configs, &fruit);
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame();  // Unload loaded data (textures, sounds, models...)
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void GameWindow(ScreenSettings **configs, Food **fruit)
{

    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (!(*configs)->gameOver)
    {
        // Draw BackGround
        // TODO

        // Draw grid lines
        for (int i = 0; i < (*configs)->screenWidth / SQUARE_SIZE + 1; i++)
        {
            DrawLineV((Vector2){SQUARE_SIZE * i + offset.x / 2, offset.y / 2}, (Vector2){SQUARE_SIZE * i + offset.x / 2, (*configs)->screenHeight - offset.y / 2}, LIGHTGRAY);
        }

        for (int i = 0; i < (*configs)->screenHeight / SQUARE_SIZE + 1; i++)
        {
            DrawLineV((Vector2){offset.x / 2, SQUARE_SIZE * i + offset.y / 2}, (Vector2){(*configs)->screenWidth - offset.x / 2, SQUARE_SIZE * i + offset.y / 2}, LIGHTGRAY);
        }

        // Draw snake
        for (int i = 0; i < snake->counterTail; i++)
            DrawRectangleV(snake[i].position, snake[i].size, snake[i].color);

        // Draw fruit to pick
        DrawRectangleV((*fruit)->position, (*fruit)->size, (*fruit)->color);

        if ((*configs)->pause)
        {
            DrawRectangle(70, 95, 180, 27, DARKGRAY);
            DrawText(TextFormat("Fruits eated: %i", snake->fruitsCounter), 76, 100, 20, GREEN);
            DrawText("GAME PAUSED", (*configs)->screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, (*configs)->screenHeight / 2 - 40, 40, GRAY);
        }
    }
    else
    {
        DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
        DrawText("Press [G] to check us in GitHub!", GetScreenWidth() / 2 - MeasureText("Press [G] to check us in GitHub!", 20) / 2, GetScreenHeight() / 2, 20, GRAY);
    }
    EndDrawing();
}

void TitleWindow(ScreenSettings **configs)
{
    BeginDrawing();
    ClearBackground(DARKGREEN);
    // IMAGE
    Image img = LoadImage("resources/images/SnakeBackground.gif");
    Texture2D imgTexture = LoadTextureFromImage(img);
    DrawTexture(imgTexture, GetScreenWidth() / 2 + 300, 100, WHITE);
    DrawText("SNAKE GAME", GetScreenWidth() / 2 - MeasureText("SNAKE GAME", 40) / 2, 150, 40, BLACK);
    DrawText("START", GetScreenWidth() / 2 - MeasureText("START", 20) / 2, GetScreenHeight() / 2 + 100, 20, ((*configs)->optionSelect == 0) ? WHITE : BLACK);
    DrawText("SCORE", GetScreenWidth() / 2 - MeasureText("SCORE", 20) / 2, GetScreenHeight() / 2 + 150, 20, ((*configs)->optionSelect == 1) ? WHITE : BLACK);
    DrawText("OPTIONS", GetScreenWidth() / 2 - MeasureText("OPTIONS", 20) / 2, GetScreenHeight() / 2 + 200, 20, ((*configs)->optionSelect == 2) ? WHITE : BLACK);
    DrawText("EXIT", GetScreenWidth() / 2 - MeasureText("EXIT", 20) / 2, GetScreenHeight() / 2 + 250, 20, ((*configs)->optionSelect == 3) ? WHITE : BLACK);
    EndDrawing();
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitScoreGame(ScreenSettings **configs, Food **fruit)
{
    (*configs)->framesCounter = 0;
    (*configs)->gameOver = false;
    (*configs)->pause = false;

    snake->counterTail = 1;
    snake->allowMove = false;
    snake->fruitsCounter = 0;

    offset.x = (*configs)->screenWidth % SQUARE_SIZE;
    offset.y = (*configs)->screenHeight % SQUARE_SIZE;

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snake[i].position = (Vector2){offset.x / 2, offset.y / 2};
        snake[i].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
        snake[i].speed = (Vector2){SQUARE_SIZE, 0};

        if (i == 0)
            snake[i].color = DARKGREEN;
        else
            snake[i].color = GREEN;
    }

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snakePosition[i] = (Vector2){0.0f, 0.0f};
    }

    (*fruit)->size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    (*fruit)->color = SKYBLUE;
    (*fruit)->active = false;

    SetExitKey(KEY_F10);
}

// Update game (one frame)
void UpdateGame(ScreenSettings **configs, Food **fruit)
{
    if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER))
    {
        (*configs)->screenWidth = GetScreenWidth();
        (*configs)->screenHeight = GetScreenHeight();
        // SQUARE_SIZE = 63 ? SQUARE_SIZE == 31 : 31;
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

    if ((*configs)->currentScreen == 'T')
    {
        if (IsKeyPressed(KEY_W))
        {
            (*configs)->optionSelect--;
        }
        else if (IsKeyPressed(KEY_S))
        {
            (*configs)->optionSelect++;
        }

        if ((*configs)->optionSelect < 0)
            (*configs)->optionSelect = 0;
        else if ((*configs)->optionSelect > 3)
            (*configs)->optionSelect = 3;

        if (IsKeyPressed(KEY_ENTER))
        {
            switch ((*configs)->optionSelect)
            {
            case 0:
                (*configs)->currentScreen = 'G';
                break;
            case 1:
                (*configs)->currentScreen = 'S';
                break;
            case 2:
                (*configs)->currentScreen = 'O';
                break;
            case 3:
                (*configs)->exitGame = true;
                break;
            default:
                TitleWindow(configs);
                break;
            }
        }
    }

    else if ((*configs)->currentScreen == 'G')
    {
        if (!(*configs)->gameOver)
        {
            if (IsKeyPressed(KEY_ESCAPE))
                (*configs)->pause = !(*configs)->pause;

            if (!(*configs)->pause)
            {
                // Player control
                if (IsKeyPressed(KEY_D) && (snake[0].speed.x == 0) && snake->allowMove)
                {
                    snake[0].speed = (Vector2){SQUARE_SIZE, 0};
                    snake->allowMove = false;
                }
                if (IsKeyPressed(KEY_A) && (snake[0].speed.x == 0) && snake->allowMove)
                {
                    snake[0].speed = (Vector2){-SQUARE_SIZE, 0};
                    snake->allowMove = false;
                }
                if (IsKeyPressed(KEY_W) && (snake[0].speed.y == 0) && snake->allowMove)
                {
                    snake[0].speed = (Vector2){0, -SQUARE_SIZE};
                    snake->allowMove = false;
                }
                if (IsKeyPressed(KEY_S) && (snake[0].speed.y == 0) && snake->allowMove)
                {
                    snake[0].speed = (Vector2){0, SQUARE_SIZE};
                    snake->allowMove = false;
                }

                // Snake movement
                for (int i = 0; i < snake->counterTail; i++)
                    snakePosition[i] = snake[i].position;

                if (((*configs)->framesCounter % 5) == 0)
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
                if (((snake[0].position.x) > ((*configs)->screenWidth - offset.x)) ||
                    ((snake[0].position.y) > ((*configs)->screenHeight - offset.y)) ||
                    (snake[0].position.x < 0) || (snake[0].position.y < 0))
                {
                    (*configs)->gameOver = true;
                }

                // Collision with yourself
                for (int i = 1; i < snake->counterTail; i++)
                {
                    if ((snake[0].position.x == snake[i].position.x) && (snake[0].position.y == snake[i].position.y))
                        (*configs)->gameOver = true;
                }

                // Fruit position calculation
                if (!(*fruit)->active)
                {
                    (*fruit)->active = true;
                    (*fruit)->position = (Vector2){GetRandomValue(0, ((*configs)->screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, ((*configs)->screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};

                    for (int i = 0; i < snake->counterTail; i++)
                    {
                        while (((*fruit)->position.x == snake[i].position.x) && ((*fruit)->position.y == snake[i].position.y))
                        {
                            (*fruit)->position = (Vector2){GetRandomValue(0, ((*configs)->screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, ((*configs)->screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                            i = 0;
                        }
                    }
                }

                // Collision
                if ((snake[0].position.x < ((*fruit)->position.x + (*fruit)->size.x) && (snake[0].position.x + snake[0].size.x) > (*fruit)->position.x) &&
                    (snake[0].position.y < ((*fruit)->position.y + (*fruit)->size.y) && (snake[0].position.y + snake[0].size.y) > (*fruit)->position.y))
                {
                    snake->fruitsCounter += 1;
                    snake[snake->counterTail].position = snakePosition[snake->counterTail - 1];
                    snake->counterTail += 1;
                    (*fruit)->active = false;
                }

                (*configs)->framesCounter++;
            }
        }
        else
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                InitScoreGame(configs, fruit);
                (*configs)->gameOver = false;
            }
            if (IsKeyPressed(KEY_G))
            {
                OpenURL("https://github.com/astuno/UPE---LP2Game");
            }
        }
    }
}

ScoreData *getData()
{
    FILE *filePointer = fopen("data/scoreData/score.csv", "r");

    if (filePointer != NULL)
    {
        ScoreData *headerPointer = NULL;
        while (!feof(filePointer))
        {
            char *pos;
            char *name;
            char *score;
            fscanf(filePointer, "%s;%s;%s", pos, name, score);

            if (headerPointer == NULL)
            {
                headerPointer->pos = pos;
                headerPointer->name = name;
                headerPointer->score = score;
                headerPointer->next = NULL;
            }
            else
            {
                ScoreData *current = headerPointer;
                while (current->next != NULL)
                {
                    current = current->next;
                }
                current = current->next;
                current->pos = pos;
                current->name = name;
                current->score = score;
                current->next = NULL;
            }
        }
        fclose(filePointer);
        return headerPointer;
    }
}

void ScoreWindow()
{
    ScoreData *dataToDraw = getData();

    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("SCORE", GetScreenWidth() / 2 - MeasureText("SCORE", 40) / 2, 150, 40, GREEN);
    int yPosi = 150;
    if (dataToDraw != NULL)
    {
        ScoreData *current = dataToDraw;
        while (current->next != NULL)
        {
            DrawText(current->pos, GetScreenWidth() / 2 - MeasureText(current->pos, 40) / 2, yPosi, 25, GREEN);
            DrawText(current->name, GetScreenWidth() / 2 - MeasureText(current->name, 25) / 2, yPosi, 25, GREEN);
            DrawText(current->score, GetScreenWidth() / 2 - MeasureText(current->score, 25) / 2, yPosi, 25, GREEN);
            yPosi += 50;
        }
    }
    EndDrawing();
}

void DrawCurrentScreen(ScreenSettings **configs, Food **fruit)
{
    switch ((*configs)->currentScreen)
    {
    case 'G':
        GameWindow(configs, fruit);
        break;
    case 'S':
        ScoreWindow();
        break;
    case 'O':
        OptionsWindow();
        break;
    default:
        TitleWindow(configs);
        break;
    }
}

// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
    UnloadMusicStream(sound);
    CloseAudioDevice();
    // UnloadFont(font);
}

// Update and Draw (one frame)
void UpdateDrawFrame(ScreenSettings **configs, Food **fruit)
{
    UpdateGame(configs, fruit);
    DrawCurrentScreen(configs, fruit);
}

void InitSound(void)
{
    InitAudioDevice();
    sound = LoadMusicStream("./resources/sounds/02_A-Type Music (version 1.1).mp3");
    SetMasterVolume(0);
    PlayMusicStream(sound);
}